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
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body2
			bodyTemplate = SpacePartitionBodyTemplate{ { 1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body3
			bodyTemplate = SpacePartitionBodyTemplate{ { 2.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body4
			bodyTemplate = SpacePartitionBodyTemplate{ { 3.5, 1.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body5
			bodyTemplate = SpacePartitionBodyTemplate{ { 4.5, 2.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body6
			bodyTemplate = SpacePartitionBodyTemplate{ { 5.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body7
			bodyTemplate = SpacePartitionBodyTemplate{ { 6.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body8
			bodyTemplate = SpacePartitionBodyTemplate{ { 7.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body9
			bodyTemplate = SpacePartitionBodyTemplate{ { 8.5, 4.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body10
			bodyTemplate = SpacePartitionBodyTemplate{ { -0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body11
			bodyTemplate = SpacePartitionBodyTemplate{ { -1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderRightTemplate);
			world.addBody(bodyTemplate);

			// create dynamic body
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.75, 1. }, BodyType::DYNAMIC };
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
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.75, 1. }, BodyType::DYNAMIC };
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

			SpacePartitionBodyTemplate bodyTemplate;
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
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body2
			bodyTemplate = SpacePartitionBodyTemplate{ { 1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body3
			bodyTemplate = SpacePartitionBodyTemplate{ { 2.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body4
			bodyTemplate = SpacePartitionBodyTemplate{ { 3.5, 1.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body5
			bodyTemplate = SpacePartitionBodyTemplate{ { 4.5, 2.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body6
			bodyTemplate = SpacePartitionBodyTemplate{ { 5.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body7
			bodyTemplate = SpacePartitionBodyTemplate{ { 6.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body8
			bodyTemplate = SpacePartitionBodyTemplate{ { 7.5, 3.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body9
			bodyTemplate = SpacePartitionBodyTemplate{ { 8.5, 4.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderLeftTemplate);
			world.addBody(bodyTemplate);
			// create static body10
			bodyTemplate = SpacePartitionBodyTemplate{ { -0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopTemplate);
			world.addBody(bodyTemplate);
			// create static body11
			bodyTemplate = SpacePartitionBodyTemplate{ { -1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderRightTemplate);
			world.addBody(bodyTemplate);

			// create dynamic body
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.75, 1. }, BodyType::DYNAMIC };
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

			SpacePartitionBodyTemplate bodyTemplate;
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
			bodyTemplate = SpacePartitionBodyTemplate{ { 0.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopRightTemplate);
			world.addBody(bodyTemplate);
			// create static body2
			bodyTemplate = SpacePartitionBodyTemplate{ { 1.5, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderTopLeftTemplate);
			world.addBody(bodyTemplate);

			// create dynamic body1
			bodyTemplate = SpacePartitionBodyTemplate{ { 0, 0.5 }, BodyType::STATIC };
			bodyTemplate.appendCollider(colliderBoxTemplate);
			SpacePartitionBody* player = world.addBody(bodyTemplate);

			// the dynamic body collides with the two segemnts and is moving up
			world.updateDynamicBodies(dt);
			Assert::IsTrue(player->getPosition() == Vect2d{0, 0.5 });


		}

		TEST_METHOD(WorldUpdateDynamicBodies_multiBodies_speedtest)
		{

			SpacePartitionBodyTemplate bodyTemplate;
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
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 10.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderDownTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 20.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderTopTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 30.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderDownTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 40.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderTopTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 45.0f }, BodyType::STATIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderDownTemplate);
				world.addBody(bodyTemplate);

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 1. }, BodyType::DYNAMIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderBoxTemplate);
				SpacePartitionBody* body1 = world.addBody(bodyTemplate);
				body1->setMaxVelocityMagnitude(999999999999.9);
				body1->setVelocity({ 10., 0 });

				bodyTemplate = SpacePartitionBodyTemplate{ { float(i) + 0.5f, 6. }, BodyType::DYNAMIC };
				for (int c = 0; c < ncol; c++)
					bodyTemplate.appendCollider(colliderBoxTemplate);
				SpacePartitionBody* body2 = world.addBody(bodyTemplate);
				body2->setMaxVelocityMagnitude(999999999999.9);
				body2->setVelocity({ 10., 0 });

			}
			
			for (int i = 0; i < 100; ++i)
				world.Step(dt);

		}
	};
}