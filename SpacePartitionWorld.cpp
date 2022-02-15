
#include "SpacePartitionWorld.h"

#include "SpacePartitionGrid.h"
#include "SpacePartitionBody.h"
#include "SpacePartitionCollider.h"
#include "SpacePartitionContactListener.h"

namespace Generics
{
	SpacePartitionWorld::SpacePartitionWorld(int nx, int ny, float width, float height)
	{
		mGrid = new SpacePartitionGrid(nx, ny, width, height);
	}

	SpacePartitionWorld::~SpacePartitionWorld()
	{
		delete mGrid;
		if (mContactListener != nullptr)
		{
			mContactListener->resetContactList();
			mContactListener->setWorld(nullptr);
		}
	}

	SpacePartitionBody* SpacePartitionWorld::createBody(SpacePartitionBodyTemplate bodyTemplate)
	{
		SpacePartitionBody body { 
			bodyTemplate.position, 
			bodyTemplate.type
		};
		body.setWorldID(totalBodiesNumber());
		body.setWorld(this);

		// Add colliders to the world factory 
		// and reference them into body container
		int numberOfColliders = 0;
		for (auto it = bodyTemplate.colliders.begin(); it != bodyTemplate.colliders.end() ; it++)
		{
			body.setCurrentConfig(it->first);
			for (auto collider : it->second)
			{
				mColliders.push_back(collider);
				body.appendCollider(&mColliders.back());
				numberOfColliders += 1;
			}
		}
		
		//add the body to the world factory
		SpacePartitionBody* body_ptr = nullptr;
		if (BodyType::STATIC == body.getType())
		{
			mStaticBodies.push_back(body);
			body_ptr = &mStaticBodies.back();
		}
		else if (BodyType::DYNAMIC == body.getType())
		{
			body.setWorldDynamicID(dynamicBodiesNumber());
			mDynamicBodies.push_back(body);
			body_ptr = &mDynamicBodies.back();
			mDynamicBodiesExternalAccess.push_back(body_ptr);
		}


		// reference the body in the grid
		mGrid->setBody(body_ptr);
		
		//register the parent body ptr to each 
		//new collider added to the world factory
		auto it = mColliders.rbegin();
		for (int i = 0; i < numberOfColliders; i++)
		{
			it->p_body = body_ptr;
			it++;
		}

		return body_ptr;
	}

	SpacePartitionBody* SpacePartitionWorld::getDynamicBodyAt(int idx)
	{
		SpacePartitionBody* body = nullptr;
		if (idx < dynamicBodiesNumber())
			body = mDynamicBodiesExternalAccess[idx];
		return body;
	}

	void SpacePartitionWorld::Step(double dt)
	{
		updateDynamicBodies(dt);
		// Raise contact begin/end
		if (mContactListener != nullptr)
			mContactListener->endSolverStep();
	}

	void SpacePartitionWorld::updateDynamicBodies(double dt)
	{
		// catch elapsed time
		for (auto& body : mDynamicBodies)
		{
			body.updateFromAcceleration(dt);
			mGrid->setBody(&body);
		}

		// each dynamic body will be tested once against each other body
		std::vector<std::vector<bool> > isDone(
			dynamicBodiesNumber(),
			std::vector<bool>(totalBodiesNumber(), false));

		// For each dynamic body we test if 
		// collisions with static bodies 
		// or interference with dynamic bodies occur
		int brk = 0;
		for (auto& body : mDynamicBodies)
		{
			Boundaries boundaries;
			int bID = body.getWorldID();
			// iterate in the grid to fetch the dynamic body neighborhood
			for (auto const& gid : mGrid->getBodygIDs(&body))
			{
				if (gid == mGrid->OOBgID()) continue; // skip out of bounds bodies
				for (auto neighbor : mGrid->getBodiesAtgID(gid))
				{
					int nID = neighbor->getWorldID();
					int nrk = neighbor->getWorldDynamicID();
					if (!(bID==nID) && !isDone[brk][nID])
					{
						// test collision for each neighbor
						Collision collision = SpacePartitionBody::collisionResolution(body, *neighbor);
						boundaries.update(collision); 
						isDone[brk][nID] = true;
						if (nrk != -1) isDone[nrk][bID] = true;
					}
				}
			}
			brk += 1;

			// If collisions occur we update the body position according to the maximum collision
			// displacement values calculated (consider mean displacement in each direction)
			body.setPosition(body.getPosition() + boundaries.correction(), false);
			body.backwardUpdate(dt); // compute velocity from corrected position
			mGrid->setBody(&body);
		}
	}

	void SpacePartitionWorld::setContactListener(SpacePartitionContactListener* contactListener)
	{
		//old listener must be reset
		if (mContactListener != nullptr)
		{
			mContactListener->resetContactList();
			mContactListener->setWorld(nullptr);
		}
		mContactListener = contactListener;
		if (mContactListener != nullptr)
			mContactListener->setWorld(this);
	}

}