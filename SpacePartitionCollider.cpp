#include "SpacePartitionCollider.h"

namespace Generics
{
    AABB SpacePartitionCollider::getAABB() const
    {
		AABB aabb;
		if (type == ColliderType::BOX)
		{
			aabb.position = position;
			aabb.height = fmax(0, boxHeight);
			aabb.width = fmax(0, boxWidth);
		}
		else if (type == ColliderType::CIRCLE)
		{
			aabb.position = position;
			aabb.height = fmax(0, radius) * 2;
			aabb.width = fmax(0, radius) * 2;
		}
		else if (type == ColliderType::EDGE)
		{
			aabb.position = position
				+ Vect2d{
				(vertice0.x + vertice1.x) / 2,
				(vertice0.y + vertice1.y) / 2
			};
            aabb.height = abs(vertice0.y - vertice1.y);
            aabb.width = abs(vertice0.x - vertice1.x);
		}
        return aabb;
    }
}




