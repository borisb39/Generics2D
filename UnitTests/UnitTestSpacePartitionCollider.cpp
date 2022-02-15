#include "CppUnitTest.h"

#include "../SpacePartitionCollider.h"
#include "../SpacePartitionBody.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Generics;

namespace UnitTests
{
	TEST_CLASS(UnitTestSpacePartitionCollider)
	{
	public:

		TEST_METHOD(BoxBoxIntersection)
		{
			// classical intersection 1
			SpacePartitionCollider collider1;
			collider1.type = ColliderType::BOX;
			collider1.position = { 5,5 };
			collider1.boxWidth = 10;
			collider1.boxHeight = 10;
			SpacePartitionCollider collider2;
			collider2.type = ColliderType::BOX;
			collider2.position = { 10,10 };
			collider2.boxWidth = 10;
			collider2.boxHeight = 10;
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// classical intersection 2
			collider1.position = { 10,20 };
			collider1.boxWidth = 8;
			collider1.boxHeight = 1;
			collider2.position = { 15,20 };
			collider2.boxWidth = 8;
			collider2.boxHeight = 1;
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// classical intersection 3
			collider1.position = { 10,20 };
			collider1.boxWidth = 1;
			collider1.boxHeight = 8;
			collider2.position = { 10,25 };
			collider2.boxWidth = 1;
			collider2.boxHeight = 8;
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// no intersection 
			collider1.position = { 10,20 };
			collider1.boxWidth = 1;
			collider1.boxHeight = 4.99;
			collider2.position = { 10,25 };
			collider2.boxWidth = 1;
			collider2.boxHeight = 4.99;
			Assert::IsFalse(collidersIntersect(collider1, collider2));
			// no intersection 2
			collider1.position = { 0,0 };
			collider1.boxWidth = 1;
			collider1.boxHeight = 1;
			collider2.position = { 2,2 };
			collider2.boxWidth = 1;
			collider2.boxHeight = 1;
			Assert::IsFalse(collidersIntersect(collider1, collider2));
			// side intersection 1 
			collider1.position = { 10,20 };
			collider1.boxWidth = 1;
			collider1.boxHeight = 5;
			collider2.position = { 10,25 };
			collider2.boxWidth = 1;
			collider2.boxHeight = 5;
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// side intersection 2
			collider1.position = { 0,0 };
			collider1.boxWidth = 1;
			collider1.boxHeight = 1;
			collider2.position = { 1,1 };
			collider2.boxWidth = 1;
			collider2.boxHeight = 1;
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// corner intersection 1
			collider1.position = { 0,0 };
			collider1.boxWidth = 5;
			collider1.boxHeight = 5;
			collider2.position = { 5,5 };
			collider2.boxWidth = 5;
			collider2.boxHeight = 5;
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// corner intersection 2
			collider1.position = { 0,0 };
			collider1.boxWidth = 5;
			collider1.boxHeight = 5;
			collider2.position = { 5,5 };
			collider2.boxWidth = 5;
			collider2.boxHeight = 5;
			Assert::IsTrue(collidersIntersect(collider2, collider1));
			// almost corner intersection 
			collider1.position = { 0,0 };
			collider1.boxWidth = 5;
			collider1.boxHeight = 5;
			collider2.position = { 5,5.00001 };
			collider2.boxWidth = 5;
			collider2.boxHeight = 5;
			Assert::IsFalse(collidersIntersect(collider1, collider2));
			// identical boxes 
			collider1.position = { 5,5 };
			collider1.boxWidth = 5;
			collider1.boxHeight = 5;
			collider2.position = { 5,5 };
			collider2.boxWidth = 5;
			collider2.boxHeight = 5;
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// one box contain  the other 1
			collider1.position = { 5,5 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 5,5 };
			collider2.boxWidth = 5;
			collider2.boxHeight = 5;
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// one box contain  the other 2
			collider1.position = { 5,5 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 5,5 };
			collider2.boxWidth = 5;
			collider2.boxHeight = 5;
			Assert::IsTrue(collidersIntersect(collider2, collider1));
		}

		TEST_METHOD(EdgeEdgeIntersection)
		{
			//Edge edge never intersect
			// crossed edges
			SpacePartitionCollider collider1;
			collider1.type = ColliderType::EDGE;
			collider1.position = { 0,0 };
			collider1.vertice0 = Vect2d{-5,-5};
			collider1.vertice1 = Vect2d{5,5};
			SpacePartitionCollider collider2;
			collider2.type = ColliderType::EDGE;
			collider2.position = { 0,0 };
			collider2.vertice0 = Vect2d{-5,5};
			collider2.vertice1 = Vect2d{5, -5};
			Assert::IsFalse(collidersIntersect(collider1, collider2));
			//colinear edges
			collider1.position = { 0,0 };
			collider1.vertice0 = Vect2d{ -5,0 };
			collider1.vertice1 = Vect2d{ 5,0 };
			collider2.position = { 0,0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 10, 0 };
			Assert::IsFalse(collidersIntersect(collider1, collider2));
		}


		TEST_METHOD(BoxEdgeIntersection)
		{
			// edge inside box
			SpacePartitionCollider collider1;
			collider1.type = ColliderType::BOX;
			collider1.position = { 5,5 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			SpacePartitionCollider collider2;
			collider2.type = ColliderType::EDGE;
			collider2.position = { 5,5 };
			collider2.vertice0 = Vect2d{ 0,0 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// edge crosses one side of the box 
			collider1.position = { 5,7 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 7,7 };
			collider2.vertice0 = Vect2d{ 0,0 };
			collider2.vertice1 = Vect2d{ 0, 4 };
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// edge crosses two side of the box 1
			collider1.position = { 60,-99.5 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 60,-99.5 };
			collider2.vertice0 = Vect2d{ 3,0 };
			collider2.vertice1 = Vect2d{ 0, 4 };
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// edge crosses two side of the box 2
			collider1.position = { 120,220 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 120,220 };
			collider2.vertice0 = Vect2d{ 0,-4 };
			collider2.vertice1 = Vect2d{ 0, 4 };
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// edge colinear to one side of the box
			collider1.position = { 0,0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 0,0 };
			collider2.vertice0 = Vect2d{ 2,-4 };
			collider2.vertice1 = Vect2d{ 2, 4 };
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// edge touch one corner of the box
			collider1.position = { 0,0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 4,2 };
			collider2.vertice0 = Vect2d{ -2,0 };
			collider2.vertice1 = Vect2d{ 2, 0 };
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// edge touch one side of the box
			collider1.position = { 0,0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 4,0 };
			collider2.vertice0 = Vect2d{ -2,0 };
			collider2.vertice1 = Vect2d{ 2, 0 };
			Assert::IsTrue(collidersIntersect(collider1, collider2));
			// edge almost colinear to one side of the box
			collider1.position = { 0,0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 0,0 };
			collider2.vertice0 = Vect2d{ 2.0001,-4 };
			collider2.vertice1 = Vect2d{ 2, 4 };
			Assert::IsFalse(collidersIntersect(collider1, collider2));
			// edge almost touch one corner of the box
			collider1.position = { 0,0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 4.0001,2 };
			collider2.vertice0 = Vect2d{ -2,0 };
			collider2.vertice1 = Vect2d{ 2, 0 };
			Assert::IsFalse(collidersIntersect(collider1, collider2));
			// edge almost touch one side of the box
			collider1.position = { 0,0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 4,0 };
			collider2.vertice0 = Vect2d{ -1.9999,0 };
			collider2.vertice1 = Vect2d{ 2, 0 };
			Assert::IsFalse(collidersIntersect(collider1, collider2));
		}	

		TEST_METHOD(BoxEdgeCollisionResponse)
		{
			// box is already outside of the edge
			SpacePartitionCollider collider1;
			collider1.type = ColliderType::BOX;
			collider1.position = { 10, 10 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			SpacePartitionCollider collider2;
			collider2.type = ColliderType::EDGE;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 1, 0 }; //outside on the top
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 0 });
			// box totaly inside the edge
			collider1.position = { 0, -3 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 1, 0 };  //outside on the top
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 4.999 });
			// box partly inside the edge 1
			collider1.position = { 0, -3 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 0, -3 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 1, 0 }; //outside on the top
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 1.999 });
			// box partly inside the edge 2
			collider1.position = { 0, -3 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 0, -3 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ -1, 0 }; //outside on the bottom
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, -1.999 });
			// box partly inside the edge 3
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 0, 1 }; //outside on the left
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -1.999, 0 });
			// box partly inside the edge 4
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 0, -1 }; //outside on the right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 1.999, 0 });
			// box partly inside the edge 5
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 }; //outside on the top left
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -1.999, 1.999 });
			// box partly inside the edge 6
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 2, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 }; //outside on the top left
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -2.499, 2.499 });
			// box partly inside the edge 7
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ -1, 1 };
			collider2.vertice1 = Vect2d{ 1, -1 }; //outside on the top right
			Vect2d reponse = BoxEdgeDisplacementResponse(collider1, collider2);
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 1.999, 1.999 });
			// box partly inside the edge 8
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ 1, 1 };
			collider2.vertice1 = Vect2d{ -1, -1 }; //outside on the bottom right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 1.999, -1.999 });
			// box partly inside the edge 9
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ 1, -1 };
			collider2.vertice1 = Vect2d{ -1, 1 }; //outside on the bottom left
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -1.999, -1.999 });
			// box partly inside the edge 10
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ -2, 2 };
			collider2.vertice1 = Vect2d{ 0, -2 }; //outside on the top right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 1.5988, 0.7994 });
			// box partly inside the edge 11
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 4, 0 };
			collider2.vertice0 = Vect2d{ -2, 2 };
			collider2.vertice1 = Vect2d{ 0, -2 }; //outside on the top right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 2.3988, 1.1994 });
			// box totaly inside the edge
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 11, 0 };
			collider2.vertice0 = Vect2d{ -2, 2 };
			collider2.vertice1 = Vect2d{ 0, -2 }; //outside on the top right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 7.998, 3.999 });
			// box totaly outside the edge
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { -11, 0 };
			collider2.vertice0 = Vect2d{ -2, 2 };
			collider2.vertice1 = Vect2d{ 0, -2 }; //outside on the top right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 0 });
			// unusual case 1 : box is a point
			collider1.position = { 0, -2 };
			collider1.boxWidth = 0;
			collider1.boxHeight = 0;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 1, 0 }; //outside on the top
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 1.999 });
			// unusual case 2 : edge is a point -> no impact on box displacement
			collider1.position = { 0, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 0, 0 }; 
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 0 });
			// unusual case 3 : edge and box is a point -> no impact on box displacement
			collider1.position = { 0, 0 };
			collider1.boxWidth = 0;
			collider1.boxHeight = 0;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 0, 0 }; 
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 0 });
			// restitution vector : going inside edge -> no disp
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = {0, -1 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 0 });
			// restitution vector : parrallel to edge 1 -> no disp
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = { 1, 1 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 0 });
			// restitution vector : parrallel to edge 2 -> no disp
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = { -1, -1 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 0 });
			// restitution vector : normal vector 1
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = { -1, 1 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -0.999, 0.999 });
			// restitution vector : normal vector 2
			collider1.position = { 0.5, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = { -1, 1 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -1.249, 1.249 });
			// restitution vector : vertical vector 
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = { 0, 1 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 1.998 });
			// restitution vector : horizontal vector 
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = { -1, 0 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -1.998, 0 });
			// restitution vector : specific angle 1 
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = { -1, 3 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -0.4995, 1.4985 });
			// restitution vector : specific angle 2
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = { -3, 1 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -1.4985, 0.4995 });
			// restitution vector : specific angle 3 -> reach edge top bounding box
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = { 1, 3 };
			Vect2d disp = BoxEdgeDisplacementResponse(collider1, collider2);
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0.66633, 1.999 });
			// restitution vector : specific angle 4 -> reach edge left bounding box
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 };
			collider2.restitutionVector = { -3, -1 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -1.999, -0.66633 });
			// restitution vector : specific angle 5 -> reach edge bottom bounding box
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ 1, 1 };
			collider2.vertice1 = Vect2d{ -1, -1 };
			collider2.restitutionVector = { -1, -3 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -0.66633, -1.999 });
			// restitution vector : specific angle 6 -> reach edge right bounding box
			collider1.position = { 0, 0 };
			collider1.boxWidth = 2;
			collider1.boxHeight = 2;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ 1, 1 };
			collider2.vertice1 = Vect2d{ -1, -1 };
			collider2.restitutionVector = { 3, 1 };
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 1.999, 0.66633 });
		}

		TEST_METHOD(GenericCollisionRespone)
		{
			// box is already outside of the edge
			SpacePartitionCollider collider1;
			collider1.type = ColliderType::BOX;
			collider1.position = { 10, 10 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			SpacePartitionCollider collider2;
			collider2.type = ColliderType::EDGE;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 1, 0 }; //outside on the top
			Collision calculatedCollision = SpacePartitionCollider::collisionResolution(collider1, collider2);
			Collision expectedCollision = { false,  { 0, 0} };
			Assert::IsTrue(calculatedCollision.isTouching == expectedCollision.isTouching);
			Assert::IsTrue(calculatedCollision.response == expectedCollision.response);
			// box totaly inside the edge
			collider1.position = { 0, -3 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 1, 0 };  //outside on the top
			calculatedCollision = SpacePartitionCollider::collisionResolution(collider1, collider2);
			expectedCollision = { false,  { 0, 0} };
			Assert::IsTrue(calculatedCollision.isTouching == expectedCollision.isTouching);
			Assert::IsTrue(calculatedCollision.response == expectedCollision.response);
			// box partly inside the edge 1
			collider1.position = { 0, -3 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 0, -3 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 1, 0 }; //outside on the top
			calculatedCollision = SpacePartitionCollider::collisionResolution(collider1, collider2);
			expectedCollision = { true,  { 0, 1.999} };
			Assert::IsTrue(calculatedCollision.isTouching == expectedCollision.isTouching);
			Assert::IsTrue(calculatedCollision.response == expectedCollision.response);
			// sensor -> collision but no response
			expectedCollision = { true,  { 0, 0} };
			collider1.isSensor = true;
			collider2.isSensor = false;
			calculatedCollision = SpacePartitionCollider::collisionResolution(collider1, collider2);
			Assert::IsTrue(calculatedCollision.isTouching == expectedCollision.isTouching);
			Assert::IsTrue(calculatedCollision.response == expectedCollision.response);
			collider1.isSensor = false;
			collider2.isSensor = true;
			calculatedCollision = SpacePartitionCollider::collisionResolution(collider1, collider2);
			Assert::IsTrue(calculatedCollision.isTouching == expectedCollision.isTouching);
			Assert::IsTrue(calculatedCollision.response == expectedCollision.response);
			collider1.isSensor = true;
			collider2.isSensor = true;
			calculatedCollision = SpacePartitionCollider::collisionResolution(collider1, collider2);
			Assert::IsTrue(calculatedCollision.isTouching == expectedCollision.isTouching);
			Assert::IsTrue(calculatedCollision.response == expectedCollision.response);
			expectedCollision = { true,  { 0, 1.999} };
			collider1.isSensor = false;
			collider2.isSensor = false;
			calculatedCollision = SpacePartitionCollider::collisionResolution(collider1, collider2);
			Assert::IsTrue(calculatedCollision.isTouching == expectedCollision.isTouching);
			Assert::IsTrue(calculatedCollision.response == expectedCollision.response);
		}

		TEST_METHOD(BoxEdgeActiveCollision)
		{
			SpacePartitionBody player{ {5, 5}, BodyType::DYNAMIC };
			SpacePartitionCollider collider1;
			collider1.p_body = &player;
			collider1.type = ColliderType::BOX;
			collider1.position = { 0, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			SpacePartitionCollider collider2;
			collider2.type = ColliderType::EDGE;
			collider2.position = { 0, 0 };
			collider2.vertice0 = Vect2d{ -1, 1 };
			collider2.vertice1 = Vect2d{ 1, -1 }; //outside on the top right
			// box is moving inside of the edge -> active
			player.setPosition({ 0,0 });
			Assert::IsTrue(BoxEdgeIsActiveCollision(collider1, collider2, BoxEdgeDisplacementResponse(collider1, collider2)));
			// box is moving outside of the edge -> inactive
			player.setPosition({ 5,5 });
			Assert::IsFalse(BoxEdgeIsActiveCollision(collider1, collider2, BoxEdgeDisplacementResponse(collider1, collider2)));
			// box is moving inside but response moves it outside of the edge -> inactive
			player.setPosition({ -5,-5 });
			player.setPosition({ 0,0 });
			Assert::IsFalse(BoxEdgeIsActiveCollision(collider1, collider2, BoxEdgeDisplacementResponse(collider1, collider2)));
			// box is moving inside but response moves it outside of the edge -> inactive
			player.setPosition({ 3.1,-1 });
			player.setPosition({ 0,0 });
			Assert::IsFalse(BoxEdgeIsActiveCollision(collider1, collider2, BoxEdgeDisplacementResponse(collider1, collider2)));
			// box is moving inside, response moves it outside of the edge but final displacement still goes inside-> active
			player.setPosition({ 10,-1 });
			player.setPosition({ 0,0 });
			Assert::IsTrue(BoxEdgeIsActiveCollision(collider1, collider2, BoxEdgeDisplacementResponse(collider1, collider2)));
			// box is placed in contact with the edge and movement is colinear to the edge -> active
			player.setPosition({ 1,0 });
			player.setPosition(player.getPosition() + BoxEdgeDisplacementResponse(collider1, collider2));
			player.setPosition(player.getPosition() + Vect2d{ -0.5, 0 });
			Assert::IsTrue(BoxEdgeIsActiveCollision(collider1, collider2, BoxEdgeDisplacementResponse(collider1, collider2)));
			// box is placed almost in contact with the edge but outside and movement is almost colinear to the edge and goes inside-> inactive
			player.setPosition({ 1,0 });
			player.setPosition(player.getPosition() + BoxEdgeDisplacementResponse(collider1, collider2) + Vect2d{ -0.0001, 0 });
			player.setPosition(player.getPosition() + Vect2d{ -0.5, 0 });
			Assert::IsFalse(BoxEdgeIsActiveCollision(collider1, collider2, BoxEdgeDisplacementResponse(collider1, collider2)));
			// box is placed almost in contact with the edge but inside and movement is almost colinear to the edge and goes outside-> active
			player.setPosition({ 1,0 });
			player.setPosition(player.getPosition() + BoxEdgeDisplacementResponse(collider1, collider2) + Vect2d{ 0.0001, 0 });
			player.setPosition(player.getPosition() + Vect2d{ -0.5, 0 });
			Assert::IsTrue(BoxEdgeIsActiveCollision(collider1, collider2, BoxEdgeDisplacementResponse(collider1, collider2)));
		}

	};
}