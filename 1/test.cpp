#define _USE_MATH_DEFINES
#include <gtest/gtest.h>
#include "../Lab2/Agnesi.h"
#include <tchar.h>
#include <math.h>
TEST(AgnesiConstructor, InitConstructor) {
	double err = 0.001;
	Agnesi A1(3);
	ASSERT_EQ(3, A1.GetParam());
	Agnesi A2(sqrt(2));
	ASSERT_NEAR(1.4142, A2.GetParam(), err);
	Agnesi A3(100.1234);
	ASSERT_EQ(100.1234, A3.GetParam());
	ASSERT_ANY_THROW(Agnesi A4(0));
}

TEST(AgnesiMethods, NegativeA) {
	double err = 0.001;
	Agnesi A(-3.4);
	ASSERT_NEAR(M_PI * 3.4 * 3.4, A.FindSquare(), err);
	ASSERT_NEAR(pow(M_PI, 2) * pow(3.4, 3) / 2, A.FindVolume(), err);
	auto points = A.FindInfl();
	ASSERT_NEAR(-3.4/sqrt(3), points.first.x, err);
	ASSERT_NEAR(3.4/sqrt(3), points.second.x, err);
	ASSERT_NEAR(3 * (-3.4) / 4, points.first.y, err);
	char str[30];
	sprintf_s(str, 30, "%.3f / (x^2 + %.3f)\0", pow(-3.4, 3), pow(-3.4, 2));
	ASSERT_STREQ(str, A.Eq());
	A.SetParam(4);
	ASSERT_EQ(4, A.GetParam());
}

TEST(AgnesiMethods, PositiveA) {
	double err = 0.001;
	Agnesi A(10);
	ASSERT_NEAR(M_PI * 10 * 10, A.FindSquare(), err);
	ASSERT_NEAR(pow(M_PI, 2) * pow(10, 3) / 2, A.FindVolume(), err);
	auto points = A.FindInfl();
	ASSERT_NEAR(-10.0 / sqrt(3), points.first.x, err);
	ASSERT_NEAR(10.0 / sqrt(3), points.second.x, err);
	ASSERT_NEAR(3 * 10.0 / 4, points.first.y, err);
	char str[30];
	sprintf_s(str, 30, "%.3f / (x^2 + %.3f)\0", pow(10, 3), pow(10, 2));
	ASSERT_STREQ(str, A.Eq());
	A.SetParam(M_PI);
	ASSERT_EQ(M_PI, A.GetParam());
}

TEST(AgnesiExceptions, ConstuctorEXceptions) {
	ASSERT_ANY_THROW(Agnesi A(0));
	Agnesi A1(3);
	ASSERT_ANY_THROW(A1.SetParam(0));
}


int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}