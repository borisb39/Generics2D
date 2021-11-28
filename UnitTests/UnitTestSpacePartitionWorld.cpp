#include "CppUnitTest.h"

#include "../SpacePartitionWorld.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Generics;

namespace UnitTests
{
	bool AreCollisionsEqual(Collision collisionA, Collision collisionB)
	{
		return collisionA.isTouching == collisionB.isTouching && collisionA.response == collisionB.response;
	}

	TEST_CLASS(UnitTestSpacePartitionWorld)
	{
	public:

		TEST_METHOD(collisionResolutionDynamicStatic_Mean)
		{
			// create 2 identical static bodies with basic box collider 
			SpacePartitionCollider collider;
			collider.type = ColliderType::BOX;
			collider.boxHeight = 6;
			collider.boxWidth = 6;
			SpacePartitionBody body1{ { 0, 0 }, BodyType::STATIC };
			body1.appendCollider(collider);
			SpacePartitionBody body2{ { 0, 0 }, BodyType::STATIC };
			body1.appendCollider(collider);
			// default collision (no intersection)
			Collision defaultCollision;
			//2 static bodies don't intersect
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Mean(body1, body2)));
			//Switch body1 to dynamic with no collider : no intersection
			body1 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Mean(body1, body2)));
			//Switch body2 to dynamic with no collider : no intersection
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Mean(body1, body2)));
			//add collider to body2 : no intersection. The collision only considers collision between a dynamic first body and static second body
			body2.appendCollider(collider);
			//Switch body1 to dynamic with no collider and body2 to static with box collider : no intersection
			body1 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::STATIC };
			body2.appendCollider(collider);
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Mean(body1, body2)));
			// append a box collider to body1 : intersection with no displacement (boxes colliders do not respond)
			Collision collisionWithNoDisplacement{ true, {0,0 } };
			body1.appendCollider(collider);
			Assert::IsTrue(AreCollisionsEqual(collisionWithNoDisplacement, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Mean(body1, body2)));
			// create a body1 with edge collider : intersection with displacement
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::STATIC };
			collider.type = ColliderType::EDGE;
			collider.vertice0 = {-3, 0};
			collider.vertice1 = {3, 0};
			body2.appendCollider(collider);
			Collision collision{ true, {0, 3} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Mean(body1, body2)));
			// add several colliders to body2 and check that mean displacement response is retourned
			collider.vertice0 = { 0, 3 };
			collider.vertice1 = { 0, -3 };
			body2.appendCollider(collider);
			collision = { true, {1.5, 1.5} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Mean(body1, body2)));
			// modify body position and check that the collision detection is correctly handled #1 : update body1 position
			body1.setPosition({ -2, 0 });
			collision = { true, {2.5, 1.5} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Mean(body1, body2)));
			// modify body position and check that the collision detection is correctly handled #2 : update body2 position
			body2.setPosition({ 0, 1 });
			collision = { true, {2.5, 2} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Mean(body1, body2)));

		}

		TEST_METHOD(collisionResolutionDynamicStatic_Min)
		{
			// create 2 identical static bodies with basic box collider 
			SpacePartitionCollider collider;
			collider.type = ColliderType::BOX;
			collider.boxHeight = 6;
			collider.boxWidth = 6;
			SpacePartitionBody body1{ { 0, 0 }, BodyType::STATIC };
			body1.appendCollider(collider);
			SpacePartitionBody body2{ { 0, 0 }, BodyType::STATIC };
			body1.appendCollider(collider);
			// default collision (no intersection)
			Collision defaultCollision;
			//2 static bodies don't intersect
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			//Switch body1 to dynamic with no collider : no intersection
			body1 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			//Switch body2 to dynamic with no collider : no intersection
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			//add collider to body2 : no intersection. The collision only considers collision between a dynamic first body and static second body
			body2.appendCollider(collider);
			//Switch body1 to dynamic with no collider and body2 to static with box collider : no intersection
			body1 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::STATIC };
			body2.appendCollider(collider);
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			// append a box collider to body1 : intersection with no displacement (boxes colliders do not respond)
			Collision collisionWithNoDisplacement{ true, {0,0 } };
			body1.appendCollider(collider);
			Assert::IsTrue(AreCollisionsEqual(collisionWithNoDisplacement, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			// create a body1 with edge collider : intersection with displacement
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::STATIC };
			collider.type = ColliderType::EDGE;
			collider.vertice0 = { -3, 0 };
			collider.vertice1 = { 3, 0 };
			body2.appendCollider(collider);
			Collision collision{ true, {0, 3} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #1
			collider.vertice0 = { 1, 3 };
			collider.vertice1 = { 1, -3 };
			body2.appendCollider(collider);
			collision = { true, {0, 3} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #2
			collider.vertice0 = { -1, 3 };
			collider.vertice1 = { -1, -3 };
			body2.appendCollider(collider);
			collision = { true, {2, 0} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #3
			collider.vertice0 = { -2, -3 };
			collider.vertice1 = { -3, -2 };
			body2.appendCollider(collider);
			collision = { true, {2, 0} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #4
			collider.vertice0 = { -3, -2 };
			collider.vertice1 = { -2, -3 };
			body2.appendCollider(collider);
			collision = { true, {0.5, 0.5} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #5
			collider.vertice0 = { 2.5, -3 };
			collider.vertice1 = { 3, -2.5 };
			body2.appendCollider(collider);
			collision = { true, {-0.25, 0.25} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			// add several colliders to body1 and check that the whole body is considered for min displacement calculation
			collider.type = ColliderType::BOX;
			collider.boxHeight = 6;
			collider.boxWidth = 6;
			collider.position = {2, 0};
			body1.appendCollider(collider);
			collision = { true, {0.5, 0.5} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			// modify body position and check that the collision detection is correctly handled #1 : update body1 position
			body1.setPosition({ -2, 0 });
			collision = { true, {-0.25, 0.25} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));
			// modify body position and check that the collision detection is correctly handled #2 : update body2 position
			body2.setPosition({ 0, -1.1 });
			collision = { true, {0, 1.9} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies_Min(body1, body2)));

		}
	};
}