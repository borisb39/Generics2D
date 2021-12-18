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
						Collision collision = collisionResolutionDynamicVSstaticBodies(body, *neighbor);
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

	Collision SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(SpacePartitionBody& body1, SpacePartitionBody& body2)
	{

		// finalCollision decribes the final collision state between the dynamic body1 
		// and static body2 
		Collision finalCollision;

		//We want to test the collision only between a dynamic body1 and a static body2. 
		//This test also covers the case of identical bodies (they don't collide).
		if (!(BodyType::DYNAMIC == body1.getType()) || !(BodyType::STATIC == body2.getType()))
			return finalCollision;

		// The purpose is to find the collider of static body where
		// the displacement of dynamic body that removes the intersection
		// is minimum. For comparison purpose we initialize the finalCollision.response at +infinite.
		finalCollision.response = { 999999999999.9, 999999999999.9 };
		// during the resolution, finalCollision.isTouching is set to true only when
		// a collision response occured. In some cases there may be an intersection
		// with no response (intersection of 2 boxes) that must be tracked
		bool isTouching = false;

		// for each collider of static body
		for (int i = 0; i < body2.getNumberOfColliders(); i++)
		{
			SpacePartitionCollider staticCollider = body2.getColliderAt_globalFrame(i);

			// we want to consider the collider of dynamic body 
			// the deepest 'inside' the static collider -> it represents
			// the displacement that removes the intersection between the bodies

// !! WARNING : if a dynamic collider is 'inside' a static collider but is not touching it it will not be considered
// !! in the algorithm.
// !!     
// !!                 OUTSIDE
// !!                    ^ 
// !!                    | normal
// !!     edgeV0 *--------------* edgeV1     static collider
// !!                  INSIDE
// !! 
// !!                  -----
// !!                 |     |                dynamic collider 'inside' and not touching
// !!                 |     |    
// !!                  -----
// !!
// !! this can cause collision detection issue if the 
// !! time step is not coherent with to collider size and body displacement speed.

			Collision deepestCollision;
			// for each collider of dynamic body
			for (int j = 0; j < body1.getNumberOfColliders(); j++)
			{
				SpacePartitionCollider dynamicCollider = body1.getColliderAt_globalFrame(j);
				Collision collision = SpacePartitionCollider::collisionResolution(dynamicCollider, staticCollider);
				// the maximum magnitude represent the deepest collision
				if (collision.response.norm() > deepestCollision.response.norm()) 
					deepestCollision = collision;
				// track intersection with no response
				if (collision.isTouching) 
					isTouching = true;
			}

			// We finalize the iteration on static collider 
			// by saving its collision state if the associated displacement is minimum
			if (deepestCollision.isTouching && // at this step isTouching is true only if a collision response occured
				deepestCollision.response.norm() < finalCollision.response.norm())
				finalCollision = deepestCollision;
		}

		// if no collision response between the bodies have been found, the final collision state
		// must be reset (+infinite -> 0)
		if (!finalCollision.isTouching) // at this step isTouching is true only if a collision response occured
			finalCollision.response = { 0, 0 };

		// track intersection with no response
		finalCollision.isTouching = isTouching;

		return finalCollision;
	}
}