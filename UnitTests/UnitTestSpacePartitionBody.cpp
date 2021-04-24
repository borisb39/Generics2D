#include "CppUnitTest.h"

#include "../SpacePartitionBody.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Generics;

namespace UnitTests
{
	TEST_CLASS(UnitTestSpacePartitionBody)
	{
	public:

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
	};
}