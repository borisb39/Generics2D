#include "CppUnitTest.h"

#include "../SpacePartitionWorld.h"
#include "../SpacePartitionBody.h"
#include "../SpacePartitionCollider.h"
#include "../SpacePartitionContactListener.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Generics;

namespace UnitTests
{
	TEST_CLASS(UnitTestSpacePartitionWorld)
	{
	public:

		TEST_METHOD(WorldUpdateDynamicBodies_singleBody)
		{

			//create templates

			SpacePartitionBodyTemplate bodyTemplate;
			SpacePartitionCollider colliderLeftTemplate;
			colliderLeftTemplate.type = ColliderType::EDGE;
			colliderLeftTemplate.vertice1 = { -0.5, -0.5 };
			colliderLeftTemplate.vertice2 = { -0.5, 0.5 };
			SpacePartitionCollider colliderRightTemplate;
			colliderRightTemplate.type = ColliderType::EDGE;
			colliderRightTemplate.vertice1 = { 0.5, 0.5 };
			colliderRightTemplate.vertice2 = { 0.5, -0.5 };
			SpacePartitionCollider colliderTopTemplate;
			colliderTopTemplate.type = ColliderType::EDGE;
			colliderTopTemplate.vertice1 = { -0.5, 0.5 };
			colliderTopTemplate.vertice2 = { 0.5, 0.5 };
			SpacePartitionCollider colliderDownTemplate;
			colliderDownTemplate.type = ColliderType::EDGE;
			colliderDownTemplate.vertice1 = { 0.5, -0.5 };
			colliderDownTemplate.vertice2 = { -0.5, -0.5 };
			SpacePartitionCollider colliderTopLeftTemplate;
			colliderTopLeftTemplate.type = ColliderType::EDGE;
			colliderTopLeftTemplate.vertice1 = { -0.5, -0.5 };
			colliderTopLeftTemplate.vertice2 = { 0.5, 0.5 };
			SpacePartitionCollider colliderTopRightTemplate;
			colliderTopRightTemplate.type = ColliderType::EDGE;
			colliderTopRightTemplate.vertice1 = { -0.5, 0.5 };
			colliderTopRightTemplate.vertice2 = { 0.5, -0.5 };
			SpacePartitionCollider colliderBoxTemplate;
			colliderBoxTemplate.type = ColliderType::BOX;
			colliderBoxTemplate.boxHeight = 1.2;
			colliderBoxTemplate.boxWidth = 1.2;

			// create world :  case
			SpacePartitionWorld world(9, 5, 9, 5);
			double dt = 0.1;

			// create static body1
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body2
			bodyTemplate = SpacePartitionBodyTemplate{ { 1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body3
			bodyTemplate = SpacePartitionBodyTemplate{ { 2.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body4
			bodyTemplate = SpacePartitionBodyTemplate{ { 3.5, 1.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.createBody(bodyTemplate);
			// create static body5
			bodyTemplate = SpacePartitionBodyTemplate{ { 4.5, 2.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.createBody(bodyTemplate);
			// create static body6
			bodyTemplate = SpacePartitionBodyTemplate{ { 5.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.createBody(bodyTemplate);
			// create static body7
			bodyTemplate = SpacePartitionBodyTemplate{ { 6.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body8
			bodyTemplate = SpacePartitionBodyTemplate{ { 7.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body9
			bodyTemplate = SpacePartitionBodyTemplate{ { 8.5, 4.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderLeftTemplate);
			world.createBody(bodyTemplate);
			// create static body10
			bodyTemplate = SpacePartitionBodyTemplate{ { -0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body11
			bodyTemplate = SpacePartitionBodyTemplate{ { -1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderRightTemplate);
			world.createBody(bodyTemplate);

			// create dynamic body
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.75, 1. }, BodyType::DYNAMIC };
			bodyTemplate.appendCollider(colliderBoxTemplate);
			SpacePartitionBody* player = world.createBody(bodyTemplate);

			// body is moved on the ground 
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 0.75, 1.599 });

			// body max velocity to move right
			player->setMaxVelocityMagnitude(999999999999.9);
			
			// start going right
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 1.75, 1.599 });

			// start going up
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsFalse(player->getPosition() == Vect2d{ 2.75, 1.599 });
			Assert::IsTrue(player->getPosition() == Vect2d{ 2.5755, 1.7735 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 3.0755, 2.2735 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 3.5755, 2.7735 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 4.0755, 3.2735 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 4.5755, 3.7735 });

			//finish moving up and continue moving right
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsFalse(player->getPosition() == Vect2d{ 5.0755, 4.2735 });
			Assert::IsTrue(player->getPosition() == Vect2d{ 5.0755, 4.599 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 6.0755, 4.599 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.0755, 4.599 });

			//blocked with contact of wall at right side
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Vect2d pos = player->getPosition();
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.401, 4.599 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.401, 4.599 });

			// create dynamic body 2
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.75, 1. }, BodyType::DYNAMIC };
			bodyTemplate.appendCollider(colliderBoxTemplate);
			SpacePartitionBody* player2 = world.createBody(bodyTemplate);
			player2->setMaxVelocityMagnitude(999999999999.9);
			
			// body 2 is moving left
			player2->setVelocity({ -10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.401, 4.599 });
			Assert::IsTrue(player2->getPosition() == Vect2d{ -0.25, 1.599 });
			// out of bound bodies do not collides
			player2->setVelocity({ -10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.401, 4.599 });
			Assert::IsTrue(player2->getPosition() == Vect2d{ -1.25, 1.599 });
			player2->setVelocity({ -10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.401, 4.599 });
			Assert::IsTrue(player2->getPosition() == Vect2d{ -2.25, 1.599 });

		}

		TEST_METHOD(WorldUpdateDynamicBodies_singleBody_restitutionVector)
		{

			//create templates

			SpacePartitionBodyTemplate bodyTemplate;
			SpacePartitionCollider colliderLeftTemplate;
			colliderLeftTemplate.type = ColliderType::EDGE;
			colliderLeftTemplate.vertice1 = { -0.5, -0.5 };
			colliderLeftTemplate.vertice2 = { -0.5, 0.5 };
			SpacePartitionCollider colliderRightTemplate;
			colliderRightTemplate.type = ColliderType::EDGE;
			colliderRightTemplate.vertice1 = { 0.5, 0.5 };
			colliderRightTemplate.vertice2 = { 0.5, -0.5 };
			SpacePartitionCollider colliderTopTemplate;
			colliderTopTemplate.type = ColliderType::EDGE;
			colliderTopTemplate.vertice1 = { -0.5, 0.5 };
			colliderTopTemplate.vertice2 = { 0.5, 0.5 };
			SpacePartitionCollider colliderDownTemplate;
			colliderDownTemplate.type = ColliderType::EDGE;
			colliderDownTemplate.vertice1 = { 0.5, -0.5 };
			colliderDownTemplate.vertice2 = { -0.5, -0.5 };
			SpacePartitionCollider colliderTopLeftTemplate;
			colliderTopLeftTemplate.type = ColliderType::EDGE;
			colliderTopLeftTemplate.vertice1 = { -0.5, -0.5 };
			colliderTopLeftTemplate.vertice2 = { 0.5, 0.5 };
			colliderTopLeftTemplate.restitutionVector = { 0, 1 };
			SpacePartitionCollider colliderTopRightTemplate;
			colliderTopRightTemplate.type = ColliderType::EDGE;
			colliderTopRightTemplate.vertice1 = { -0.5, 0.5 };
			colliderTopRightTemplate.vertice2 = { 0.5, -0.5 };
			colliderTopRightTemplate.restitutionVector = { 0, 1 };
			SpacePartitionCollider colliderBoxTemplate;
			colliderBoxTemplate.type = ColliderType::BOX;
			colliderBoxTemplate.boxHeight = 1.2;
			colliderBoxTemplate.boxWidth = 1.2;

			// create world :  case
			SpacePartitionWorld world(9, 5, 9, 5);
			double dt = 0.1;

			// create static body1
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body2
			bodyTemplate = SpacePartitionBodyTemplate{ { 1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body3
			bodyTemplate = SpacePartitionBodyTemplate{ { 2.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body4
			bodyTemplate = SpacePartitionBodyTemplate{ { 3.5, 1.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.createBody(bodyTemplate);
			// create static body5
			bodyTemplate = SpacePartitionBodyTemplate{ { 4.5, 2.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.createBody(bodyTemplate);
			// create static body6
			bodyTemplate = SpacePartitionBodyTemplate{ { 5.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.createBody(bodyTemplate);
			// create static body7
			bodyTemplate = SpacePartitionBodyTemplate{ { 6.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body8
			bodyTemplate = SpacePartitionBodyTemplate{ { 7.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body9
			bodyTemplate = SpacePartitionBodyTemplate{ { 8.5, 4.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderLeftTemplate);
			world.createBody(bodyTemplate);
			// create static body10
			bodyTemplate = SpacePartitionBodyTemplate{ { -0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			// create static body11
			bodyTemplate = SpacePartitionBodyTemplate{ { -1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderRightTemplate);
			world.createBody(bodyTemplate);

			// create dynamic body
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.75, 1. }, BodyType::DYNAMIC };
			bodyTemplate.appendCollider(colliderBoxTemplate);
			SpacePartitionBody* player = world.createBody(bodyTemplate);

			// body is moved on the ground 
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 0.75, 1.599 });

			// body max velocity to move right
			player->setMaxVelocityMagnitude(999999999999.9);

			// start going right
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 1.75, 1.599 });

			// start going up
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 2.75, 1.948 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 3.75, 2.948 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 4.75, 3.948 });

			//finish moving up and continue moving right
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 5.75, 4.599 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 6.75, 4.599 });

			//blocked with contact of wall at right side
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.401, 4.599 });
			player->setVelocity({ 10, 0 });
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{ 7.401, 4.599 });
		}

		TEST_METHOD(WorldUpdateDynamicBodies_multiBodies)
		{


			//create templates

			SpacePartitionBodyTemplate bodyTemplate;
			SpacePartitionCollider colliderLeftTemplate;
			colliderLeftTemplate.type = ColliderType::EDGE;
			colliderLeftTemplate.vertice1 = { -0.5, -0.5 };
			colliderLeftTemplate.vertice2 = { -0.5, 0.5 };
			SpacePartitionCollider colliderRightTemplate;
			colliderRightTemplate.type = ColliderType::EDGE;
			colliderRightTemplate.vertice1 = { 0.5, 0.5 };
			colliderRightTemplate.vertice2 = { 0.5, -0.5 };
			SpacePartitionCollider colliderTopTemplate;
			colliderTopTemplate.type = ColliderType::EDGE;
			colliderTopTemplate.vertice1 = { -0.5, 0.5 };
			colliderTopTemplate.vertice2 = { 0.5, 0.5 };
			SpacePartitionCollider colliderDownTemplate;
			colliderDownTemplate.type = ColliderType::EDGE;
			colliderDownTemplate.vertice1 = { 0.5, -0.5 };
			colliderDownTemplate.vertice2 = { -0.5, -0.5 };
			SpacePartitionCollider colliderTopLeftTemplate;
			colliderTopLeftTemplate.type = ColliderType::EDGE;
			colliderTopLeftTemplate.vertice1 = { -0.5, -0.5 };
			colliderTopLeftTemplate.vertice2 = { 0.5, 0.5 };
			SpacePartitionCollider colliderTopRightTemplate;
			colliderTopRightTemplate.type = ColliderType::EDGE;
			colliderTopRightTemplate.vertice1 = { -0.5, 0.5 };
			colliderTopRightTemplate.vertice2 = { 0.5, -0.5 };
			SpacePartitionCollider colliderBoxTemplate;
			colliderBoxTemplate.type = ColliderType::BOX;
			colliderBoxTemplate.boxHeight = 1.;
			colliderBoxTemplate.boxWidth = 1.;

			// create world :  case
			SpacePartitionWorld world(9, 5, 9, 5);
			double dt = 0.1;

			// create static body1
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopRightTemplate);
			world.createBody(bodyTemplate);
			// create static body2
			bodyTemplate = SpacePartitionBodyTemplate{ { 1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.createBody(bodyTemplate);

			// create dynamic body1
			bodyTemplate = SpacePartitionBodyTemplate{ { 0, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderBoxTemplate);
			SpacePartitionBody* player = world.createBody(bodyTemplate);

			// the dynamic body collides with the two segemnts and is moving up
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{0, 0.5 });


		}

		TEST_METHOD(WorldUpdateDynamicBodies_multiBodies_speedtest)
		{

			SpacePartitionBodyTemplate bodyTemplate;
			SpacePartitionCollider colliderLeftTemplate;
			colliderLeftTemplate.type = ColliderType::EDGE;
			colliderLeftTemplate.vertice1 = { -0.5, -0.5 };
			colliderLeftTemplate.vertice2 = { -0.5, 0.5 };
			SpacePartitionCollider colliderRightTemplate;
			colliderRightTemplate.type = ColliderType::EDGE;
			colliderRightTemplate.vertice1 = { 0.5, 0.5 };
			colliderRightTemplate.vertice2 = { 0.5, -0.5 };
			SpacePartitionCollider colliderTopTemplate;
			colliderTopTemplate.type = ColliderType::EDGE;
			colliderTopTemplate.vertice1 = { -0.5, 0.5 };
			colliderTopTemplate.vertice2 = { 0.5, 0.5 };
			SpacePartitionCollider colliderDownTemplate;
			colliderDownTemplate.type = ColliderType::EDGE;
			colliderDownTemplate.vertice1 = { 0.5, -0.5 };
			colliderDownTemplate.vertice2 = { -0.5, -0.5 };
			SpacePartitionCollider colliderTopLeftTemplate;
			colliderTopLeftTemplate.type = ColliderType::EDGE;
			colliderTopLeftTemplate.vertice1 = { -0.5, -0.5 };
			colliderTopLeftTemplate.vertice2 = { 0.5, 0.5 };
			SpacePartitionCollider colliderTopRightTemplate;
			colliderTopRightTemplate.type = ColliderType::EDGE;
			colliderTopRightTemplate.vertice1 = { -0.5, 0.5 };
			colliderTopRightTemplate.vertice2 = { 0.5, -0.5 };
			SpacePartitionCollider colliderBoxTemplate;
			colliderBoxTemplate.type = ColliderType::BOX;
			colliderBoxTemplate.boxHeight = 1.2;
			colliderBoxTemplate.boxWidth = 1.2;

			// create world :  case
			//SpacePartitionWorld world(100, 10, 700, 50);
			//SpacePartitionWorld world(1, 1, 700, 50);
			//SpacePartitionWorld world(700, 50, 700, 50);
			//SpacePartitionWorld world(70, 5, 700, 50);
			SpacePartitionWorld world(350, 25, 700, 50);

			SpacePartitionContactListener listener;
			world.setContactListener(&listener);

			double dt = 0.01;

			for (int i = 0; i < 100; ++i)
			{
				int ncol = 10;

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i)+0.5f, 1.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderTopTemplate);
				world.createBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 10.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderDownTemplate);
				world.createBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 20.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderTopTemplate);
				world.createBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 30.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderDownTemplate);
				world.createBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 40.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderTopTemplate);
				world.createBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 45.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderDownTemplate);
				world.createBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 1. }, BodyType::DYNAMIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderBoxTemplate);
				SpacePartitionBody* body1 = world.createBody(bodyTemplate);
				body1->setMaxVelocityMagnitude(999999999999.9);
				body1->setVelocity({ 10., 0 });
				
				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 6. }, BodyType::DYNAMIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderBoxTemplate);
				SpacePartitionBody* body2 = world.createBody(bodyTemplate);
				body2->setMaxVelocityMagnitude(999999999999.9);
				body2->setVelocity({ 10., 0 });

			}
			
			for (int i = 0; i < 100; ++i)
				world.Step(dt);

		}

		TEST_METHOD(GetDynamicBodyAt)
		{
			SpacePartitionWorld world(1, 1, 1, 1);
			SpacePartitionBodyTemplate bodyTemplate;
			world.createBody(bodyTemplate);
			//nodynamic body -> nullptr is returned
			Assert::IsTrue(world.getDynamicBodyAt(0) == nullptr);
			Assert::IsTrue(world.getDynamicBodyAt(10) == nullptr);
			bodyTemplate.type = BodyType::DYNAMIC;
			//1 dynamic body -> nullptr is returned for index >= 1
			world.createBody(bodyTemplate);
			Assert::IsTrue(world.getDynamicBodyAt(0) != nullptr);
			Assert::IsTrue(world.getDynamicBodyAt(1) == nullptr);
			Assert::IsTrue(world.getDynamicBodyAt(10) == nullptr);
			//4 dynamic body -> nullptr is returned for index >= 3
			world.createBody(bodyTemplate);
			world.createBody(bodyTemplate);
			world.createBody(bodyTemplate);
			bodyTemplate.type = BodyType::STATIC;
			world.createBody(bodyTemplate);
			Assert::IsTrue(world.getDynamicBodyAt(0) != nullptr);
			Assert::IsTrue(world.getDynamicBodyAt(1) != nullptr);
			Assert::IsTrue(world.getDynamicBodyAt(2) != nullptr);
			Assert::IsTrue(world.getDynamicBodyAt(3) != nullptr);
			Assert::IsTrue(world.getDynamicBodyAt(4) == nullptr);
			Assert::IsTrue(world.getDynamicBodyAt(10) == nullptr);
		}

		TEST_METHOD(ContactListenerCreationDelete)
		{
			//// check that creation / delete of world and contact listener do not raise memory access errors
			///Test case 1 : add listener to world delete listener delete world
			SpacePartitionWorld* world = new SpacePartitionWorld(1, 1, 1, 1);
			//create contacts
			SpacePartitionBodyTemplate bodyTemplate;
			bodyTemplate.type = BodyType::DYNAMIC;
			SpacePartitionCollider collider;
			collider.boxHeight = 2;
			collider.boxWidth = 2;
			bodyTemplate.appendCollider(collider);
			world->createBody(bodyTemplate);
			world->createBody(bodyTemplate);
			world->createBody(bodyTemplate);
			SpacePartitionContactListener* listener = new SpacePartitionContactListener;
			world->setContactListener(listener);
			Assert::IsTrue(listener->getNumberOfContacts() == 0);
			world->Step(0.1);
			Assert::IsTrue(listener->getNumberOfContacts() == 3);
			//remove listener
			delete listener;
			Assert::IsTrue(world->getContactListener() == nullptr);
			//delete world
			delete world;
			///Test case 2 : add listener to world delete world delete listener
			world = new SpacePartitionWorld(1, 1, 1, 1);
			//create contacts
			bodyTemplate.type = BodyType::DYNAMIC;
			world->createBody(bodyTemplate);
			world->createBody(bodyTemplate);
			world->createBody(bodyTemplate);
			listener = new SpacePartitionContactListener;
			world->setContactListener(listener);
			Assert::IsTrue(listener->getNumberOfContacts() == 0);
			world->Step(0.1);
			Assert::IsTrue(listener->getNumberOfContacts() == 3);
			//remove world
			delete world;
			Assert::IsTrue(listener->getNumberOfContacts() == 0);
			//delete listener
			delete listener;
			///Test case 3 : do not add listener to world delete listener delete world
			world = new SpacePartitionWorld(1, 1, 1, 1);
			//create contacts
			bodyTemplate.type = BodyType::DYNAMIC;
			world->createBody(bodyTemplate);
			world->createBody(bodyTemplate);
			world->createBody(bodyTemplate);
			listener = new SpacePartitionContactListener;
			Assert::IsTrue(listener->getNumberOfContacts() == 0);
			world->Step(0.1);
			Assert::IsTrue(listener->getNumberOfContacts() == 0);
			Assert::IsTrue(world->getContactListener() == nullptr);
			//remove listener
			delete listener;
			Assert::IsTrue(world->getContactListener() == nullptr);
			//delete world
			delete world;
			///Test case 4 : do not add listener to world delete world delete listener
			world = new SpacePartitionWorld(1, 1, 1, 1);
			//create contacts
			bodyTemplate.type = BodyType::DYNAMIC;
			world->createBody(bodyTemplate);
			world->createBody(bodyTemplate);
			world->createBody(bodyTemplate);
			listener = new SpacePartitionContactListener;
			Assert::IsTrue(listener->getNumberOfContacts() == 0);
			world->Step(0.1);
			Assert::IsTrue(listener->getNumberOfContacts() == 0);
			Assert::IsTrue(world->getContactListener() == nullptr);
			//remove world
			delete world;
			Assert::IsTrue(listener->getNumberOfContacts() == 0);
			//delete listener
			delete listener;
			///Test case 5 :  add listener to world switch listener delete listener delete listener2 delete world
			world = new SpacePartitionWorld(1, 1, 1, 1);
			//create contacts
			bodyTemplate.type = BodyType::DYNAMIC;
			world->createBody(bodyTemplate);
			world->createBody(bodyTemplate);
			world->createBody(bodyTemplate);
			listener = new SpacePartitionContactListener;
			world->setContactListener(listener);
			Assert::IsTrue(listener->getNumberOfContacts() == 0);
			world->Step(0.1);
			Assert::IsTrue(listener->getNumberOfContacts() == 3);
			Assert::IsTrue(world->getContactListener() == listener);
			// switch listener
			SpacePartitionContactListener* listener2 = new SpacePartitionContactListener;
			world->setContactListener(listener2);
			Assert::IsTrue(world->getContactListener() == listener2);
			Assert::IsTrue(listener->getNumberOfContacts() == 0);
			Assert::IsTrue(listener2->getNumberOfContacts() == 0);
			world->Step(0.1);
			Assert::IsTrue(listener2->getNumberOfContacts() == 3);
			//delete listener
			delete listener;
			Assert::IsTrue(listener2->getNumberOfContacts() == 3);
			Assert::IsTrue(world->getContactListener() == listener2);
			//delete listener2
			delete listener2;
			Assert::IsTrue(world->getContactListener() == nullptr);
			//remove world
			delete world;
		}

		TEST_METHOD(WorldUpdateDynamicBodies_updateBackward)
		{
			SpacePartitionBodyTemplate bodyTemplate;
			SpacePartitionCollider colliderLeftTemplate;
			colliderLeftTemplate.type = ColliderType::EDGE;
			colliderLeftTemplate.vertice1 = { -0.5, -0.5 };
			colliderLeftTemplate.vertice2 = { -0.5, 0.5 };
			SpacePartitionCollider colliderTopTemplate;
			colliderTopTemplate.type = ColliderType::EDGE;
			colliderTopTemplate.vertice1 = { -0.5, 0.5 };
			colliderTopTemplate.vertice2 = { 0.5, 0.5 };
			SpacePartitionCollider colliderBoxTemplate;
			colliderBoxTemplate.type = ColliderType::BOX;
			colliderBoxTemplate.boxHeight = 1.2;
			colliderBoxTemplate.boxWidth = 1.2;

			// create world
			SpacePartitionWorld world(10, 10, 10, 10);
			double dt = 0.1;

			bodyTemplate = SpacePartitionBodyTemplate{ {  0.5f, 1. }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			bodyTemplate = SpacePartitionBodyTemplate{ {  1.5f, 1. }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			bodyTemplate = SpacePartitionBodyTemplate{ {  2.5f, 1. }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.createBody(bodyTemplate);
			bodyTemplate = SpacePartitionBodyTemplate{ {  3.5f, 2. }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderLeftTemplate);
			world.createBody(bodyTemplate);

			bodyTemplate = SpacePartitionBodyTemplate{ {  0.5f, 1. }, BodyType::DYNAMIC };
			bodyTemplate.appendCollider(colliderBoxTemplate);
			SpacePartitionBody* body = world.createBody(bodyTemplate);
			body->setMaxVelocityMagnitude(999);
			

			//test initial properties
			Assert::IsTrue(Vect2d{ 0.5, 1. } == body->getPosition());
			Assert::IsTrue(Vect2d{ 0., 0. } == body->getVelocity());

			// body is moved on the ground
			world.Step(dt);
			Assert::IsTrue(Vect2d{ 0.5, 2.099 } == body->getPosition());
			Assert::IsTrue(Vect2d{ 0., 10.99 } == body->getVelocity());

			// body is moved on the right and fixed on the ground
			body->setVelocity({ 10, -1 });
			world.Step(dt);
			Assert::IsTrue(Vect2d{ 1.5, 2.099 } == body->getPosition());
			Assert::IsTrue(Vect2d{ 10., 0 } == body->getVelocity());
			// body is moved on the right, fixed on the ground but blocked by right wall
			body->setVelocity({ 10, -1 });
			world.Step(dt);
			Assert::IsTrue(Vect2d{ 2.401, 2.099 } == body->getPosition());
			Assert::IsTrue(Vect2d{ 9.0099999, 0 } == body->getVelocity());
			// body is moved on the right, fixed on the ground but blocked by right wall
			body->setVelocity({ 10, -1 });
			world.Step(dt);
			Assert::IsTrue(Vect2d{ 2.401, 2.099 } == body->getPosition());
			Assert::IsTrue(Vect2d{ 0, 0 } == body->getVelocity());
		}

	};
}