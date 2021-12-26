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

	Collision SpacePartitionCollider::collisionResolution(const SpacePartitionCollider& a, const SpacePartitionCollider& b)
	{
		Collision collision;
		collision.isTouching = collidersIntersect(a, b);
		if (collision.isTouching)
		{
			if (a.isSensor || b.isSensor) {}
			else if (a.type == ColliderType::BOX && b.type == ColliderType::EDGE)
				collision.response = BoxEdgeDisplacementResponse(a, b);
			else if (a.type == ColliderType::EDGE && b.type == ColliderType::BOX)
				collision.response = BoxEdgeDisplacementResponse(b, a) * -1;
		}
		return collision;
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
		Vect2d boxCorners[5] = {
			{ box.position.x - box.boxWidth / 2, //topleft
			box.position.y + box.boxHeight / 2 },
			{ box.position.x + box.boxWidth / 2, //topright
			box.position.y + box.boxHeight / 2 },
			{ box.position.x + box.boxWidth / 2, //bottomright
			box.position.y - box.boxHeight / 2 },
			{ box.position.x - box.boxWidth / 2, //bottomleft
			box.position.y - box.boxHeight / 2 },
			{ box.position.x - box.boxWidth / 2, //topleft
			box.position.y + box.boxHeight / 2 }
		};
;
		// Edge bounds
		Vect2d edgesBounds[2] = { edge.position + edge.vertice0,
			edge.position + edge.vertice1 };

		// test if one of the edge bound is inside the box
		for (int i = 0; i < 2; i++)
		{
			if (boxCorners[3].x <= edgesBounds[i].x && boxCorners[3].y <= edgesBounds[i].y
				&& boxCorners[1].x >= edgesBounds[i].x && boxCorners[1].y >= edgesBounds[i].y)
				return true;
		}

		// test if one of the box sides intersect with the edge 
		for (int i = 0; i < 4; i++)
		{
			if (doIntersect(boxCorners[i], boxCorners[i+1], edgesBounds[0], edgesBounds[1]))
				return true;
		}

		return false;
	}

	Vect2d BoxEdgeDisplacementResponse(const SpacePartitionCollider& box, const SpacePartitionCollider& edge)
	{
		//Find the displacement of the box with respect to the edge restitution vector
		//in order the move the box "outside" of the edge
		//
		//            OUTSIDE
		//               ^ 
		//               | restitution vector
		//edgeV0 *--------------* edgeV1
		//             INSIDE           
		// Remark : the restitution vector may not be the normal vector

		//final displacement of the box
		Vect2d response = { 0, 0 };

		// box corners
		Vect2d corners[4] = {
			{ box.position.x - box.boxWidth / 2, //topleft
			box.position.y + box.boxHeight / 2 },
			{ box.position.x + box.boxWidth / 2, //topright
			box.position.y + box.boxHeight / 2 },
			{ box.position.x - box.boxWidth / 2, //bottomleft
			box.position.y - box.boxHeight / 2 },
			{ box.position.x + box.boxWidth / 2, //bottomright
			box.position.y - box.boxHeight / 2 }
		};

		// default edge restitution vector is replaced by normal vector
		Vect2d restitutionVector = edge.restitutionVector;
		if (restitutionVector == Vect2d{ 0, 0 })
		{
			restitutionVector.x = -(edge.vertice1.y - edge.vertice0.y);
			restitutionVector.y = edge.vertice1.x - edge.vertice0.x;
		}

		// min and max displacement allowed for the box collider
		// to keep the edge and box bounding box touching each other
		float max_dispx = fmax(0, edge.getAABB().right() + box.boxWidth / 2 - box.position.x);
		float min_dispx = fmin(0, edge.getAABB().left() - box.boxWidth / 2 - box.position.x);
		float max_dispy = fmax(0, edge.getAABB().top() + box.boxHeight / 2 - box.position.y);
		float min_dispy = fmin(0, edge.getAABB().bottom() - box.boxHeight / 2 - box.position.y);

		//the response calculation is based on vector cross product to find the intersection
		//between the restitution vector and edge segment vector for each corner of box collider
	    //https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
		Vect2d p = { edge.position + edge.vertice0 };
		Vect2d r = { edge.vertice1 - edge.vertice0 };
		Vect2d s = { restitutionVector };

		// restition vector is parallel or goes 'inside' edge segment vector
		if ((r ^ s) <= 0)
			return response;

		// for each box corner we calculate the intersection between the edge segment and restitution vector
		// goint throught the corner. The distance between the corner and the intersection
		// is the displacement required to move the corner 'oustisde' of the edge. 
		for (int i = 0; i < 4; i++)
		{
			//box corner
			Vect2d q = corners[i];

			//corner is already ouside of the edge
			if ((r ^ (q - p)) >= 0)
				continue;

			// disp is the displacement of the corner 
			float t = ((q - p) ^ s) / (r ^ s);
			Vect2d disp = p + r * t - q;

			//apply boundary conditions

			//box collider left must touch the edge segment
			if (disp.x > max_dispx)
			{
				disp.y *= max_dispx / disp.x;
				disp.x = max_dispx;
			}
			//box collider right must touch the edge segment
			else if (disp.x < min_dispx)
			{
				disp.y *= min_dispx / disp.x;
				disp.x = min_dispx;
			}
			//box collider bottom must touch the edge segment
			if (disp.y > max_dispy)
			{
				disp.x *= max_dispy / disp.y;
				disp.y = max_dispy;
			}
			//box collider top must touch the edge segment
			else if (disp.y < min_dispy)
			{
				disp.x *= min_dispy / disp.y;
				disp.y = min_dispy;
			}

			// The corner with biggest displacement is used 
			// to define the box displacement.
			if (disp.norm() > response.norm())
				response = disp;
		}

		return response;
	}
}




