#include <gtest/gtest.h>
#include "../SetDynamic/DynSet.h"
#include <tchar.h>
#include <string>


TEST(SetConstructor, InitConstructor) {
	EXPECT_TRUE((Set(5) == Set{ 1, 2, 3, 4, 5 }));
	EXPECT_TRUE(Set() == Set{});
	EXPECT_TRUE((Set{ 1, 2, 3 } == Set{ 1, 3, 2, 2, 2, 1, 3 }));
	int arr[] = { 2, 2, 3, 5, 4, 3, 1 };
	EXPECT_TRUE(Set (7, arr) == Set(5));
}


TEST(SetConstructor, CopyConstructor) {
	Set a(5);
	Set b = a; //copyconstructor
	Set c;
	c = b;	//operator =

	EXPECT_TRUE(a == b);
	EXPECT_TRUE(c == b);

	Set d = a | c; //move constructor
	Set e;
	e = a | c;    //operator = with &&
	EXPECT_TRUE(d == a);
	EXPECT_TRUE(c == a);	
}

TEST(SetExceptions, Exceptions) {
	EXPECT_ANY_THROW(Set(-1));
	Set a;
	std::string str = "not_int\n";
	std::istringstream st(str);
	st >> a;
	EXPECT_TRUE(st.fail());
}

TEST(SetMethods, Insert_Isexist) {
	Set a;
	a.insert(2);
	EXPECT_TRUE(a == Set{2});

	a.insert(2);
	a.insert(-1);
	a.insert(100);
	a.insert(-1);

	std::string str = "-5\n";
	std::istringstream st(str);
	st >> a;

	EXPECT_TRUE((a == Set{ -5, -1, 2, 100 }));

	EXPECT_TRUE(a.isexist(2));
	EXPECT_TRUE(a.isexist(100));
	EXPECT_FALSE(a.isexist(333));
}


TEST(SetMethods, SetOperations) {
	Set a = { 1, 2, 3, 4, 5, 6 };
	Set b = { -10, -1, 2, 5, 100, 150 };
	Set c;

	EXPECT_TRUE((a - 5) == Set{ 6 });
	EXPECT_TRUE((7 - a) == Set{ 7 });
	EXPECT_TRUE((7 - a) == Set{ 7 });
	EXPECT_TRUE((a - Set{ 4, 5, 6 }) == 3);

	EXPECT_TRUE(((a | b) == Set{ -10, -1, 1, 2, 3, 4, 5, 6, 100, 150 }));
	EXPECT_TRUE(((b | a) == Set{ -10, -1, 1, 2, 3, 4, 5, 6, 100, 150 }));

	EXPECT_TRUE(((a & b) == Set{ 2, 5 }));
	EXPECT_TRUE(((b & a) == Set{ 2, 5 }));

	EXPECT_TRUE(((a - b) == Set{ 1, 3, 4, 6 }));
	EXPECT_TRUE(((b - a) == Set{ -10, -1, 100, 150 }));


	//with empty set
	EXPECT_TRUE((c - a) == Set());
	EXPECT_TRUE((c & a) == Set());
	EXPECT_TRUE((c | a) == a);	
	
	EXPECT_TRUE((a - c) == a);
	EXPECT_TRUE((a & c) == Set());
	EXPECT_TRUE((c | a) == a);
}


int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}