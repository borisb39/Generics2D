

#include "SpacePartitionBody.h"


namespace Generics
{
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

	void SpacePartitionBody::appendCollider(SpacePartitionCollider collider)
	{
		collider.boxHeight = fmax(0, collider.boxHeight);
		collider.boxWidth = fmax(0, collider.boxWidth);
		mColliders.push_back(collider);
		updateAABB(collider);
	}

	SpacePartitionCollider SpacePartitionBody::getColliderAt(int idx) const
	{
		SpacePartitionCollider collider;
		if (idx < mColliders.size())
			collider = mColliders[idx];
		return collider;
	}

	SpacePartitionCollider SpacePartitionBody::getColliderAt_globalFrame(int idx) const
	{
		SpacePartitionCollider collider = getColliderAt(idx);
		collider.position += mPosition;
		return collider;
	}


	int SpacePartitionBody::getNumberOfColliders() const
	{
		return mColliders.size();
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

	void SpacePartitionBody::updateAABB(SpacePartitionCollider collider)
	{
		AABB cAABB = collider.getAABB();
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
}