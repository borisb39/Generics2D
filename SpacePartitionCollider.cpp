#include "SpacePartitionCollider.h"

namespace Generics
{
    SpacePartitionCollider::SpacePartitionCollider(ColliderProperties properties)
    {
        mType = properties.type;
        mTag = properties.tag;
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
        properties.position = mPosition;
        properties.boxWidth = mBoxWidth;
        properties.boxHeight = mBoxHeight;
        properties.radius = mRadius;
        properties.vertice0 = mVertice0;
        properties.vertice1 = mVertice1;

        return properties;
    }
}




