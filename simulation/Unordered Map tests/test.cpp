#include "../Unordered Map/Unordered_Map.h"
#include <gtest/gtest.h>

Unordered_Map<int, int> foo(Unordered_Map<int, int> map) {
	Unordered_Map<int, int> move = map;
	return move;
}

TEST(Constructors, constructors) {
	Unordered_Map<int, int> map;
	for (int i = 0; i < 5; ++ i) {
		map.insert(i, i + 1);
	}

	Unordered_Map<int, int> copy_map = map; //copy c_tor
	ASSERT_TRUE(copy_map == map);

	Unordered_Map<int, int> copy_equal;		//copy operator=
	copy_equal = copy_map;
	ASSERT_TRUE(copy_equal == map);

	Unordered_Map<int, int> move = foo(map);	//move c_tor
	ASSERT_TRUE(move == map);

	Unordered_Map<int, int> move_equal;			//move operator=
	move_equal = foo(map);
	ASSERT_TRUE(move_equal == map);
}

TEST(Parameters, getters_and_setters) {
	Unordered_Map<double, double> map;
	ASSERT_TRUE(map.size() == 0);
	ASSERT_TRUE(map.empty());
	ASSERT_TRUE(map.buckets_number() == 10);

	map.insert(3, 3);
	ASSERT_TRUE(map.size() == 1);
	ASSERT_TRUE(!map.empty());

	map.erase(3);
	ASSERT_TRUE(map.size() == 0);
	ASSERT_TRUE(map.empty());

	map.reserve(15);
	ASSERT_TRUE(map.buckets_number() == 15);

	ASSERT_ANY_THROW(map.reserve(10));

	map.max_load_factor(0.8);
	ASSERT_NEAR(map.max_load_factor(), 0.8, 0.01);
	ASSERT_ANY_THROW(map.max_load_factor(-1));
}

TEST(Methods, insertions_and_erase) {
	Unordered_Map<int, int> map;
	map[-1] = 10;

	for (int i = 0; i < 20; ++i) {
		map.insert(i, i);
	}

	ASSERT_TRUE(map.size() == 21);

	ASSERT_TRUE(map.contains(-1));
	ASSERT_TRUE(map.contains(19));
	ASSERT_FALSE(map.contains(100));

	int x = 15, y = 16;
	ASSERT_ANY_THROW( map.insert(std::make_pair(x, y)) );

	for (int i = 0; i < 10; ++i) {
		map.erase(i);
	}
	ASSERT_TRUE(map.size() == 11);

	x = 100, y = 16;
	map.insert(std::make_pair(x, y));
	ASSERT_TRUE(map[100] = 16);

	map.clear();
	ASSERT_TRUE(map.size() == 0);
	ASSERT_TRUE(map.empty());

	map.insert(-1, -1);
	ASSERT_TRUE(map.size() == 1 && map[-1] == -1);
}

TEST(Methods, Find) {
	Unordered_Map<int, int> map;
	map[-1] = 10;

	for (int i = 0; i < 20; ++i) {
		map.insert(i, i);
	}

	const Unordered_Map<int, int> const_map = map;

	//contains()
	ASSERT_TRUE(map.contains(-1));
	ASSERT_TRUE(map.contains(19));
	ASSERT_FALSE(map.contains(100));

	//find()
	ASSERT_TRUE(map.find(100) == map.end());
	auto it = map.find(5);
	ASSERT_TRUE(it->first == 5 && it->second == 5);

	//const find()
	Unordered_Map<int, int>::const_iterator const_it;
	ASSERT_TRUE(const_map.find(100) == const_map.end());
	const_it = const_map.find(5);
	ASSERT_TRUE(const_it->first == 5 && const_it->second == 5);

	//at()
	ASSERT_TRUE(map.at(8) == 8);
	ASSERT_ANY_THROW(map.at(100));

	//const at()
	ASSERT_TRUE(const_map.at(8) == 8);
	ASSERT_ANY_THROW(const_map.at(100));

	//operator[]
	ASSERT_TRUE(map[5] == 5);
	ASSERT_TRUE(map[-1] == 10);
}

TEST(Other, Iterators) {
	Unordered_Map<std::string, int> map;
	ASSERT_TRUE(map.begin() == map.end());
	ASSERT_TRUE(map.cbegin() == map.cend());

	map.insert("key", -1);


	std::hash<const char*> hasher;
	const char* key = "key";
	std::cout << hasher("key") << std::endl;
	std::cout << hasher("key") << std::endl;
	std::cout << hasher("key") << std::endl;
	std::cout << hasher(key) << std::endl;


	for (auto it = map.begin(); it != map.end(); ++it) {
		ASSERT_STREQ((*it).first.c_str(), "key");
		ASSERT_TRUE((*it).second == -1);
		it->second = -2;
		ASSERT_TRUE(map.at("key") == -2);
	}

	const Unordered_Map<std::string, int> const_map = map;

	for (auto it = map.begin(); it != map.end(); ++it) {
		ASSERT_STREQ(it->first.c_str(), "key");
		ASSERT_TRUE((*it).second == -2);
	}
}

