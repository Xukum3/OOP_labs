#include "gtest/gtest.h"
#include "../Library/Vector.h"
#include <sstream>

constexpr double err = 0.000001;

namespace Static {
	TEST(Constructor_S, Constructor) {
		Vector v1;
		EXPECT_EQ(v1.getCurSize(), 0);
		Vector v2(3);
		EXPECT_EQ(v2.getCurSize(), 1);
		EXPECT_NEAR(v2[0], 3, err);
		Vector v3(4, 1);
		EXPECT_EQ(v3.getCurSize(), 4);
		for (int i = 0; i < 4; i++) {
			EXPECT_NEAR(v3[i], 1, err);
		}
		double arr4[4] = { 1,2,3,4 };
		Vector v4(4, arr4);
		EXPECT_EQ(v4.getCurSize(), 4);
		for (int i = 0; i < 4; i++) {
			EXPECT_NEAR(v4[i], arr4[i], err);
		}
		double arr5[5] = { 1,2,3,4,5 };
		Vector v5 = { 1,2,3,4,5 };
		EXPECT_EQ(v5.getCurSize(), 5);
		for (int i = 0; i < 5; i++) {
			EXPECT_NEAR(v5[i], arr5[i], err);
		}
		EXPECT_THROW({
			Vector v6(v5.getMaxSize() + 1, 0.0);
			}, std::exception);
	}

	TEST(Methods_S, Setters) {
		Vector v1 = { 1,2,3,4,5 };
		EXPECT_TRUE(v1.setItem(3, 10));
		double val;
		EXPECT_TRUE(v1.getItem(3, val));
		EXPECT_NEAR(val, 10, err);
		EXPECT_FALSE(v1.setItem(5, 10));
		EXPECT_FALSE(v1.getItem(5, val));
		EXPECT_EQ(v1.getMaxSize(), 20);
		EXPECT_EQ(v1.getCurSize(), 5);
		double arr[] = { 1,2,3,10,5 };
		for (int i = 0; i < 5; i++) {
			EXPECT_NEAR(v1[i], arr[i], err);
		}
		v1[3] = 5;
		EXPECT_NEAR(v1[3], 5, err);
		
		std::string str = "3 1.0 3.4 5.6\n";
		std::istringstream istr(str);
		std::ostringstream ostr;
		double arr1[] = { 1.0,3.4,5.6 };
		v1.setArray(istr);
		EXPECT_EQ(v1.getCurSize(), 3);
		for (int i = 0; i < 3; i++) {
			EXPECT_NEAR(v1[i], arr1[i], err);
		}
		v1.getArray(ostr);
		std::string str1 = ostr.str();
		std::string expectStr = "3 1 3.4 5.6 \n";
		EXPECT_EQ(expectStr, str1);

		Vector v2 = { 1,2,3,4,5 };
		double arr2[] = { 1,2,3,4,5 };
		std::string str2 = "3 1.0 q 3.4 5.6";
		std::istringstream istr2(str2);
		v2.setArray(istr2);
		EXPECT_TRUE(istr2.fail());
		EXPECT_EQ(v2.getCurSize(), 5);
		for (int i = 0; i < 5; i++) {
			EXPECT_NEAR(v2[i], arr2[i], err);
		}
	}
	TEST(Methods_S, Math) {
		Vector v1 = { 1,2,3,6 };
		Vector v2 = { 1,4,6,3,5 };
		Vector new_v;
		double array_add[] = { 2,6,9,9 };
		double array_sub[] = { 0,-2,-3,3 };
		v1.add(v2, new_v);
		EXPECT_EQ(new_v.getCurSize(), 4);
		for (int i = 0; i < 4; i++) {
			EXPECT_NEAR(new_v[i], array_add[i], err);
		}
		v1.sub(v2, new_v);
		EXPECT_EQ(new_v.getCurSize(), 4);
		for (int i = 0; i < 4; i++) {
			EXPECT_NEAR(new_v[i], array_sub[i], err);
		}
		double val = v1.mult(v2);
		EXPECT_EQ(val, 45);
	}
	TEST(Methods_S, OtherMethods) {
		Vector v1 = { 1,3,-4,99 };
		double norma;
		EXPECT_TRUE(v1.normOfVector(norma));
		EXPECT_NEAR(norma, 99, err);
		v1 = {};
		EXPECT_FALSE(v1.normOfVector(norma));
		v1 = { 1,2,3 };
		v1.append(5);
		EXPECT_EQ(v1.getCurSize(), 4);
		EXPECT_NEAR(v1[3], 5, err);
		v1.append(99);
		EXPECT_EQ(v1.getCurSize(), 5);
		EXPECT_NEAR(v1[4], 99, err);
		Vector v2(v1.getMaxSize(), 0.);
		EXPECT_FALSE(v2.append(5));
	}
	TEST(Operators_S, Math) {
		Vector v1 = { 1,2,3,6 };
		Vector v2 = { 1,4,6,3,5 };
		double array_add[] = { 2,6,9,9 };
		double array_sub[] = { 0,-2,-3,3 };
		double array_mult[] = { 1,9,27,45 };
		EXPECT_EQ((v1 + v2).getCurSize(), 4);
		for (int i = 0; i < 4; i++) {
			EXPECT_NEAR((v1 + v2)[i], array_add[i], err);
		}
		EXPECT_EQ((v1 - v2).getCurSize(), 4);
		for (int i = 0; i < 4; i++) {
			EXPECT_NEAR((v1 - v2)[i], array_sub[i], err);
		}
		EXPECT_EQ((v1 * v2), 45);
	}
}

