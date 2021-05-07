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
		
	Vect2d projectionPointToLineSegment(Vect2d& P1, Vect2d& P2, Vect2d& P3)
	{
		//Find the projection coordinate H of a point P3 on a line segment P1, P2
		//The equation of the line is defined as P = P1 + u(P2 - P1)
		//http://paulbourke.net/geometry/pointlineplane/
		

		//square distance between P1 and P2
		float L2 = pow((P2.x - P1.x), 2) + pow((P2.y - P1.y), 2);

		//the line segment is a point : the projection is that point
		if (L2 == 0)
			return P1;

		//relative postion of H on line segment
		float u = ((P3.x - P1.x) * (P2.x - P1.x) + (P3.y - P1.y) * (P2.y - P1.y)) / L2;

		//projection coordinates
		Vect2d H = { P1.x + u * (P2.x - P1.x) , P1.y + u * (P2.y - P1.y) };

		return H;
	}

	Vect2d BoxEdgeDisplacementResponse(const SpacePartitionCollider& box, const SpacePartitionCollider& edge)
	{
		//Find the displacement of the box with respect to the edge normal vector
		//in order the move the box "outside" of the edge
		//
		//            OUTSIDE
        //               ^ 
		//               | normal
		//edgeV0 *--------------* edgeV1
		//             INSIDE

		// Box corners
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

		//final displacement of the box
		Vect2d response = { 0, 0 };

		//we want to find the projection coordinate H
		//of P3 on the line segment P1 P2
		//P3 is a box corner
		//P1 P2 are the edge coordinates
		Vect2d P1 = edge.position + edge.vertice0;
		Vect2d P2 = edge.position + edge.vertice1;
		Vect2d P3;
		Vect2d H;
		Vect2d P1P2 = P2 - P1; //vector P1->P2 (edge vector)
		Vect2d P3H; // vector P3->H (opposite of edge normal vector going throw box corner)
		
		//D2 is the square distance between H and P3
		float D2max = 0;
		float D2;

		// for each box corner P3 we calculate the projection coordinate H 
		// associated and check if the corner is "inside" the edge. The corner
		// the furthest inside the edge is the one used to define the box displacement
		for (int i = 0; i < 4; i++)
		{
			P3 = corners[i];
			H = projectionPointToLineSegment(P1, P2, P3);
			P3H = H - P3;
			// the sign of the cross product between P1->P2 and P3->H 
			// says if P3 is considered as "inside"
			if ((P1P2.x * P3H.y - P1P2.y * P3H.x) > 0 )
			{
				D2 = pow((P3.x - H.x), 2) + pow((P3.y - H.y), 2);
				//if the corner is the furthest inside
				if (D2 > D2max)
				{
					D2max = D2;
					// the displacement of the box is the opposite 
					// of edge normal vector going throw the corner
					response = P3H; 
				}
			}
		}
		return response;
	}
}




