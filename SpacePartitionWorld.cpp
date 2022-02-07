
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

	SpacePartitionBody* SpacePartitionWorld::addBody(SpacePartitionBodyTemplate bodyTemplate)
	{
		SpacePartitionBody body{  bodyTemplate.position, bodyTemplate.type};

		//append colliders to world factory and reference it into body container
		std::vector<SpacePartitionCollider*> newColliders;
		for (auto config : bodyTemplate.colliders)
		{
			body.setCurrentConfig(config.first);
			for (auto collider : config.second)
			{
				mColliders.push_back(collider);
				body.appendCollider(&mColliders.back());
				newColliders.push_back(&mColliders.back());
			}
		}
		
		//assign world id and ptr to the body
		body.setWorldID(totalBodiesNumber());
		if (BodyType::DYNAMIC == body.getType())
			body.setWorldDynamicID(dynamicBodiesNumber());
		body.setWorld(this);

		//append the body to world factory and reference it into the grid
		SpacePartitionBody* body_ptr = nullptr;
		if (BodyType::STATIC == body.getType())
		{
			mStaticBodies.push_back(body);
			body_ptr = &mStaticBodies.back();
			mGrid->setBody(body_ptr);
		}
		else if (BodyType::DYNAMIC == body.getType())
		{
			mDynamicBodies.push_back(body);
			body_ptr = &mDynamicBodies.back();
			mGrid->setBody(body_ptr);
			mDynamicBodiesExternalAccess.push_back(body_ptr);
		}
		
		//register the parent body ptr to each 
		//new collider appenned into world factory
		for (auto collider : newColliders)
			collider->p_body = body_ptr;

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
			int bID = body.getWorldID();
			float positive_dispx = 0.f;
			float negative_dispx = 0.f;
			float positive_dispy = 0.f;
			float negative_dispy = 0.f;
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
						// we store the maximum positive/negative collision displacement values
						positive_dispx = fmax(positive_dispx, collision.response.x);
						negative_dispx = fmin(negative_dispx, collision.response.x);
						positive_dispy = fmax(positive_dispy, collision.response.y);
						negative_dispy = fmin(negative_dispy, collision.response.y);
						isDone[brk][nID] = true;
						if (nrk != -1) isDone[nrk][bID] = true;
					}
				}
			}
			brk += 1;

			// if collisions occur we update the body position according the maximum displacement values
			float divx = (positive_dispx != 0 && negative_dispx != 0) ? 2.f : 1.f;
			float divy = (positive_dispy != 0 && negative_dispy != 0) ? 2.f : 1.f;
			float correction_x = (positive_dispx + negative_dispx)/ divx;
			float correction_y = (positive_dispy + negative_dispy)/ divy;
			Vect2d correction = { correction_x, correction_y };
			Vect2d correctedPosition = body.getPosition() + correction;
			body.setPosition(correctedPosition);
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