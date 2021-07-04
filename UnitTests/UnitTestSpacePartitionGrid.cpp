#include "CppUnitTest.h"

#include "../SpacePartitionGrid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Generics;

namespace UnitTests
{
	TEST_CLASS(SpacePartitinoGrid)
	{
	public:

		TEST_METHOD(OutOfBoundgID)
		{
			//Out of bound gID is equal to number of cells + 1
			SpacePartitionGrid Grid1(10, 11, 25.0, 44.0);
			int OOB1 = 10 * 11 + 1;
			OOB1 == Grid1.OOBgID();
			SpacePartitionGrid Grid2(1, 1, 25.0, 44.0);
			int OOB2 = 1 * 1 + 1;
			OOB2 == Grid2.OOBgID();
		}
		TEST_METHOD(GetGridID)
		{
			SpacePartitionGrid Grid(10, 11, 25.0, 44.0);
			//request out of bound return OUBgID
			Assert::IsTrue(Grid.gridID(-1.0f, 5.6f) == Grid.OOBgID());
			Assert::IsTrue(Grid.gridID(1.0f, -5.6f) == Grid.OOBgID());
			Assert::IsTrue(Grid.gridID(1.0f, 45.0f) == Grid.OOBgID());
			Assert::IsTrue(Grid.gridID(44.6f, 0.f) == Grid.OOBgID());
			//request at min/ max size -> return min/ max gID
			Assert::IsTrue(Grid.gridID(0.f, 0.f) == 0);
			Assert::IsTrue(Grid.gridID(24.999f, 43.999f) == 109);
			//random requests
			Assert::IsTrue(Grid.gridID(12.5f, 22.0f) == 55);
			Assert::IsTrue(Grid.gridID(3.3f, 39.2f) == 91);
		}

		TEST_METHOD(SetBodyWithNoCollider)
		{
			SpacePartitionGrid Grid(10, 11, 25.0, 44.0);
			SpacePartitionBody body;
			//body not registered in the Grid return an empty gIDs
			body.setPosition({ 0, 0 });
			Assert::IsTrue(std::vector<int>{} == Grid.getBodygIDs(&body));
			//body registered at initial position with no collider return gID 0
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{0} == Grid.getBodygIDs(&body));
			//body with no collider moved inside a random cell in the Grid return an unique gID
			body.setPosition({ 12.5, 22.0 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{55} == Grid.getBodygIDs(&body));
			// body with no collider moved betwen 2 cells is considered in the cell with biggest gID
			body.setPosition({ 2.5, 4.0 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{11} == Grid.getBodygIDs(&body));
			// body with no collider moved at upper/right side of the Grid is considered out of bound 
			body.setPosition({ 25, 44 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{Grid.OOBgID()} == Grid.getBodygIDs(&body));
			// body outside of the Grid is considered out of bound 
			body.setPosition({ -25, 44 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{Grid.OOBgID()} == Grid.getBodygIDs(&body));
			// body outside of the Grid can be moved inside of the Grid and being valid again
			body.setPosition({ 12.5, 22.0 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{55} == Grid.getBodygIDs(&body));
		}
		TEST_METHOD(SetBodyWithCollider)
		{
			SpacePartitionGrid Grid(10, 11, 25.0, 44.0);
			int OOB = Grid.OOBgID();
			SpacePartitionBody body;
			SpacePartitionCollider collider;
			collider.type = ColliderType::BOX;
			collider.boxWidth = 2;
			collider.boxHeight = 3;
			collider.position = Vect2d{ 0, 0 };
			body.appendCollider(collider);
			//body not registered in the Grid return an empty gIDs
			body.setPosition({ 0, 0 });
			Assert::IsTrue(std::vector<int>{} == Grid.getBodygIDs(&body));
			//body at center of one cell with no overlapping return one unique gID
			body.setPosition({ 1.25, 2 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{0} == Grid.getBodygIDs(&body));
			body.setPosition({ 13.75, 22 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{55} == Grid.getBodygIDs(&body));
			//body overlaping 2 cells return 2 gIDs
			body.setPosition({ 12.5, 22 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{54, 55} == Grid.getBodygIDs(&body));
			//body overlaping 4 cells return 4 gIDs
			body.setPosition({ 12.5, 20 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{44, 54, 45, 55} == Grid.getBodygIDs(&body));
			//body at bottom left of the Grid return gID 0
			body.setPosition({ 1, 1.5 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{0} == Grid.getBodygIDs(&body));
			//body at top right of the Grid return max gID 
			body.setPosition({ 24.999-1, 43.999-1.5 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{109} == Grid.getBodygIDs(&body));
			//body partly out of bound at bottom left is correctly handled
			body.setPosition({ 0, 0 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{OOB, OOB, OOB, 0} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body, &body, &body} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(0));
			// body moved inside the Grid correctly hanlded
			body.setPosition({ 12.5, 20 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{44, 54, 45, 55} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(55));
			//body partly out of bound at top right is correctly handled
			body.setPosition({ 25, 44 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{109, OOB, OOB, OOB} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body, &body, &body} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(109));
			//body moved inside the Grid correctly hanlded
			body.setPosition({ 12.5, 20 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{44, 54, 45, 55} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(55));
			//body touching sides at top right of the Grid is considered partially out of bound 
			body.setPosition({ 25 - 1, 44 - 1.5 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{109, OOB, OOB, OOB} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body, & body, & body} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(109));
			//body touching side at top of the Grid is considered partially out of bound 
			body.setPosition({ 24.999 - 1, 44 - 1.5 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{109, OOB } == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(109));
			//body touching side at right of the Grid is considered partially out of bound 
			body.setPosition({ 25 - 1, 43.999 - 1.5 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{109, OOB } == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(109));
			//body touching sides at bottom left of the Grid is considered totally inside the Grid
			body.setPosition({ 1, 1.5 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{0} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(0));
			//body covering ceveral cells is correctly handled 1
			collider.boxWidth = 4;
			collider.boxHeight = 6;
			body.appendCollider(collider);
			body.setPosition({ 12.5, 22 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{44, 54, 64, 45, 55, 65} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(65));
			//body covering ceveral cells is correctly handled 2
			body.setPosition({ 13.75, 22 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{44, 54, 64, 45, 55, 65, 46, 56, 66} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(66));
			//body covering ceveral cells is correctly handled 3 
			body.setPosition({ 1.999, 2.999 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{OOB, OOB, OOB, OOB, 0, 10, OOB, 1, 11} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body, &body, &body, &body, &body} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(11));
			//body covering ceveral cells is correctly handled 4
			body.setPosition({ 2, 3 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{ 0, 10, 1, 11} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(11));
			//body covering ceveral cells is correctly handled 5
			body.setPosition({ 24.999-2, 3 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{ 8, 18, 9, 19} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(8));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(9));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(18));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(19));
			//body covering ceveral cells is correctly handled 6
			body.setPosition({ 25 - 2, 3 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{ 8, 18, 9, 19, OOB, OOB} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body, &body} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(8));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(9));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(18));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(19));
			//moving a body is correctly updating the Grid 
			body.setPosition({ 13.75, 22 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{44, 54, 64, 45, 55, 65, 46, 56, 66} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(66));
			body.setPosition({ 2, 3 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{ 0, 10, 1, 11} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(66));
			body.setPosition({ 1.999, 2.999 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{OOB, OOB, OOB, OOB, 0, 10, OOB, 1, 11} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body, & body, & body, & body, & body} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(66));
			body.setPosition({ 13.75, 22 });
			Grid.setBody(&body);
			Assert::IsTrue(std::vector<int>{44, 54, 64, 45, 55, 65, 46, 56, 66} == Grid.getBodygIDs(&body));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body} == Grid.getBodiesAtgID(66));
		}

		TEST_METHOD(SetMultiBodies)
		{
			SpacePartitionGrid Grid(10, 11, 25.0, 44.0);
			int OOB = Grid.OOBgID();
			SpacePartitionCollider collider;
			collider.type = ColliderType::BOX;
			collider.boxWidth = 4;
			collider.boxHeight = 6;
			collider.position = Vect2d{ 0, 0 };
			SpacePartitionBody body1;
			body1.appendCollider(collider);
			body1.setPosition({ 2, 3 });
			SpacePartitionBody body2;
			body2.appendCollider(collider);
			body2.setPosition({ 2, 3 });
			SpacePartitionBody body3;
			body3.appendCollider(collider);
			body3.setPosition({ 13.75, 22 });
			//Initial state : bodies not added into the Grid
			Assert::IsTrue(std::vector<int>{} == Grid.getBodygIDs(&body1));
			Assert::IsTrue(std::vector<int>{} == Grid.getBodygIDs(&body2));
			Assert::IsTrue(std::vector<int>{} == Grid.getBodygIDs(&body3));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(8));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(9));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(18));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(19));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(66));
			//Add 3 bodies 
			Grid.setBody(&body1);
			Assert::IsTrue(std::vector<int>{0, 10, 1, 11} == Grid.getBodygIDs(&body1));
			Assert::IsTrue(std::vector<int>{} == Grid.getBodygIDs(&body2));
			Assert::IsTrue(std::vector<int>{} == Grid.getBodygIDs(&body3));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(8));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(9));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(18));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(19));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(66));
			Grid.setBody(&body2);
			Assert::IsTrue(std::vector<int>{0, 10, 1, 11} == Grid.getBodygIDs(&body1));
			Assert::IsTrue(std::vector<int>{0, 10, 1, 11} == Grid.getBodygIDs(&body2));
			Assert::IsTrue(std::vector<int>{} == Grid.getBodygIDs(&body3));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1, & body2} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1, & body2} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1, & body2} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1, & body2} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(8));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(9));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(18));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(19));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(66));
			Grid.setBody(&body3);
			Assert::IsTrue(std::vector<int>{0, 10, 1, 11} == Grid.getBodygIDs(&body1));
			Assert::IsTrue(std::vector<int>{0, 10, 1, 11} == Grid.getBodygIDs(&body2));
			Assert::IsTrue(std::vector<int>{44, 54, 64, 45, 55, 65, 46, 56, 66} == Grid.getBodygIDs(&body3));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1, & body2} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1, & body2} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1, & body2} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1, & body2} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(8));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(9));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(18));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(19));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(66));
			body1.setPosition({ 25 - 2, 3 });
			Grid.setBody(&body1);
			Assert::IsTrue(std::vector<int>{8, 18, 9, 19, OOB, OOB} == Grid.getBodygIDs(&body1));
			Assert::IsTrue(std::vector<int>{0, 10, 1, 11} == Grid.getBodygIDs(&body2));
			Assert::IsTrue(std::vector<int>{44, 54, 64, 45, 55, 65, 46, 56, 66} == Grid.getBodygIDs(&body3));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1, & body1} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(8));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(9));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(18));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(19));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(66));
			body2.setPosition({ 1.999, 2.999 });
			Grid.setBody(&body2);
			Assert::IsTrue(std::vector<int>{8, 18, 9, 19, OOB, OOB} == Grid.getBodygIDs(&body1));
			Assert::IsTrue(std::vector<int>{OOB, OOB, OOB, OOB, 0, 10, OOB, 1, 11} == Grid.getBodygIDs(&body2));
			Assert::IsTrue(std::vector<int>{44, 54, 64, 45, 55, 65, 46, 56, 66} == Grid.getBodygIDs(&body3));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1, &body1, &body2, &body2, &body2, &body2, & body2} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(8));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(9));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(18));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body1} == Grid.getBodiesAtgID(19));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(66));
			body1.setPosition({ 13.75, 22 });
			Grid.setBody(&body1);
			Assert::IsTrue(std::vector<int>{44, 54, 64, 45, 55, 65, 46, 56, 66} == Grid.getBodygIDs(&body1));
			Assert::IsTrue(std::vector<int>{OOB, OOB, OOB, OOB, 0, 10, OOB, 1, 11} == Grid.getBodygIDs(&body2));
			Assert::IsTrue(std::vector<int>{44, 54, 64, 45, 55, 65, 46, 56, 66} == Grid.getBodygIDs(&body3));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{& body2, & body2, & body2, & body2, & body2} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(8));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(9));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(18));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(19));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3, & body1} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3, & body1} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3, & body1} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3, & body1} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3, & body1} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3, & body1} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3, & body1} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3, & body1} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3, & body1} == Grid.getBodiesAtgID(66));
			body1.setPosition({ 2, 3 });
			Grid.setBody(&body1);
			Assert::IsTrue(std::vector<int>{0, 10, 1, 11} == Grid.getBodygIDs(&body1));
			Assert::IsTrue(std::vector<int>{OOB, OOB, OOB, OOB, 0, 10, OOB, 1, 11} == Grid.getBodygIDs(&body2));
			Assert::IsTrue(std::vector<int>{44, 54, 64, 45, 55, 65, 46, 56, 66} == Grid.getBodygIDs(&body3));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2, & body2, & body2, & body2, & body2} == Grid.getBodiesAtgID(OOB));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2, & body1} == Grid.getBodiesAtgID(0));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2, & body1} == Grid.getBodiesAtgID(10));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2, & body1} == Grid.getBodiesAtgID(1));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body2, & body1} == Grid.getBodiesAtgID(11));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(8));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(9));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(18));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{} == Grid.getBodiesAtgID(19));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(44));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(54));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(64));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(45));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(55));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(65));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(46));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(56));
			Assert::IsTrue(std::vector<SpacePartitionBody*>{&body3} == Grid.getBodiesAtgID(66));
		}
	};
}