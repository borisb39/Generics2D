#include "SpacePartitionCollider.h"

namespace Generics
{
    SpacePartitionCollider::SpacePartitionCollider(ColliderProperties properties)
    {
        mType = properties.type;
        mTag = properties.tag;
        mIsEnabled = properties.isEnabled;
        mIsSensor = properties.isSensor;
        mPosition = properties.position;
        mBoxWidth = fmax(0, properties.boxWidth);
        mBoxHeight = fmax(0, properties.boxHeight);
        mRadius = fmax(0, properties.radius);
        mVertice0 = properties.vertice0;
        mVertice1 = properties.vertice1;
    }

    ColliderProperties SpacePartitionCollider::getProperties() const
    {
        ColliderProperties properties;
        properties.type = mType;
        properties.tag = mTag;
        properties.isEnabled = mIsEnabled;
        properties.isSensor = mIsSensor;
        properties.position = mPosition;
        properties.boxWidth = mBoxWidth;
        properties.boxHeight = mBoxHeight;
        properties.radius = mRadius;
        properties.vertice0 = mVertice0;
        properties.vertice1 = mVertice1;

        return properties;
    }

    AABB SpacePartitionCollider::getAABB() const
    {
		AABB aabb;
		if (mType == ColliderType::BOX)
		{
			aabb.position = mPosition;
			aabb.height = mBoxHeight;
			aabb.width = mBoxWidth;
		}
		else if (mType == ColliderType::CIRCLE)
		{
			aabb.position = mPosition;
			aabb.height = mRadius * 2;
			aabb.width = mRadius * 2;
		}
		else if (mType == ColliderType::EDGE)
		{
			aabb.position = mPosition
				+ Vect2d{
				(mVertice0.x + mVertice1.x) / 2,
				(mVertice0.y + mVertice1.y) / 2
			};
            aabb.height = abs(mVertice0.y - mVertice1.y);
            aabb.width = abs(mVertice0.x - mVertice1.x);
		}
        return aabb;
    }
}




