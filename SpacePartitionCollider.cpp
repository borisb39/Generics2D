#include "SpacePartitionCollider.h"
#include "ToolsLineSegmentsIntersect.hpp"

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

	bool collidersIntersect(const SpacePartitionCollider& a, const SpacePartitionCollider& b)
	{
		bool intersect = false;
		if (a.type == ColliderType::BOX && b.type == ColliderType::BOX)
			intersect = BoxBoxIntersect(a, b);
		else if (a.type == ColliderType::BOX && b.type == ColliderType::EDGE)
			intersect = BoxEdgeIntersect(a, b);
		else if (a.type == ColliderType::EDGE && b.type == ColliderType::BOX)
			intersect = BoxEdgeIntersect(b, a);
		return intersect;
	}

	bool BoxBoxIntersect(const SpacePartitionCollider& a, const SpacePartitionCollider& b)
	{
		return (abs(a.position.x - b.position.x) * 2 <= (a.boxWidth + b.boxWidth)) &&
			(abs(a.position.y - b.position.y) * 2 <= (a.boxHeight + b.boxHeight));
	}

	bool BoxEdgeIntersect(const SpacePartitionCollider& box, const SpacePartitionCollider& edge)
	{
		// Box corners
		Vect2d topLeft = { box.position.x - box.boxWidth / 2,
			box.position.y + box.boxHeight / 2 };
		Vect2d topRight = { box.position.x + box.boxWidth / 2,
			box.position.y + box.boxHeight / 2 };
		Vect2d bottomLeft = { box.position.x - box.boxWidth / 2,
			box.position.y - box.boxHeight / 2 };
		Vect2d bottomRight = { box.position.x + box.boxWidth / 2,
			box.position.y - box.boxHeight / 2 };
		// Edge bounds
		Vect2d edgesBounds[2] = { edge.position + edge.vertice0,
			edge.position + edge.vertice1 };

		// test if one of the edge bound is inside the box
		for (int i = 0; i < 2; i++)
		{
			if (bottomLeft.x <= edgesBounds[i].x && bottomLeft.y <= edgesBounds[i].y
				&& topRight.x >= edgesBounds[i].x && topRight.y >= edgesBounds[i].y)
				return true;
		}

		// test if one of the box sides intersect with the edge 
		Vect2d sides[5] = {topLeft, topRight,  bottomRight, bottomLeft, topLeft };
		for (int i = 0; i < 4; i++)
		{
			if (doIntersect(sides[i], sides[i+1], edgesBounds[0], edgesBounds[1]))
				return true;
		}

		return false;
	}
		
}




