#include "SpacePartitionWorld.h"

namespace Generics
{
	SpacePartitionWorld::SpacePartitionWorld(int nx, int ny, float width, float height)
		: mGrid( nx,  ny,  width,  height)
	{
	}


	void SpacePartitionWorld::addBody(SpacePartitionBody& body)
	{
		if (BodyType::STATIC == body.getType())
		{
			mStaticBodies.push_back(body);
			mGrid.setBody(&mStaticBodies.back());
		}
		else if (BodyType::DYNAMIC == body.getType())
		{
			mDynamicBodies.push_back(body);
			mGrid.setBody(&mDynamicBodies.back());
		}
	}

	Collision SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Mean(SpacePartitionBody& body1, SpacePartitionBody& body2)
	{
		// globalCollision decribes the global collision state between the dynamic body1 
		// and static body2 (considering all colliders from both bodies)
		Collision globalCollision;
		// total number of collisions between the 2 bodies
		int nbOfCollision = 0;

		//We want to test the collision only between a dynamic body1 and a static body2. 
		//This test also covers the case of identical bodies (they don't collide).
		if (!(BodyType::DYNAMIC == body1.getType()) || !(BodyType::STATIC == body2.getType()))
			return globalCollision;
	
		// for each collider of dynamic body
		for (int i = 0; i < body1.getNumberOfColliders(); i++)
		{
			SpacePartitionCollider dynamicCollider = body1.getColliderAt_globalFrame(i);
			// for each collider of static body
			for (int j = 0; j < body2.getNumberOfColliders(); j++)
			{
				SpacePartitionCollider staticCollider = body2.getColliderAt_globalFrame(j);
				// check if there is an interference between the colliders
				Collision collision = SpacePartitionCollider::collisionResolution(dynamicCollider, staticCollider);
				if (collision.isTouching)
				{
					// update the global collision state if there is an interferance
					globalCollision.isTouching = true;
					globalCollision.response += collision.response;
					nbOfCollision += 1;
				}
			}
		}

		// When want to consider the mean collision response value
		if (nbOfCollision != 0)
			globalCollision.response /= nbOfCollision;

		return globalCollision;
	}

	Collision SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(SpacePartitionBody& body1, SpacePartitionBody& body2)
	{
		// finalCollision decribes the final collision state between the dynamic body1 
		// and static body2 
		Collision finalCollision;
		// The purpose is to find the collider of static body where
		// the displacement of dynamic body that removes the intersection
		// is minimum. For comparison purpose we initialize the finalCollision.response at +infinite.
		constexpr float inf = std::numeric_limits<float>::infinity();
		finalCollision.response = { inf, inf };
		// during the resolution, finalCollision.isTouching is set to true only when
		// a collision response occured. In some cases there may be an intersection
		// with no response (intersection of 2 boxes) that must be tracked
		bool isTouching = false;

		//We want to test the collision only between a dynamic body1 and a static body2. 
		//This test also covers the case of identical bodies (they don't collide).
		if (!(BodyType::DYNAMIC == body1.getType()) || !(BodyType::STATIC == body2.getType()))
			return finalCollision;

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