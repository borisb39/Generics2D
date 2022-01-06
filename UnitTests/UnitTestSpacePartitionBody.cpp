#include "CppUnitTest.h"

#include "../SpacePartitionBody.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Generics;

namespace UnitTests
{
	TEST_CLASS(UnitTestSpacePartitionBody)
	{
	public:

		bool AreCollisionsEqual(Collision collisionA, Collision collisionB)
		{
			return collisionA.isTouching == collisionB.isTouching && collisionA.response == collisionB.response;
		}

		TEST_METHOD(SetAndGetTimeRelativeProperties)
		{ 
			//position
			SpacePartitionBody body1;
			Assert::IsTrue(body1.getPosition() == Vect2d{ 0, 0 });
			body1.setPosition(Vect2d{ 10, 20 });
			Assert::IsTrue(body1.getPosition() == Vect2d{ 10, 20 });
			//velocity
			SpacePartitionBody body2;
			Assert::IsTrue(body2.getVelocity() == Vect2d{ 0, 0 });
			body2.setVelocity(Vect2d{ 10, 20 });
			Assert::IsFalse(body2.getVelocity() == Vect2d{ 10, 20 });
			Assert::IsTrue(body2.getVelocity() == Vect2d{ 0, 0 });
			body2.setMaxVelocityMagnitude(100);
			body2.setVelocity(Vect2d{ 10, 20 });
			Assert::IsTrue(body2.getVelocity() == Vect2d{ 10, 20 });
			body2.setVelocity(Vect2d{ -10, -20 });
			Assert::IsTrue(body2.getVelocity() == Vect2d{ -10, -20 });
			body2.setVelocity(Vect2d{ -100, 100 });
			Assert::IsFalse(body2.getVelocity() == Vect2d{ -100, 100 });
			Assert::IsTrue(body2.getVelocity() == Vect2d{ -70.71068, 70.71068 });
			//acceleration
			SpacePartitionBody body3;
			Assert::IsTrue(body3.getAcceleration() == Vect2d{ 0, 0 });
			body3.setAcceleration(Vect2d{ 10, 20 });
			Assert::IsFalse(body3.getAcceleration() == Vect2d{ 10, 20 });
			Assert::IsTrue(body3.getAcceleration() == Vect2d{ 0, 0 });
			body3.setMaxAccelerationMagnitude(100);
			body3.setAcceleration(Vect2d{ 10, 20 });
			Assert::IsTrue(body3.getAcceleration() == Vect2d{ 10, 20 });
			body3.setAcceleration(Vect2d{ -10, -20 });
			Assert::IsTrue(body3.getAcceleration() == Vect2d{ -10, -20 });
			body3.setAcceleration(Vect2d{ -100, 100 });
			Assert::IsFalse(body3.getAcceleration() == Vect2d{ -100, 100 });
			Assert::IsTrue(body3.getAcceleration() == Vect2d{ -70.71068, 70.71068 });
			//non negative max velocity magnitude
			SpacePartitionBody body4;
			body4.setMaxVelocityMagnitude(100);
			body4.setVelocity(Vect2d{ 100, 100 });
			Assert::IsFalse(body4.getVelocity() == Vect2d{ 100, 100 });
			Assert::IsTrue(body4.getVelocity() == Vect2d{ 70.71068, 70.71068 });
			body4.setMaxVelocityMagnitude(-100);
			Assert::IsTrue(body4.getVelocity() == Vect2d{ 70.71068, 70.71068 });
			body4.setVelocity(Vect2d{ 100, 100 });
			Assert::IsTrue(body4.getVelocity() == Vect2d{ 0, 0 });
			//non negative max acceleration magnitude
			SpacePartitionBody body5;
			body5.setMaxAccelerationMagnitude(100);
			body5.setAcceleration(Vect2d{ 100, 100 });
			Assert::IsFalse(body5.getAcceleration() == Vect2d{ 100, 100 });
			Assert::IsTrue(body5.getAcceleration() == Vect2d{ 70.71068, 70.71068 });
			body5.setMaxAccelerationMagnitude(-100);
			Assert::IsTrue(body5.getAcceleration() == Vect2d{ 70.71068, 70.71068 });
			body5.setAcceleration(Vect2d{ 100, 100 });
			Assert::IsTrue(body5.getAcceleration() == Vect2d{ 0, 0 });
		}

		TEST_METHOD(UpdateFromVelocity)
		{
			float dt = 0.1;
			SpacePartitionBody body;
			body.setMaxVelocityMagnitude(100);
			body.setVelocity(Vect2d{ -10, 22.4 });
			Vect2d th_position({ 0, 0 });
			for (int i = 0; i < 10; i++)
			{
				body.updateFromVelocity(dt);
				Assert::IsTrue(body.getPosition() == (th_position += Vect2d{ -10, 22.4 } * dt));
			}
		}
		TEST_METHOD(UpdateFromAcceleration)
		{
			float dt = 0.1;
			SpacePartitionBody body;
			body.setMaxVelocityMagnitude(100);
			body.setMaxAccelerationMagnitude(100);
			body.setAcceleration(Vect2d{ -10, 22.4 });
			Vect2d th_position({ 0, 0 });
			Vect2d th_velocity({ 0, 0 });
			for (int i = 0; i < 10; i++)
			{
				body.updateFromAcceleration(dt);
				Assert::IsTrue(body.getVelocity() == (th_velocity += Vect2d{ -10, 22.4 } *dt));
				Assert::IsTrue(body.getPosition() == (th_position += th_velocity *dt));
			}
		}
		TEST_METHOD(UpdateFromAccelerationReachesMaxVelocityBound)
		{
			float dt = 0.11;
			SpacePartitionBody body;
			body.setMaxVelocityMagnitude(100);
			body.setMaxAccelerationMagnitude(100);
			body.setAcceleration(Vect2d{ 0, -10 });
			Vect2d th_position({ 0, 0 });
			Vect2d th_velocity({ 0, 0 });
			Vect2d th_position_corrected({ 0, 0 });
			Vect2d th_velocity_corrected({ 0, 0 });
			for (int i = 0; i < 1000; i++)
			{
				body.updateFromAcceleration(dt);
				th_velocity += Vect2d{ 0, -10 } *dt;
				th_position += th_velocity * dt;
				th_velocity_corrected += Vect2d{ 0, -10 } *dt;
				th_velocity_corrected.y = fmax(-100, th_velocity_corrected.y);
				th_position_corrected += th_velocity_corrected * dt;
			}
			Assert::IsFalse(body.getVelocity() == th_velocity);
			Assert::IsFalse(body.getPosition() == th_position);
			Assert::IsTrue(body.getVelocity() == th_velocity_corrected);
			Assert::IsTrue(body.getPosition() == th_position_corrected);
		}

		TEST_METHOD(CollidersManagement)
		{
			// create Colliders, append to and get from a body
			std::vector< SpacePartitionCollider > th_colliders; // what we expect 
			SpacePartitionBody body; // body where the colliders will be added
			SpacePartitionCollider defaultCollider;
			//first collider : default
			SpacePartitionCollider collider;
			body.appendCollider(collider);
			th_colliders.push_back(collider);
			//2st collider : Box
			collider.tag = "BoxCollider";
			collider.type = ColliderType::BOX;
			collider.boxWidth = 10;
			collider.boxHeight = 10;
			collider.position = Vect2d{ 3, 2 };
			collider.isSensor = true;
			body.appendCollider(collider);
			th_colliders.push_back(collider);
			//3rd collider : Box
			collider.tag = "BoxCollider";
			collider.type = ColliderType::BOX;
			collider.boxWidth = 5.3;
			collider.boxHeight = 12.6;
			collider.position = Vect2d{ -1, 0 };
			collider.isSensor = true;
			body.appendCollider(collider);
			th_colliders.push_back(collider);
			//4th collider : Edge
			collider.tag = "EdgeCollider";
			collider.type = ColliderType::EDGE;
			collider.vertice0 = Vect2d{ 0, -10 };
			collider.vertice1 = Vect2d{ -6., 5 };
			collider.position = Vect2d{ 2.1, 4 };
			collider.isSensor = false;
			body.appendCollider(collider);
			th_colliders.push_back(collider);
			//we will iterate out of colliders container bounds 
			//to check if default colliders are correctly returned in this case
			th_colliders.push_back(defaultCollider);
			th_colliders.push_back(defaultCollider);
			//test the size of the colliders container
			Assert::IsTrue(body.getNumberOfColliders() == 4);
			//test each collider 
			for (int i = 0; i < 6; i++)
			{
				SpacePartitionCollider colliderAtIdx = body.getColliderAt(i);
				Assert::IsTrue(th_colliders[i].type == colliderAtIdx.type);
				Assert::IsTrue(th_colliders[i].tag == colliderAtIdx.tag);
				Assert::IsTrue(th_colliders[i].isSensor == colliderAtIdx.isSensor);
				Assert::IsTrue(th_colliders[i].position == colliderAtIdx.position);
				Assert::IsTrue(th_colliders[i].boxWidth == colliderAtIdx.boxWidth);
				Assert::IsTrue(th_colliders[i].boxHeight == colliderAtIdx.boxHeight);
				Assert::IsTrue(th_colliders[i].vertice0 == colliderAtIdx.vertice0);
				Assert::IsTrue(th_colliders[i].vertice1 == colliderAtIdx.vertice1);
			}

			// test non negative properties
			SpacePartitionBody body2;
			collider.boxWidth = -5.3;
			collider.boxHeight = -12.6;
			body2.appendCollider(collider);
			Assert::IsTrue(0 == body2.getColliderAt(0).boxWidth);
			Assert::IsTrue(0 == body2.getColliderAt(0).boxHeight);
			// test body displacement and collider position in global frame
			collider.boxWidth = 6;
			collider.boxHeight = 6;
			collider.position = Vect2d{ 0, 0 };
			body2.appendCollider(collider);
			Assert::IsTrue(Vect2d{ 0, 0 } == body2.getColliderAt(1).position);
			body2.setPosition({ 5, 6 });
			Assert::IsFalse(Vect2d{ 5, 6 } == body2.getColliderAt(1).position);
			Assert::IsTrue(Vect2d{ 5, 6 } == body2.getColliderAt_globalFrame(1).position);
			collider.position = { 1, -2.5 };
			body2.appendCollider(collider);
			Assert::IsFalse(Vect2d{ 5, 6 } == body2.getColliderAt(1).position);
			Assert::IsTrue(Vect2d{ 5, 6 } == body2.getColliderAt_globalFrame(1).position);
			Assert::IsTrue(Vect2d{ 1, -2.5 } == body2.getColliderAt(2).position);
			Assert::IsTrue(Vect2d{ 6, 3.5 } == body2.getColliderAt_globalFrame(2).position);
		}

		TEST_METHOD(BodyAndColliderAxisAlignedBoundingBox)
		{
			// Test Body and Colliders AABB
			SpacePartitionBody body;
			std::vector< AABB > th_cAABB; // expected colliders AABB
			//first collider : default
			SpacePartitionCollider collider;
			body.appendCollider(collider);
			th_cAABB.push_back({ 0, 0, 0, 0 });
			Assert::IsTrue(AABB{ 0, 0, 0, 0 } == body.getAABB());
			//2st collider : Collider with no shape
			collider.tag = "BoxCollider";
			collider.type = ColliderType::BOX;
			collider.boxWidth = 0.;
			collider.boxHeight = 0.;
			collider.position = Vect2d{ 3, 2 };
			body.appendCollider(collider);
			th_cAABB.push_back({ 3, 2, 0, 0 });
			Assert::IsTrue(AABB{ 0, 0, 0, 0 } == body.getAABB()); // no impact on Body AABB
			//3rd collider : Box
			collider.tag = "BoxCollider";
			collider.type = ColliderType::BOX;
			collider.boxWidth = 5.3;
			collider.boxHeight = 12.6;
			collider.position = Vect2d{ -1, -2.6 };
			body.appendCollider(collider);
			th_cAABB.push_back({ -1, -2.6, 5.3, 12.6 });
			Assert::IsTrue(AABB{ -1, -2.6, 5.3, 12.6 } == body.getAABB()); // the Body AABB is initialized with the collider AABB
			//4th collider : Edge
			collider.tag = "EdgeCollider";
			collider.type = ColliderType::EDGE;
			collider.vertice0 = Vect2d{ 0, -10 };
			collider.vertice1 = Vect2d{ -6., 5 };
			collider.position = Vect2d{ 2.1, -5 };
			body.appendCollider(collider);
			th_cAABB.push_back({ -0.9, -7.5, 6, 15 });
			Assert::IsTrue(AABB{ -0.9, -5.65, 6,  18.7} == body.getAABB()); // the Body AABB is updated with the collider AABB
			//5th collider : Edge identical as previous one
			collider.tag = "EdgeCollider";
			collider.type = ColliderType::EDGE;
			collider.vertice0 = Vect2d{ 0, -10 };
			collider.vertice1 = Vect2d{ -6., 5 };
			collider.position = Vect2d{ 2.1, -5 };
			body.appendCollider(collider);
			th_cAABB.push_back({ -0.9, -7.5, 6, 15 });
			Assert::IsTrue(AABB{ -0.9, -5.65, 6,  18.7 } == body.getAABB()); // no impact on Body AABB
			//6th collider : Circle already inside body
			collider.tag = "BoxCollider";
			collider.type = ColliderType::BOX;
			collider.boxHeight = 2;
			collider.boxWidth = 2;
			collider.position = Vect2d{ 1, 2 };
			body.appendCollider(collider);
			th_cAABB.push_back({ 1, 2, 2, 2 });
			Assert::IsTrue(AABB{ -0.9, -5.65, 6,  18.7 } == body.getAABB()); // no impact on Body AABB
			//7th collider : Box
			collider.tag = "BoxCollider";
			collider.type = ColliderType::BOX;
			collider.boxHeight = 10;
			collider.boxWidth = 10;
			collider.position = Vect2d{ -0.9, -7. };
			body.appendCollider(collider);
			th_cAABB.push_back({ -0.9, -7, 10, 10 });
			Assert::IsTrue(AABB{ -0.9, -5.65, 10,  18.7 } == body.getAABB()); // the Body AABB is updated with the collider AABB
			//8rd collider : Collider with negative properties
			collider.tag = "BoxCollider";
			collider.type = ColliderType::BOX;
			collider.boxWidth = -100;
			collider.boxHeight = -100;
			collider.position = Vect2d{ 100, 200 };
			body.appendCollider(collider);
			th_cAABB.push_back({ 100, 200, 0, 0 });
			Assert::IsTrue(AABB{ -0.9, -5.65, 10,  18.7 } == body.getAABB()); // no impact on Body AABB
			//9rd collider : Collider with one negative property
			collider.tag = "BoxCollider";
			collider.type = ColliderType::BOX;
			collider.boxWidth = -100;
			collider.boxHeight = 100;
			collider.position = Vect2d{ -100, 0 };
			body.appendCollider(collider);
			th_cAABB.push_back({ -100, 0, 0, 100 });
			Assert::IsTrue(AABB{ -47.95, 0, 104.1, 100 } == body.getAABB()); // the Body AABB is updated with the collider AABB
			for (int i = 0; i < th_cAABB.size(); i++)
			{
				Assert::IsTrue(th_cAABB[i] == body.getColliderAt(i).getAABB());
			}
		}

		TEST_METHOD(AxisAlignedBoundingBoxIntersection)
		{
			Assert::IsTrue(AABB{ 0, 0, 5, 5 }.intersect(AABB{ 0, 0, 5, 5 }));
			Assert::IsTrue(AABB{ 5, 0, 5, 5 }.intersect(AABB{ 0, 0, 5, 5 }));
			Assert::IsTrue(AABB{ 0, -5, 5, 5 }.intersect(AABB{ 0, 0, 5, 5 }));
			Assert::IsTrue(AABB{ -5, -5, 5, 5 }.intersect(AABB{ 0, 0, 5, 5 }));
			Assert::IsFalse(AABB{ -5.01, -5, 5, 5 }.intersect(AABB{ 0, 0, 5, 5 }));
			Assert::IsFalse(AABB{ 10.5, 22.3, 4, 10 }.intersect(AABB{ 20, -6, 10, 5 }));
			Assert::IsFalse(AABB{ 10.5, 22.3, 4, 10 }.intersect(AABB{ 20, -6, 10, 50 }));
			Assert::IsFalse(AABB{ 10.5, 22.3, 0, 6.599 }.intersect(AABB{ 20, -6, 50, 50 }));
			Assert::IsTrue(AABB{ 10.5, 22.3, 0, 6.6 }.intersect(AABB{ 20, -6, 50, 50 }));
			Assert::IsFalse(AABB{ -10.5, 22.3, 10.999, 6.599 }.intersect(AABB{ 20, -6, 50, 50 }));
			Assert::IsFalse(AABB{ -10.5, 22.3, 10.99, 6.6 }.intersect(AABB{ 20, -6, 50, 50 }));
			Assert::IsFalse(AABB{ -10.5, 22.3, 11, 6.599 }.intersect(AABB{ 20, -6, 50, 50 }));
			Assert::IsTrue(AABB{ -10.5, 22.3, 11, 6.6 }.intersect(AABB{ 20, -6, 50, 50 }));
			Assert::IsTrue(AABB{ -10.5, 22.3, 11.0001, 6.6001 }.intersect(AABB{ 20, -6, 50, 50 }));
		}

		TEST_METHOD(UserDataManagement)
		{
			SpacePartitionBody body;
			Assert::IsTrue(body.getUserData() == nullptr);
			SpacePartitionBody* body2ptrobject = new SpacePartitionBody;
			body.setUserData(body2ptrobject);
			Assert::IsTrue(body.getUserData() == body2ptrobject);
			SpacePartitionBody body3;
			body.setUserData(&body3);
			Assert::IsFalse(body.getUserData() == body2ptrobject);
			Assert::IsTrue(body.getUserData() == &body3);
			body.setUserData(nullptr);
			Assert::IsTrue(body.getUserData() == nullptr);
			delete body2ptrobject;
		}

		TEST_METHOD(BodyConfigsManagement)
		{
			SpacePartitionBody body;
			// correct initialisation of bodyconfig
			Assert::IsTrue(body.getCurrentConfig() == "default");
			Assert::IsTrue(body.getNumberOfColliders() == 0);
			SpacePartitionCollider collider;
			// append collider to "default" config
			collider.tag = "DefaultCollider1";
			collider.type = ColliderType::BOX;
			collider.boxHeight = 2;
			collider.boxWidth = 2;
			collider.position = { 0, 0 };
			body.appendCollider(collider);
			collider.tag = "DefaultCollider2";
			collider.type = ColliderType::BOX;
			collider.boxHeight = 2;
			collider.boxWidth = 2;
			collider.position = { 2, 0 };
			body.appendCollider(collider);
			Assert::IsTrue(body.getNumberOfColliders() == 2);
			// switch config
			body.setCurrentConfig("config1");
			Assert::IsFalse(body.getCurrentConfig() == "default");
			Assert::IsTrue(body.getCurrentConfig() == "config1");
			Assert::IsTrue(body.getNumberOfColliders() == 0);
			// append collider tp "config1"
			collider.tag = "Config1Collider1";
			collider.type = ColliderType::BOX;
			collider.boxHeight = 2;
			collider.boxWidth = 2;
			collider.position = { 2, 0 };
			body.appendCollider(collider);
			Assert::IsTrue(body.getNumberOfColliders() == 1);
			collider.tag = "Config1Collider2";
			collider.type = ColliderType::BOX;
			collider.boxHeight = 2;
			collider.boxWidth = 2;
			collider.position = { 2, 0 };
			body.appendCollider(collider);
			Assert::IsTrue(body.getNumberOfColliders() == 2);
			collider.tag = "Config1Collider2";
			collider.type = ColliderType::BOX;
			collider.boxHeight = 2;
			collider.boxWidth = 2;
			collider.position = { 2, 0 };
			body.appendCollider(collider);
			Assert::IsTrue(body.getNumberOfColliders() == 3);
			//swtich back to default config and test if it is ok
			// switch config
			body.setCurrentConfig("default");
			Assert::IsTrue(body.getNumberOfColliders() == 2);
			Assert::IsTrue(body.getCurrentConfig() == "default");
		}

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
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			//Switch body1 to dynamic with no collider : no intersection
			body1 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			//Switch body2 to dynamic with no collider : no intersection
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			//add collider to body2 : no intersection. The collision only considers collision between a dynamic first body and static second body
			body2.appendCollider(collider);
			//Switch body1 to dynamic with no collider and body2 to static with box collider : no intersection
			body1 = SpacePartitionBody{ { 0, 0 }, BodyType::DYNAMIC };
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::STATIC };
			body2.appendCollider(collider);
			Assert::IsTrue(AreCollisionsEqual(defaultCollision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			// append a box collider to body1 : intersection with no displacement (boxes colliders do not respond)
			Collision collisionWithNoDisplacement{ true, {0,0 } };
			body1.appendCollider(collider);
			Assert::IsTrue(AreCollisionsEqual(collisionWithNoDisplacement, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			// create a body1 with edge collider : intersection with displacement
			body2 = SpacePartitionBody{ { 0, 0 }, BodyType::STATIC };
			collider.type = ColliderType::EDGE;
			collider.vertice0 = { -3, 0 };
			collider.vertice1 = { 3, 0 };
			body2.appendCollider(collider);
			Collision collision{ true, {0, 3} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #1
			collider.vertice0 = { 1, 3 };
			collider.vertice1 = { 1, -3 };
			body2.appendCollider(collider);
			collision = { true, {0, 3} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #2
			collider.vertice0 = { -1, 3 };
			collider.vertice1 = { -1, -3 };
			body2.appendCollider(collider);
			collision = { true, {2, 0} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #3
			collider.vertice0 = { -2, -3 };
			collider.vertice1 = { -3, -2 };
			body2.appendCollider(collider);
			collision = { true, {2, 0} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #4
			collider.vertice0 = { -3, -2 };
			collider.vertice1 = { -2, -3 };
			body2.appendCollider(collider);
			collision = { true, {0.5, 0.5} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			// add several colliders to body2 and check that min displacement response is retourned #5
			collider.vertice0 = { 2.5, -3 };
			collider.vertice1 = { 3, -2.5 };
			body2.appendCollider(collider);
			collision = { true, {-0.25, 0.25} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			// add several colliders to body1 and check that the whole body is considered for min displacement calculation
			collider.type = ColliderType::BOX;
			collider.boxHeight = 6;
			collider.boxWidth = 6;
			collider.position = { 2, 0 };
			body1.appendCollider(collider);
			collision = { true, {0.5, 0.5} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			// modify body position and check that the collision detection is correctly handled #1 : update body1 position
			body1.setPosition({ -2, 0 });
			collision = { true, {-0.25, 0.25} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			// modify body position and check that the collision detection is correctly handled #2 : update body2 position
			body2.setPosition({ 0, -1.1 });
			collision = { true, {0, 1.9} };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));

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
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			body1.setCurrentConfig("config1");
			collision = { true, {0 ,1.5 } };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			body1.setCurrentConfig("config2"); // new config created with no colliders  -> no collision
			collision = { false, {0 ,0 } };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
			body1.setCurrentConfig("default");
			collision = { true, {0 ,3 } };
			Assert::IsTrue(AreCollisionsEqual(collision, SpacePartitionBody::collisionResolutionDynamicVSstatic(body1, body2)));
		}

		TEST_METHOD(AABB_globalFrame)
		{
			SpacePartitionBody body;
			SpacePartitionCollider collider;
			collider.type = ColliderType::BOX;
			collider.boxHeight = 6;
			collider.boxWidth = 6;
			body.appendCollider(collider);
			collider.position = { 5, 5 };
			body.appendCollider(collider);
			Assert::IsTrue(AABB{ 2.5, 2.5, 11, 11 } == body.getAABB_globalFrame());
			// move body
			body.setPosition({ 2.96, -6.9 });
			Assert::IsTrue(AABB{ 5.46, -4.4, 11, 11 } == body.getAABB_globalFrame());
		}
	};
}