#include "CppUnitTest.h"

#include "../SpacePartitionContactListener.h"
#include "../SpacePartitionWorld.h"
#include "../SpacePartitionBody.h"
#include "../SpacePartitionCollider.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Generics;

namespace UnitTests
{
	class ContactListener : public SpacePartitionContactListener
	{
	private:
		void BeginContact(SpacePartitionContact*) override
		{
			numberBegin += 1;
		};

		void EndContact(SpacePartitionContact*) override
		{
			numberEnd += 1;
		};

	public:
		int numberBegin = 0;
		int numberEnd = 0;

	};


	TEST_CLASS(UnitTestSpacePartitionContactListener)
	{
	public:

		TEST_METHOD(worldContacts)
		{
			SpacePartitionWorld world{ 10, 10, 10, 10 };
			ContactListener listener;
			world.setContactListener(&listener);
			SpacePartitionBodyTemplate bodyTemplate;
			SpacePartitionCollider collider;
			collider.type = ColliderType::BOX;
			collider.boxHeight = 2;
			collider.boxWidth = 3;
			bodyTemplate.type = BodyType::DYNAMIC;
			bodyTemplate.appendCollider(collider);
			// create one contact between two dynamic bodies with 1 collider / body
			SpacePartitionBody* body1 = world.createBody(bodyTemplate);
			bodyTemplate.position = { 1, 0 };
			SpacePartitionBody* body2 = world.createBody(bodyTemplate);
			float dt = 0.1;
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 1);
			Assert::IsTrue(listener.numberEnd == 0);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			Assert::IsTrue(listener.getNumberOfContacts() == 1);
			// create one static body with 1 colliders
			bodyTemplate = SpacePartitionBodyTemplate{ {0, 0}, BodyType::STATIC };
			collider = SpacePartitionCollider{};
			collider.type = ColliderType::EDGE;
			collider.vertice1 = { 0, 0 };
			collider.vertice2 = { 1, 0 };
			bodyTemplate.appendCollider(collider);
			SpacePartitionBody* body3 = world.createBody(bodyTemplate);
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 3);
			Assert::IsTrue(listener.numberEnd == 0);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 2);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 2);
			collider = body3->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 2);
			Assert::IsTrue(listener.getNumberOfContacts() == 3);
			// create one static body with 2 colliders
			bodyTemplate = SpacePartitionBodyTemplate{ {0, 0}, BodyType::STATIC };
			collider = SpacePartitionCollider{};
			collider.type = ColliderType::EDGE;
			collider.vertice1 = { 2, 0 };
			collider.vertice2 = { 2, 1 };
			bodyTemplate.appendCollider(collider);
			collider.vertice1 = { 2, 1 };
			collider.vertice2 = { 2, 2 };
			bodyTemplate.appendCollider(collider);
			SpacePartitionBody* body4 = world.createBody(bodyTemplate);
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 5);
			Assert::IsTrue(listener.numberEnd == 0);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 2);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 4);
			collider = body3->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 2);
			collider = body4->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			collider = body4->getColliderAt(1);
			Assert::IsTrue(collider.contatsList.size() == 1);
			Assert::IsTrue(listener.getNumberOfContacts() == 5);
			// Move dynamic body 2 outside of contacts
			body2->setPosition({ 10, 10 });
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 5);
			Assert::IsTrue(listener.numberEnd == 4);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body3->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			collider = body4->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(1);
			Assert::IsTrue(collider.contatsList.size() == 0);
			Assert::IsTrue(listener.getNumberOfContacts() == 1);
			// Move dynamic body 1 outside of contacts
			body1->setPosition({ 10, 20 });
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 5);
			Assert::IsTrue(listener.numberEnd == 5);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body3->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(1);
			Assert::IsTrue(collider.contatsList.size() == 0);
			Assert::IsTrue(listener.getNumberOfContacts() == 0);
			// Move dynamic body 1 in contact with dynamic body 2 
			body1->setPosition({ 10, 10.9 });
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 6);
			Assert::IsTrue(listener.numberEnd == 5);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			collider = body3->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(1);
			Assert::IsTrue(collider.contatsList.size() == 0);
			Assert::IsTrue(listener.getNumberOfContacts() == 1);
			// Move dynamic body 2 in contact with static body 4 
			body2->setPosition({ 1, 1.1 });
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 8);
			Assert::IsTrue(listener.numberEnd == 6);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 2);
			collider = body3->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			collider = body4->getColliderAt(1);
			Assert::IsTrue(collider.contatsList.size() == 1);
			Assert::IsTrue(listener.getNumberOfContacts() == 2);
			// Move dynamic body 1 in contact with body 2 3 4
			body1->setPosition({ 1, 0 });
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 12);
			Assert::IsTrue(listener.numberEnd == 6);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 4);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 3);
			collider = body3->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			collider = body4->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 2);
			collider = body4->getColliderAt(1);
			Assert::IsTrue(collider.contatsList.size() == 2);
			Assert::IsTrue(listener.getNumberOfContacts() == 6);
			// Move dynamic body 1 and 2 outside of contact 
			body1->setPosition({ 10, 0 });
			body2->setPosition({ 20, 0 });
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 12);
			Assert::IsTrue(listener.numberEnd == 12);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body3->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(1);
			Assert::IsTrue(collider.contatsList.size() == 0);
			Assert::IsTrue(listener.getNumberOfContacts() == 0);
			// Move dynamic body 1 and 2 in contact
			body2->setPosition({ 10, 0 });
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 13);
			Assert::IsTrue(listener.numberEnd == 12);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 1);
			collider = body3->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(1);
			Assert::IsTrue(collider.contatsList.size() == 0);
			Assert::IsTrue(listener.getNumberOfContacts() == 1);
			// Move dynamic body 1 and 2 in contact outside of world bound (contact no registered)
			body1->setPosition({ 20, 0 });
			body2->setPosition({ 20, 0 });
			world.Step(dt);
			// test contact begin/end callback
			Assert::IsTrue(listener.numberBegin == 13);
			Assert::IsTrue(listener.numberEnd == 13);
			// check that the contact is correctly tracked
			collider = body1->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body2->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body3->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(0);
			Assert::IsTrue(collider.contatsList.size() == 0);
			collider = body4->getColliderAt(1);
			Assert::IsTrue(collider.contatsList.size() == 0);
			Assert::IsTrue(listener.getNumberOfContacts() == 0);
		}
	};
}
