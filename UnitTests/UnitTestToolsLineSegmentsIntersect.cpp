#include "CppUnitTest.h"

#include "../ToolsLineSegmentsIntersect.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Generics;

namespace UnitTests
{
	TEST_CLASS(ToolLineSegmentsIntersect)
	{
	public:

		TEST_METHOD(LineSegmentsIntersectIntegers)
		{
            Vect2d p1 = { 1, 1 }, q1 = { 10, 1 };
            Vect2d p2 = { 1, 2 }, q2 = { 10, 2 };
            Assert::IsFalse(doIntersect(p1, q1, p2, q2));

            p1 = { 10, 0 }, q1 = { 0, 10 };
            p2 = { 0, 0 }, q2 = { 10, 10 };
            Assert::IsTrue(doIntersect(p1, q1, p2, q2));

            p1 = { -5, -5 }, q1 = { 0, 0 };
            p2 = { 1, 1 }, q2 = { 10, 10 };
            Assert::IsFalse(doIntersect(p1, q1, p2, q2));
		}

        TEST_METHOD(LineSegmentsIntersectFloat)
        {
            //bound touching
            Vect2d p1 = { 0, 0 }, q1 = { 1.5, 2.5 };
            Vect2d p2 = { 1.5, 2.5 }, q2 = { 3.5, 4.5 };
            Assert::IsTrue(doIntersect(p1, q1, p2, q2));

            //almost bound touching
            p1 = { 0, 0 }, q1 = { 1.50001, 2.5 };
            p2 = { 1.5, 2.5 }, q2 = { 3.5, 4.5 };
            Assert::IsFalse(doIntersect(p1, q1, p2, q2));

            //colinear
            p1 = { 0, 0 }, q1 = { 3, 5 };
            p2 = { 1.5, 2.5}, q2 = { 4.5, 7.5 };
            Assert::IsTrue(doIntersect(p1, q1, p2, q2));

            //almost colinear
            p1 = { 0, 0 }, q1 = { 3, 5 };
            p2 = { 1.50001, 2.5 }, q2 = { 4.5, 7.5 };
            Assert::IsFalse(doIntersect(p1, q1, p2, q2));

            //almost colinear
            p1 = { 0, 0 }, q1 = { 3, 5 };
            p2 = { -1.50001, 2.5 }, q2 = { 4.5, 7.5 };
            Assert::IsFalse(doIntersect(p1, q1, p2, q2));

            //bound to segment touching
            p1 = { 0, 0 }, q1 = { 4.5, 7.5 };
            p2 = { 3, 5 }, q2 = { 10.99, -7.5 };
            Assert::IsTrue(doIntersect(p1, q1, p2, q2));

            //almost bound to segment touching
            p1 = { 0, 0 }, q1 = { 4.5, 7.5 };
            p2 = { 3.00001, 5 }, q2 = { 10.99, -7.5 };
            Assert::IsFalse(doIntersect(p1, q1, p2, q2));

            //almost bound to segment with intersection
            p1 = { 0, 0 }, q1 = { 4.5, 7.5 };
            p2 = { 2.99999, 5 }, q2 = { 10.99, -7.5 };
            Assert::IsTrue(doIntersect(p1, q1, p2, q2));

            //classical intersection
            p1 = { 0., 0. }, q1 = { 10, 11.5 };
            p2 = { 12.3, -6 }, q2 = { -8.3, 11.3 };
            Assert::IsTrue(doIntersect(p1, q1, p2, q2));
        }
	};
}