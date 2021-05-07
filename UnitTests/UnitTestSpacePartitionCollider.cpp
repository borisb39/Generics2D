#include "CppUnitTest.h"

#include "../SpacePartitionCollider.h"

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
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 5 });
			// box partly inside the edge 1
			collider1.position = { 0, -3 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 0, -3 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 1, 0 }; //outside on the top
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 2 });
			// box partly inside the edge 2
			collider1.position = { 0, -3 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 0, -3 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ -1, 0 }; //outside on the bottom
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, -2 });
			// box partly inside the edge 3
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 0, 1 }; //outside on the left
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -2, 0 });
			// box partly inside the edge 4
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ 0, 0 };
			collider2.vertice1 = Vect2d{ 0, -1 }; //outside on the right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 2, 0 });
			// box partly inside the edge 5
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 }; //outside on the top left
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -2, 2 });
			// box partly inside the edge 6
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 2, 0 };
			collider2.vertice0 = Vect2d{ -1, -1 };
			collider2.vertice1 = Vect2d{ 1, 1 }; //outside on the top left
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -2.5, 2.5 });
			// box partly inside the edge 7
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ -1, 1 };
			collider2.vertice1 = Vect2d{ 1, -1 }; //outside on the top right
			Vect2d reponse = BoxEdgeDisplacementResponse(collider1, collider2);
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 2, 2 });
			// box partly inside the edge 8
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ 1, 1 };
			collider2.vertice1 = Vect2d{ -1, -1 }; //outside on the bottom right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 2, -2 });
			// box partly inside the edge 9
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ 1, -1 };
			collider2.vertice1 = Vect2d{ -1, 1 }; //outside on the bottom left
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ -2, -2 });
			// box partly inside the edge 10
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 3, 0 };
			collider2.vertice0 = Vect2d{ -2, 2 };
			collider2.vertice1 = Vect2d{ 0, -2 }; //outside on the top right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 1.6, 0.8 });
			// box partly inside the edge 11
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 4, 0 };
			collider2.vertice0 = Vect2d{ -2, 2 };
			collider2.vertice1 = Vect2d{ 0, -2 }; //outside on the top right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 2.4, 1.2 });
			// box totaly inside the edge
			collider1.position = { 3, 0 };
			collider1.boxWidth = 4;
			collider1.boxHeight = 4;
			collider2.position = { 11, 0 };
			collider2.vertice0 = Vect2d{ -2, 2 };
			collider2.vertice1 = Vect2d{ 0, -2 }; //outside on the top right
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 8, 4 });
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
			Assert::IsTrue(BoxEdgeDisplacementResponse(collider1, collider2) == Vect2d{ 0, 2 });
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
		}
	};
}