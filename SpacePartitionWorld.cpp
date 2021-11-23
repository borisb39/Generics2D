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

	Collision SpacePartitionWorld::collisionResolutionMeanDynamicVSstaticBodies(SpacePartitionBody* body1, SpacePartitionBody* body2)
	{
		// globalCollision decribes the global collision state between the dynamic body1 
		// and static body2 (considering all colliders from both bodies)
		Collision globalCollision;
		// total number of collisions between the 2 bodies
		int nbOfCollision = 0;

		//We want to test the collision only between a dynamic body1 and a static body2. 
		//This test also covers the case of identical bodies (they don't collide).
		if (!(BodyType::DYNAMIC == body1->getType()) || !(BodyType::STATIC == body2->getType()))
			return globalCollision;
	
		// for each collider of dynamic body
		for (int i = 0; i < body1->getNumberOfColliders(); i++)
		{
			SpacePartitionCollider dynamicCollider = body1->getColliderAt(i);
			// for each collider of static body
			for (int j = 0; j < body2->getNumberOfColliders(); j++)
			{
				SpacePartitionCollider staticCollider = body2->getColliderAt(j);
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
}