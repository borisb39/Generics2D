#include "SpacePartitionWorld.h"

namespace Generics
{
	SpacePartitionWorld::SpacePartitionWorld(int nx, int ny, float width, float height)
		: mGrid( nx,  ny,  width,  height)
	{
	}


	SpacePartitionBody* SpacePartitionWorld::addBody(SpacePartitionBody& bodyTemplate)
	{
		bodyTemplate.setWorldID(totalBodiesNumber());

		SpacePartitionBody* body = nullptr;

		if (BodyType::STATIC == bodyTemplate.getType())
		{
			mStaticBodies.push_back(bodyTemplate);
			body = &mStaticBodies.back();
			mGrid.setBody(body);
		}

		else if (BodyType::DYNAMIC == bodyTemplate.getType())
		{
			mDynamicBodies.push_back(bodyTemplate);
			body = &mDynamicBodies.back();
			mGrid.setBody(body);
		}
		
		return body;
	}


	void SpacePartitionWorld::updateDynamicBodies(double dt)
	{
		// catch elapsed time
		for (auto& body : mDynamicBodies)
		{
			body.updateFromAcceleration(dt);
			mGrid.setBody(&body);
		}

		// each dynamic body will be tested once against each other body
		std::vector<std::vector<bool> > isDone(
			dynamicBodiesNumber(),
			std::vector<bool>(totalBodiesNumber(), false));

		// For each dynamic body we test if 
		// collisions with static bodies occur
		int nrk = 0;
		for (auto& body : mDynamicBodies)
		{
			float positive_dispx = 0.f;
			float negative_dispx = 0.f;
			float positive_dispy = 0.f;
			float negative_dispy = 0.f;
			// iterate in the grid to fetch the dynamic body neighborhood
			for (auto const& gid : mGrid.getBodygIDs(&body))
			{
				if (gid == mGrid.OOBgID()) continue; // skip out of bounds bodies
				for (auto const& neighbor : mGrid.getBodiesAtgID(gid))
				{
					int nID = neighbor->getWorldID();
					if (!isDone[nrk][nID])
					{
						// test collision for each neighbor
						Collision collision = SpacePartitionBody::collisionResolutionDynamicVSstatic(body, *neighbor);
						// we store the maximum positive/negative collision displacement values
						positive_dispx = fmax(positive_dispx, collision.response.x);
						negative_dispx = fmin(negative_dispx, collision.response.x);
						positive_dispy = fmax(positive_dispy, collision.response.y);
						negative_dispy = fmin(negative_dispy, collision.response.y);
						isDone[nrk][nID] = true;
					}
				}
			}
			nrk += 1;

			// if collisions occur we update the body position according the maximum displacement values
			float divx = (positive_dispx != 0 && negative_dispx != 0) ? 2.f : 1.f;
			float divy = (positive_dispy != 0 && negative_dispy != 0) ? 2.f : 1.f;
			float correction_x = (positive_dispx + negative_dispx)/ divx;
			float correction_y = (positive_dispy + negative_dispy)/ divy;
			Vect2d correction = { correction_x, correction_y };
			Vect2d correctedPosition = body.getPosition() + correction;
			body.setPosition(correctedPosition);
			mGrid.setBody(&body);
		}
	}
}