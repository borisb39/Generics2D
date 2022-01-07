
#include "SpacePartitionBody.h"

#include "SpacePartitionCollider.h"
#include <cassert>

namespace Generics
{
	SpacePartitionBody::SpacePartitionBody(Vect2d position, BodyType type)
	{
		mPosition = position;
		mType = type;
		mColliders[mCurrentConfig] = {};
	}

	void SpacePartitionBody::setPosition(Vect2d position)
	{
		mPosition = position;
	};

	void SpacePartitionBody::setVelocity(Vect2d velocity)
	{
		mVelocity = checkMagnitude(velocity, mMaxVelocityMagnitude);
	};

	void SpacePartitionBody::setAcceleration(Vect2d acceleration)
	{
		mAcceleration = checkMagnitude(acceleration, mMaxAccelerationMagnitude);
	};

	void SpacePartitionBody::setMaxVelocityMagnitude(float magnitude)
	{
		mMaxVelocityMagnitude = fmax(0, magnitude);
	}

	void SpacePartitionBody::setMaxAccelerationMagnitude(float magnitude)
	{
		mMaxAccelerationMagnitude = fmax(0, magnitude);
	}

	void SpacePartitionBody::updateFromVelocity(double dt)
	{
		Vect2d position = mPosition + mVelocity * dt;
		setPosition(position);
	}

	void SpacePartitionBody::updateFromAcceleration(double dt)
	{
		Vect2d velocity = mVelocity + mAcceleration * dt;
		setVelocity(velocity);
		updateFromVelocity(dt);
	}

	AABB SpacePartitionBody::getAABB_globalFrame() const
	{
		AABB aabb = mAABB;
		aabb.position += mPosition;
		return aabb;
	}

	void SpacePartitionBody::setCurrentConfig(typeBodyconfigID config)
	{
		if (mColliders.find(config) == mColliders.end())
			mColliders[config] = {};
		mCurrentConfig = config;
	}

	void SpacePartitionBody::appendCollider(SpacePartitionCollider* collider)
	{
		collider->boxHeight = fmax(0, collider->boxHeight);
		collider->boxWidth = fmax(0, collider->boxWidth);
		mColliders.at(mCurrentConfig).push_back(collider);
		updateAABB(collider);
	}

	SpacePartitionCollider SpacePartitionBody::getColliderAt(int idx) const
	{
		SpacePartitionCollider collider;
		if (idx < getNumberOfColliders())
			collider = *(mColliders.at(mCurrentConfig)[idx]);
		return collider;
	}

	int SpacePartitionBody::getNumberOfColliders() const
	{
		return mColliders.at(mCurrentConfig).size();
	}

	Vect2d SpacePartitionBody::checkMagnitude(Vect2d vector, float maxMagnitude)
	{
		double magnitude = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
		if (magnitude > maxMagnitude)
		{
			double correctionRatio = maxMagnitude / magnitude;
			vector *= correctionRatio;
		}
		return vector;
	}

	void SpacePartitionBody::setWorldID( int id)
	{
		assert(mWorldID == -1 && "worldID can't be assigned twice");
		assert(id >= 0 && "worldID must be >= 0");
		mWorldID = id;
	}

	void SpacePartitionBody::updateAABB(SpacePartitionCollider* collider)
	{
		AABB cAABB = collider->getAABB();
		// if the collider has no shape we do not consider it
		if (cAABB.width == 0 && cAABB.height == 0)
			return;

		//if the body has no shape we initialize the body AABB with le collider AABB
		if (mAABB.width == 0 && mAABB.height == 0)
			mAABB = cAABB;
		else
		{
			//else we include the collider AABB inside the body AABB
			float top = fmax(cAABB.top(), mAABB.top());
			float bottom = fmin(cAABB.bottom(), mAABB.bottom());
			float right = fmax(cAABB.right(), mAABB.right());
			float left = fmin(cAABB.left(), mAABB.left());
			mAABB.position.y = (top + bottom) / 2;
			mAABB.position.x = (right + left) / 2;
			mAABB.height = top - bottom;
			mAABB.width = right - left;
		}
	}

	Collision SpacePartitionBody::collisionResolutionDynamicVSstatic(SpacePartitionBody& body1, SpacePartitionBody& body2)
	{

		// finalCollision decribes the final collision state between the dynamic body1 
		// and static body2 
		Collision finalCollision;

		//We want to test the collision only between a dynamic body1 and a static body2. 
		//This test also covers the case of identical bodies (they don't collide).
		if (!(BodyType::DYNAMIC == body1.getType()) || !(BodyType::STATIC == body2.getType()))
			return finalCollision;

		//We test first rought collisison detection via bodies AABB
		if (!body1.getAABB_globalFrame().intersect(body2.getAABB_globalFrame()))
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
		for (auto staticCollider : body2.mColliders.at(body2.mCurrentConfig))
		{
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
			for (auto dynamicCollider : body1.mColliders.at(body1.mCurrentConfig))
			{
				Collision collision = SpacePartitionCollider::collisionResolution(*dynamicCollider, *staticCollider);
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

	void SpacePartitionBodyTemplate::appendCollider(SpacePartitionCollider collider, typeBodyconfigID config)
	{
		if (colliders.find(config) == colliders.end())
			colliders[config] = {};
		colliders.at(config).push_back(collider);
	}
}