namespace Dynamic {
	TEST(Constructor_D, InitConstructor) {
		Vector v1;
		EXPECT_EQ(v1.getCurSize(), 0);
		Vector v2(3);
		EXPECT_EQ(v2.getCurSize(), 1);
		Vector v3(4, 1);
		EXPECT_EQ(v3.getCurSize(), 4);
		for (int i = 0; i < 4; i++) {
			EXPECT_NEAR(v3[i], 1, err);
		}
		double arr[] = { 1,2,3,4 };
		Vector v4 = { 4, arr };
		EXPECT_EQ(v4.getCurSize(), 4);
		for (int i = 0; i < 4; i++) {
			EXPECT_NEAR(v4[i], arr[i], err);
		}
		Vector v5 = { 1,2,3,4 };
		v5 = { 1,2,3,4 };
		double array[] = { 1,2,3,4 };
		EXPECT_EQ(v5.getCurSize(), 4);
		for (int i = 0; i < v5.getCurSize(); i++) {
			EXPECT_NEAR(v5[i], array[i], err);
		}
		EXPECT_THROW({
			Vector v6(-1, 0.);
			}, std::exception);
	}
	TEST(Constructor_D, CopyConstructor) {
		Vector v1 = { 1,2,3,4 };
		Vector v2 = v1;
		EXPECT_EQ(v1.getCurSize(), v2.getCurSize());
		for (int i = 0; i < v1.getCurSize(); i++) {
			EXPECT_NEAR(v1[i], v2[i], err);
		}
	}
	TEST(Methods_D, Setters){
		Vector v1 = { 1,2,3,4,5 };
		EXPECT_TRUE(v1.setItem(3, 10));
		double val;
		EXPECT_TRUE(v1.getItem(3, val));
		EXPECT_NEAR(val, 10, err);
		EXPECT_FALSE(v1.setItem(5, 10));
		EXPECT_FALSE(v1.getItem(5, val));
		EXPECT_EQ(v1.getCurSize(), 5);
		double arr[] = { 1,2,3,10,5 };
		for (int i = 0; i < 5; i++) {
			EXPECT_NEAR(v1[i], arr[i], err);
		}
		v1[3] = 5;
		EXPECT_NEAR(v1[3], 5, err);

		std::string str = "3 1.0 3.4 5.6\n";
		std::istringstream istr(str);
		std::ostringstream ostr;
		double arr1[] = { 1.0,3.4,5.6 };
		v1.setArray(istr);
		EXPECT_EQ(v1.getCurSize(), 3);
		for (int i = 0; i < 3; i++) {
			EXPECT_NEAR(v1[i], arr1[i], err);
		}
		v1.getArray(ostr);
		std::string str1 = ostr.str();
		std::string expectStr = "3 1 3.4 5.6 \n";
		EXPECT_EQ(expectStr, str1);

		Vector v2 = { 1,2,3,4,5 };
		double arr2[] = { 1,2,3,4,5 };
		std::string str2 = "3 1.0 q 3.4 5.6";
		std::istringstream istr2(str2);
		v2.setArray(istr2);
		EXPECT_TRUE(istr2.fail());
		EXPECT_EQ(v2.getCurSize(), 5);
		for (int i = 0; i < 5; i++) {
			EXPECT_NEAR(v2[i], arr2[i], err);
		}
	}
	TEST(Methods_D, OtherMethods) {
		Vector v1 = { 1,4,-88,0 };
		double val;
		EXPECT_TRUE(v1.normOfVector(val));
		EXPECT_NEAR(val, 88, err);
		v1 = {};
		EXPECT_FALSE(v1.normOfVector(val));
		v1 = { -1 };
		v1.normOfVector(val);
		EXPECT_NEAR(val, 1, err);
		v1 = { 1,2,3,4 };
		v1.append(5);
		EXPECT_EQ(v1.getCurSize(), 5);
		EXPECT_NEAR(v1[4], 5, err);
	}
	
