#include "CppUnitTest.h"

#include "../Generics_types.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Generics;

namespace UnitTests
{
	TEST_CLASS(UnitTestVect2d)
	{
	public:

		TEST_METHOD(TestFloatComparator)
		{
			Assert::IsFalse(Vect2d{ 1, 2000.1 } == Vect2d{ 1, 2000.2 });
			Assert::IsTrue(Vect2d{ 1, 20000.1 } == Vect2d{ 1, 20000.2 });
			Assert::IsFalse(Vect2d{ 1, 0.0001 } == Vect2d{ 1, 0.0002 });
			Assert::IsTrue(Vect2d{ 1, 0.00001 } == Vect2d{ 1, 0.00002 });
		}

		TEST_METHOD(TestAddition)
		{
			Vect2d v1{ 1, 2 };
			Assert::IsTrue((v1 += Vect2d{ -2, 7.9 }) == Vect2d{ -1.0, 9.9 });
			Assert::IsFalse((Vect2d{ 1, 2 } + Vect2d{ -4, 7.1156 }) == Vect2d{ -3, 9 });
			Assert::IsTrue((Vect2d{ 1, 2 } + Vect2d{ -4, 7.1156 }) == Vect2d{ -3, 9.1156 });
		}
		TEST_METHOD(TestSubstraction)
		{
			Vect2d v1{ 1, 2 };
			Assert::IsTrue((v1 -= Vect2d{ -2, 7.9 }) == Vect2d{ 3.0, -5.9 });
			Assert::IsFalse((Vect2d{ 1, 2 } - Vect2d{ -4, 7.1156 }) == Vect2d{ 5, -5.0 });
			Assert::IsTrue((Vect2d{ 1, 2 } - Vect2d{ -4, 7.1156 }) == Vect2d{ 5, -5.1156 });
		}

		TEST_METHOD(TestMutliplication)
		{
			Vect2d v1{ 1, 2 };
			int icoeff = 5;
			float fcoeff = -12.2;
			Assert::IsTrue((v1 *= icoeff) == Vect2d{ 5, 10 });
			Assert::IsFalse((Vect2d{ 1, 2 } *fcoeff) == Vect2d{ -12.2, 24.4 });
			Assert::IsTrue((Vect2d{ 1, 2 } *fcoeff) == Vect2d{ -12.2, -24.4 });
		}
		TEST_METHOD(TestDivision)
		{
			Vect2d v1{ 1, 2 };
			int icoeff = 2;
			float fcoeff = -0.5;
			Assert::IsTrue((v1 /= icoeff) == Vect2d{ 0.5, 1 });
			Assert::IsFalse((Vect2d{ 1, 2 } /fcoeff) == Vect2d{ -2, 4 });
			Assert::IsTrue((Vect2d{ 1, 2 } /fcoeff) == Vect2d{ -2, -4 });
		}

		TEST_METHOD(TestMultiOperation)
		{
			Vect2d v1 = Vect2d{ 3, 3 } * 2 +Vect2d{ 4, 4 };
			Assert::IsTrue(v1 == Vect2d{10, 10});
			Vect2d v2 = Vect2d{ 4, 4 } - Vect2d{ 3, 3 } /2;
			Assert::IsTrue(v2 == Vect2d{ 2.5, 2.5 });
			Vect2d v31 = Vect2d{ 3, 3 } * 2 + Vect2d{ 4, 4 } + Vect2d{ 4, 4 } -(Vect2d{ 3, 3 } / 2) * 6;
			Assert::IsTrue(v31 == Vect2d{ 5, 5 });
			Vect2d v31bis = Vect2d{ 3, 3 } *2 + Vect2d{ 4, 4 } +Vect2d{ 4, 4 } -Vect2d{ 3, 3 } / (2 * 6);
			Assert::IsTrue(v31bis == Vect2d{ 13.75, 13.75 });
		}
		TEST_METHOD(TestNorm)
		{
			Vect2d v1 = Vect2d{ 3.0, 3.0 };
			Assert::IsTrue(isEqual(v1.norm(), 4.242640687119285));
			v1 = Vect2d{ 3.123, -23.45646 };
			Assert::IsTrue(isEqual(v1.norm(), 23.663445326739723));
			v1 = Vect2d{ 3.0, -3.0 };
			Assert::IsTrue(isEqual(v1.norm(), 4.242640687119285));
			v1 = Vect2d{ -3.0, -3.0 };
			Assert::IsTrue(isEqual(v1.norm(), 4.242640687119285));
			v1 = Vect2d{ 3, -3 };
			Assert::IsTrue(isEqual(v1.norm(), 4.242640687119285));
			v1 = Vect2d{ 3, 0 };
			Assert::IsTrue(isEqual(v1.norm(), 3));
			v1 = Vect2d{ 0, 0 };
			Assert::IsTrue(isEqual(v1.norm(), 0));
			v1 = Vect2d{ 0, -3 };
			Assert::IsTrue(isEqual(v1.norm(), 3));
		}
	};
}