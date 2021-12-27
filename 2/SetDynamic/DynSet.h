#ifndef D_SET
#define D_SET

#include <iostream>
#include <initializer_list>
#include <cstring>

class Set {
	size_t capacity;
	int* array;

	bool isexist(int key, unsigned int& pos) const;
	void push_back(int val);

public:
	Set() : capacity(0), array(nullptr) { }
	Set(size_t size);
	Set(size_t size, const int* arr);
	Set(const std::initializer_list<int>& list);

	~Set() { delete[] array; };

	Set(const Set& other);
	Set& operator= (const Set& other);

	Set(Set&& other) noexcept;
	Set& operator= (Set&& other) noexcept;

	bool insert(int val);
	void show(std::ostream& out = std::cout) const;
	bool isexist(int val) const;
	void set(size_t size, std::istream& in = std::cin);

	friend Set operator| (const Set& first, const Set& other);
	friend Set operator- (const Set& first, const Set& other);
	friend Set operator& (const Set& first, const Set& other);

	friend bool operator== (const Set& first, const Set& second);
	friend bool operator!= (const Set& first, const Set& second);


	friend std::ostream& operator<<(std::ostream& out, const Set& set);
	friend std::istream& operator>>(std::istream& out, Set& set);

};

#endif //D_SET
