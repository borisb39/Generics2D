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

		TEST_METHOD(collisionResolutionDynamicStatic)
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
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			//Switch body1 to dynamic with no collider : no intersection
			body1 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			//Switch body2 to dynamic with no collider : no intersection
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			//add collider to body2 : no intersection. The collision only considers collision between a dynamic first body and static second body
			body2.appendCollider(collider);
			//Switch body1 to dynamic with no collider and body2 to static with box collider : no intersection
			body1 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::STATIC };
			body2.appendCollider(collider);
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			// append a box collider to body1 : intersection with no displacement (boxes colliders do not respond)
			Collision collisionWithNoDisplacement{ true, {0,0 } };
			body1.appendCollider(collider);
			Assert::IsTrue(AreCollisionsEqual(collisionWithNoDisplacement, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			// create a body1 with edge collider : intersection with displacement
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::STATIC };
			collider.type = ColliderType::EDGE;
			collider.vertice0 = { -3, 0 };
			collider.vertice1 = { 3, 0 };
			body2.appendCollider(collider);
			Collision collision{ true, {0, 3} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #1
			collider.vertice0 = { 1, 3 };
			collider.vertice1 = { 1, -3 };
			body2.appendCollider(collider);
			collision = { true, {0, 3} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #2
			collider.vertice0 = { -1, 3 };
			collider.vertice1 = { -1, -3 };
			body2.appendCollider(collider);
			collision = { true, {2, 0} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #3
			collider.vertice0 = { -2, -3 };
			collider.vertice1 = { -3, -2 };
			body2.appendCollider(collider);
			collision = { true, {2, 0} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #4
			collider.vertice0 = { -3, -2 };
			collider.vertice1 = { -2, -3 };
			body2.appendCollider(collider);
			collision = { true, {0.5, 0.5} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #5
			collider.vertice0 = { 2.5, -3 };
			collider.vertice1 = { 3, -2.5 };
			body2.appendCollider(collider);
			collision = { true, {-0.25, 0.25} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			// add several colliders to body1 and check that the whole body is considered for min displacement calculation
			collider.type = ColliderType::BOX;
			collider.boxHeight = 6;
			collider.boxWidth = 6;
			collider.position = {2, 0};
			body1.appendCollider(collider);
			collision = { true, {0.5, 0.5} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			// modify body position and check that the collision detection is correctly handled #1 : update body1 position
			body1.setPosition({ -2, 0 });
			collision = { true, {-0.25, 0.25} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			// modify body position and check that the collision detection is correctly handled #2 : update body2 position
			body2.setPosition({ 0, -1.1 });
			collision = { true, {0, 1.9} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));

		}

		

		TEST_METHOD(WorldUpdateDynamicBodies_singleBody)
		{

			//create templates

			SpacePartitionBody bodyTemplate;
			SpacePartitionCollider colliderLeftTemplate;
			colliderLeftTemplate.type = ColliderType::EDGE;
			colliderLeftTemplate.vertice0 = { -0.5, -0.5 };
			colliderLeftTemplate.vertice1 = { -0.5, 0.5 };
			SpacePartitionCollider colliderRightTemplate;
			colliderRightTemplate.type = ColliderType::EDGE;
			colliderRightTemplate.vertice0 = { 0.5, 0.5 };
			colliderRightTemplate.vertice1 = { 0.5, -0.5 };
			SpacePartitionCollider colliderTopTemplate;
			colliderTopTemplate.type = ColliderType::EDGE;
			colliderTopTemplate.vertice0 = { -0.5, 0.5 };
			colliderTopTemplate.vertice1 = { 0.5, 0.5 };
			SpacePartitionCollider colliderDownTemplate;
			colliderDownTemplate.type = ColliderType::EDGE;
			colliderDownTemplate.vertice0 = { 0.5, -0.5 };
			colliderDownTemplate.vertice1 = { -0.5, -0.5 };
			SpacePartitionCollider colliderTopLeftTemplate;
			colliderTopLeftTemplate.type = ColliderType::EDGE;
			colliderTopLeftTemplate.vertice0 = { -0.5, -0.5 };
			colliderTopLeftTemplate.vertice1 = { 0.5, 0.5 };
			SpacePartitionCollider colliderTopRightTemplate;
			colliderTopRightTemplate.type = ColliderType::EDGE;
			colliderTopRightTemplate.vertice0 = { -0.5, 0.5 };
			colliderTopRightTemplate.vertice1 = { 0.5, -0.5 };
			SpacePartitionCollider colliderBoxTemplate;
			colliderBoxTemplate.type = ColliderType::BOX;
			colliderBoxTemplate.boxHeight = 1.2;
			colliderBoxTemplate.boxWidth = 1.2;

			// create world :  case
			SpacePartitionWorld world(9, 5, 9, 5);
			double dt = 0.1;

			// create static body1
			bodyTemplate = SpacePartitionBody{ { 0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body2
			bodyTemplate = SpacePartitionBody{ { 1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body3
			bodyTemplate = SpacePartitionBody{ { 2.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body4
			bodyTemplate = SpacePartitionBody{ { 3.5, 1.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body5
			bodyTemplate = SpacePartitionBody{ { 4.5, 2.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body6
			bodyTemplate = SpacePartitionBody{ { 5.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body7
			bodyTemplate = SpacePartitionBody{ { 6.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body8
			bodyTemplate = SpacePartitionBody{ { 7.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body9
			bodyTemplate = SpacePartitionBody{ { 8.5, 4.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body10
			bodyTemplate = SpacePartitionBody{ { -0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body11
			bodyTemplate = SpacePartitionBody{ { -1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderRightTemplate);
			world.addBody(bodyTemplate);

			// create dynamic body
			bodyTemplate = SpacePartitionBody{ { 0.75, 1. }, BodyType::DYNAMIC };
			bodyTemplate.appendCollider(colliderBoxTemplate);
			SpacePartitionBody* player = world.addBody(bodyTemplate);

			// body is moved on the ground 
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 0.75, 1.6 });

			// body velocity to move right
			player->setMaxVelocityMagnitude(999999999999.9);
			player->setVelocity({ 10, 0 });

			// start going right
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 1.75, 1.6 });

			// start going up
			world.updateDynamicBodies(dt);
			Assert::IsFalse(player->getPosition() == Vect2d{ 2.75, 1.6 });
			Assert::IsTrue(player->getPosition() == Vect2d{ 2.575, 1.775 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 3.075, 2.275 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 3.575, 2.775 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 4.075, 3.275 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 4.575, 3.775 });

			//finish moving up and continue moving right
			world.updateDynamicBodies(dt);
			Assert::IsFalse(player->getPosition() == Vect2d{ 5.075, 4.275 });
			Assert::IsTrue(player->getPosition() == Vect2d{ 5.075, 4.6 });
			world.updateDynamicBodies(dt);
			Vect2d pos = player->getPosition();
			Assert::IsTrue(player->getPosition() == Vect2d{ 6.075, 4.6 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.075, 4.6 });

			//blocked with contact of wall at right side
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.4, 4.6 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.4, 4.6 });

			// create dynamic body 2
			bodyTemplate = SpacePartitionBody{ { 0.75, 1. }, BodyType::DYNAMIC };
			bodyTemplate.appendCollider(colliderBoxTemplate);
			SpacePartitionBody* player2 = world.addBody(bodyTemplate);
			player2->setMaxVelocityMagnitude(999999999999.9);
			player2->setVelocity({ -10, 0 });

			// body 2 is moving left
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.4, 4.6 });
			Assert::IsTrue(player2->getPosition() == Vect2d{ -0.25, 1.6 });
			// out of bound bodies do not collides
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.4, 4.6 });
			Assert::IsTrue(player2->getPosition() == Vect2d{ -1.25, 1.6 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.4, 4.6 });
			Assert::IsTrue(player2->getPosition() == Vect2d{ -2.25, 1.6 });

		}

		TEST_METHOD(WorldUpdateDynamicBodies_singleBody_restitutionVector)
		{

			//create templates

			SpacePartitionBody bodyTemplate;
			SpacePartitionCollider colliderLeftTemplate;
			colliderLeftTemplate.type = ColliderType::EDGE;
			colliderLeftTemplate.vertice0 = { -0.5, -0.5 };
			colliderLeftTemplate.vertice1 = { -0.5, 0.5 };
			SpacePartitionCollider colliderRightTemplate;
			colliderRightTemplate.type = ColliderType::EDGE;
			colliderRightTemplate.vertice0 = { 0.5, 0.5 };
			colliderRightTemplate.vertice1 = { 0.5, -0.5 };
			SpacePartitionCollider colliderTopTemplate;
			colliderTopTemplate.type = ColliderType::EDGE;
			colliderTopTemplate.vertice0 = { -0.5, 0.5 };
			colliderTopTemplate.vertice1 = { 0.5, 0.5 };
			SpacePartitionCollider colliderDownTemplate;
			colliderDownTemplate.type = ColliderType::EDGE;
			colliderDownTemplate.vertice0 = { 0.5, -0.5 };
			colliderDownTemplate.vertice1 = { -0.5, -0.5 };
			SpacePartitionCollider colliderTopLeftTemplate;
			colliderTopLeftTemplate.type = ColliderType::EDGE;
			colliderTopLeftTemplate.vertice0 = { -0.5, -0.5 };
			colliderTopLeftTemplate.vertice1 = { 0.5, 0.5 };
			colliderTopLeftTemplate.restitutionVector = { 0, 1 };
			SpacePartitionCollider colliderTopRightTemplate;
			colliderTopRightTemplate.type = ColliderType::EDGE;
			colliderTopRightTemplate.vertice0 = { -0.5, 0.5 };
			colliderTopRightTemplate.vertice1 = { 0.5, -0.5 };
			colliderTopRightTemplate.restitutionVector = { 0, 1 };
			SpacePartitionCollider colliderBoxTemplate;
			colliderBoxTemplate.type = ColliderType::BOX;
			colliderBoxTemplate.boxHeight = 1.2;
			colliderBoxTemplate.boxWidth = 1.2;

			// create world :  case
			SpacePartitionWorld world(9, 5, 9, 5);
			double dt = 0.1;

			// create static body1
			bodyTemplate = SpacePartitionBody{ { 0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body2
			bodyTemplate = SpacePartitionBody{ { 1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body3
			bodyTemplate = SpacePartitionBody{ { 2.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body4
			bodyTemplate = SpacePartitionBody{ { 3.5, 1.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body5
			bodyTemplate = SpacePartitionBody{ { 4.5, 2.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body6
			bodyTemplate = SpacePartitionBody{ { 5.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body7
			bodyTemplate = SpacePartitionBody{ { 6.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body8
			bodyTemplate = SpacePartitionBody{ { 7.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body9
			bodyTemplate = SpacePartitionBody{ { 8.5, 4.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body10
			bodyTemplate = SpacePartitionBody{ { -0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body11
			bodyTemplate = SpacePartitionBody{ { -1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderRightTemplate);
			world.addBody(bodyTemplate);

			// create dynamic body
			bodyTemplate = SpacePartitionBody{ { 0.75, 1. }, BodyType::DYNAMIC };
			bodyTemplate.appendCollider(colliderBoxTemplate);
			SpacePartitionBody* player = world.addBody(bodyTemplate);

			// body is moved on the ground 
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 0.75, 1.6 });

			// body velocity to move right
			player->setMaxVelocityMagnitude(999999999999.9);
			player->setVelocity({ 10, 0 });

			// start going right
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 1.75, 1.6 });

			// start going up
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 2.75, 1.95 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 3.75, 2.95 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 4.75, 3.95 });

			//finish moving up and continue moving right
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 5.75, 4.6 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 6.75, 4.6 });

			//blocked with contact of wall at right side
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.4, 4.6 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.4, 4.6 });
		}

		TEST_METHOD(WorldUpdateDynamicBodies_multiBodies)
		{


			//create templates

			SpacePartitionBody bodyTemplate;
			SpacePartitionCollider colliderLeftTemplate;
			colliderLeftTemplate.type = ColliderType::EDGE;
			colliderLeftTemplate.vertice0 = { -0.5, -0.5 };
			colliderLeftTemplate.vertice1 = { -0.5, 0.5 };
			SpacePartitionCollider colliderRightTemplate;
			colliderRightTemplate.type = ColliderType::EDGE;
			colliderRightTemplate.vertice0 = { 0.5, 0.5 };
			colliderRightTemplate.vertice1 = { 0.5, -0.5 };
			SpacePartitionCollider colliderTopTemplate;
			colliderTopTemplate.type = ColliderType::EDGE;
			colliderTopTemplate.vertice0 = { -0.5, 0.5 };
			colliderTopTemplate.vertice1 = { 0.5, 0.5 };
			SpacePartitionCollider colliderDownTemplate;
			colliderDownTemplate.type = ColliderType::EDGE;
			colliderDownTemplate.vertice0 = { 0.5, -0.5 };
			colliderDownTemplate.vertice1 = { -0.5, -0.5 };
			SpacePartitionCollider colliderTopLeftTemplate;
			colliderTopLeftTemplate.type = ColliderType::EDGE;
			colliderTopLeftTemplate.vertice0 = { -0.5, -0.5 };
			colliderTopLeftTemplate.vertice1 = { 0.5, 0.5 };
			SpacePartitionCollider colliderTopRightTemplate;
			colliderTopRightTemplate.type = ColliderType::EDGE;
			colliderTopRightTemplate.vertice0 = { -0.5, 0.5 };
			colliderTopRightTemplate.vertice1 = { 0.5, -0.5 };
			SpacePartitionCollider colliderBoxTemplate;
			colliderBoxTemplate.type = ColliderType::BOX;
			colliderBoxTemplate.boxHeight = 1.;
			colliderBoxTemplate.boxWidth = 1.;

			// create world :  case
			SpacePartitionWorld world(9, 5, 9, 5);
			double dt = 0.1;

			// create static body1
			bodyTemplate = SpacePartitionBody{ { 0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopRightTemplate);
			world.addBody(bodyTemplate);
			// create static body2
			bodyTemplate = SpacePartitionBody{ { 1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);

			// create dynamic body1
			bodyTemplate = SpacePartitionBody{ { 0, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderBoxTemplate);
			SpacePartitionBody* player = world.addBody(bodyTemplate);

			// the dynamic body collides with the two segemnts and is moving up
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{0, 0.5 });


		}

		TEST_METHOD(WorldUpdateDynamicBodies_multiBodies_speedtest)
		{

			SpacePartitionBody bodyTemplate;
			SpacePartitionCollider colliderLeftTemplate;
			colliderLeftTemplate.type = ColliderType::EDGE;
			colliderLeftTemplate.vertice0 = { -0.5, -0.5 };
			colliderLeftTemplate.vertice1 = { -0.5, 0.5 };
			SpacePartitionCollider colliderRightTemplate;
			colliderRightTemplate.type = ColliderType::EDGE;
			colliderRightTemplate.vertice0 = { 0.5, 0.5 };
			colliderRightTemplate.vertice1 = { 0.5, -0.5 };
			SpacePartitionCollider colliderTopTemplate;
			colliderTopTemplate.type = ColliderType::EDGE;
			colliderTopTemplate.vertice0 = { -0.5, 0.5 };
			colliderTopTemplate.vertice1 = { 0.5, 0.5 };
			SpacePartitionCollider colliderDownTemplate;
			colliderDownTemplate.type = ColliderType::EDGE;
			colliderDownTemplate.vertice0 = { 0.5, -0.5 };
			colliderDownTemplate.vertice1 = { -0.5, -0.5 };
			SpacePartitionCollider colliderTopLeftTemplate;
			colliderTopLeftTemplate.type = ColliderType::EDGE;
			colliderTopLeftTemplate.vertice0 = { -0.5, -0.5 };
			colliderTopLeftTemplate.vertice1 = { 0.5, 0.5 };
			SpacePartitionCollider colliderTopRightTemplate;
			colliderTopRightTemplate.type = ColliderType::EDGE;
			colliderTopRightTemplate.vertice0 = { -0.5, 0.5 };
			colliderTopRightTemplate.vertice1 = { 0.5, -0.5 };
			SpacePartitionCollider colliderBoxTemplate;
			colliderBoxTemplate.type = ColliderType::BOX;
			colliderBoxTemplate.boxHeight = 1.2;
			colliderBoxTemplate.boxWidth = 1.2;

			// create world :  case
			//SpacePartitionWorld world(350, 25, 700, 50);
			//SpacePartitionWorld world(1, 1, 700, 50);
			SpacePartitionWorld world(700, 50, 700, 50);
			double dt = 0.01;

			for (int i = 0; i < 100; ++i)
			{
				bodyTemplate = SpacePartitionBody{ { float(i)+0.5f, 1.0f }, BodyType::STATIC };
				bodyTemplate.appendCollider(colliderTopTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBody{ { float(i) + 0.5f, 10.0f }, BodyType::STATIC };
				bodyTemplate.appendCollider(colliderDownTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBody{ { float(i) + 0.5f, 20.0f }, BodyType::STATIC };
				bodyTemplate.appendCollider(colliderTopTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBody{ { float(i) + 0.5f, 30.0f }, BodyType::STATIC };
				bodyTemplate.appendCollider(colliderDownTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBody{ { float(i) + 0.5f, 40.0f }, BodyType::STATIC };
				bodyTemplate.appendCollider(colliderTopTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBody{ { float(i) + 0.5f, 45.0f }, BodyType::STATIC };
				bodyTemplate.appendCollider(colliderDownTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBody{ { float(i) + 0.5f, 1. }, BodyType::DYNAMIC };
				bodyTemplate.setMaxVelocityMagnitude(999999999999.9);
				bodyTemplate.setVelocity({ 10., 0 });
				bodyTemplate.appendCollider(colliderBoxTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBody{ { float(i) + 0.5f, 10. }, BodyType::DYNAMIC };
				bodyTemplate.setMaxVelocityMagnitude(999999999999.9);
				bodyTemplate.setVelocity({ 10., 0 });
				bodyTemplate.appendCollider(colliderBoxTemplate);
				world.addBody(bodyTemplate);

			}
			

			for (int i = 0; i < 120; ++i)
				world.updateDynamicBodies(dt);

		}
		TEST_METHOD(collisionResolutionDynamicStatic_multiconfig)
		{
			SpacePartitionCollider collider;
			// create the dynamic body with 2 configs
			SpacePartitionBody body1{ { 0, 0 }, BodyType::DYNAMIC };
			collider.type = ColliderType::BOX;
			collider.boxHeight = 6;
			collider.boxWidth = 6;
			body1.appendCollider(collider);
			// create the second config 
			body1.setCurrentConfig("config1");
			collider.type = ColliderType::BOX;
			collider.boxHeight = 3;
			collider.boxWidth = 3;
			body1.appendCollider(collider);
			//create the static body with 1 default config
			SpacePartitionBody body2{ { 0, 0 }, BodyType::STATIC };
			collider.type = ColliderType::EDGE;
			collider.vertice0 = { -1, 0 };
			collider.vertice1 = { 1, 0 };
			body2.appendCollider(collider);
			// test the collision scenarios
			body1.setCurrentConfig("default");
			Collision collision{ true, {0 ,3 } };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			body1.setCurrentConfig("config1");
			collision = { true, {0 ,1.5 } };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			body1.setCurrentConfig("config2"); // new config created with no colliders  -> no collision
			collision = { false, {0 ,0 } };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
			body1.setCurrentConfig("default");
			collision = { true, {0 ,3 } };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionWorld::collisionResolutionDynamicVSstaticBodies(body1, body2)));
		}

	};
}