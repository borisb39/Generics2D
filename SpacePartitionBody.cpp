

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

	void SpacePartitionBody::createCollider(ColliderProperties properties)
	{
		SpacePartitionCollider collider(properties);
		mColliders.push_back(collider);
	}

	ColliderProperties SpacePartitionBody::getColliderPropertiesAt(int idx)
	{
		ColliderProperties properties;
		if (idx < mColliders.size())
			properties = mColliders[idx].getProperties();
		return properties;
	}

	int SpacePartitionBody::getNumberOfColliders()
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




}