	TEST(Operators_D, Math) {
		Vector v1 = { 1,2,3,6 };
		Vector v2 = { 1,4,6,3,5 };
		double array_add[] = { 2,6,9,9 };
		double array_sub[] = { 0,-2,-3,3 };
		EXPECT_EQ((v1 + v2).getCurSize(), 4);
		for (int i = 0; i < 4; i++) {
			EXPECT_NEAR((v1 + v2)[i], array_add[i], err);
		}
		EXPECT_EQ((v1 - v2).getCurSize(), 4);
		for (int i = 0; i < 4; i++) {
			EXPECT_NEAR((v1 - v2)[i], array_sub[i], err);
		}
		EXPECT_EQ((v1 * v2), 45);
	}
	TEST(Operators_D, Bool) {
		Vector v1 = { 1,2,3,4 };
		Vector v2 = { 1,2,3,4 };
		Vector v3 = { 1,2,3,4,5 };
		Vector v4 = { 1,1,2,3 };
		EXPECT_TRUE(v1 == v2);
		EXPECT_FALSE(v1 == v3);
		EXPECT_FALSE(v1 == v4);
		EXPECT_FALSE(v1 != v2);
		EXPECT_TRUE(v1 != v3);
		EXPECT_TRUE(v1 != v4);
	}
	TEST(Operators_D, IO) {
		Vector v1;
		std::string str = "3 1.0 3.4 5.6\n";
		std::istringstream istr(str);
		std::ostringstream ostr;
		double arr[] = { 1.0,3.4,5.6 };
		istr >> v1;
		EXPECT_EQ(v1.getCurSize(), 3);
		for (int i = 0; i < 3; i++) {
			EXPECT_NEAR(v1[i], arr[i], err);
		}
		ostr << v1;
		std::string str1 = ostr.str();
		std::string expectStr = "3 1 3.4 5.6 \n";
		EXPECT_EQ(expectStr, str1);
	}
	TEST(Operators_D, IO_Fail) {
		Vector v1 = { 1,2,3,4,5 };
		double arr[] = { 1,2,3,4,5 };
		std::string str = "3 1.0 q 3.4 5.6";
		std::istringstream istr(str);
		istr >> v1; 
		EXPECT_TRUE(istr.fail());
		EXPECT_EQ(v1.getCurSize(), 5);
		for (int i = 0; i < 5; i++) {
			EXPECT_NEAR(v1[i], arr[i], err);
		}
	}
	TEST(Cycle, Cycle) {
		Vector vec = { 1,1,1,1,1 };
		for (double& e : vec) {
			e = 4;
		}
		for (double e : vec) {
			std::cout << e << ' ';
		}
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}