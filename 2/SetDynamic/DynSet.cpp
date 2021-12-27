#include "DynSet.h"
#include <limits>

bool Get_num(int& a, std::istream& in = std::cin) {
	while (1) {
		in >> a;
		if (!in.good()) {
			if (in.eof()) return false; //^Z
			std::cout << "Please repeat: ";
			in.clear();
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		return true;
	}
}


Set::Set(size_t size) {
	capacity = size;

	try {
		array = new int[capacity];
	}
	catch (std::bad_array_new_length) {
		throw std::overflow_error("Too big Set for this system");
	}

	for (unsigned int i = 0; i < capacity; i++) {
		array[i] = i + 1;
	}
}

Set::Set(size_t size, const int* arr) {
	capacity = 0;
	array = nullptr;

	if (arr != nullptr) {
		for (unsigned int i = 0; i < size; i++) {
			insert(arr[i]);
		}
	}
}

Set::Set(const std::initializer_list<int>& list) : Set() {
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		insert(*it);
	}
}

Set::Set(const Set& other) {
	capacity = other.capacity;
	if (capacity == 0) {
		array = nullptr;
		return;
	}

	array = new int[capacity];

	for (unsigned int i = 0; i < capacity; i++) {
		array[i] = other.array[i];
	}
}

Set& Set::operator= (const Set& other) {
	if (this == &other)
		return *this;

	if (other.capacity == 0) {
		capacity = 0;
		delete[] array;
		array = nullptr;
		return *this;
	}

	int* tmp = new int[other.capacity];

	delete[] array;

	capacity = other.capacity;
	for (unsigned int i = 0; i < capacity; i++) {
		tmp[i] = other.array[i];
	}

	array = tmp;

	return *this;
}


Set::Set(Set&& other) noexcept {
	capacity = other.capacity;
	array = other.array;
	other.array = nullptr;
}


Set& Set::operator=(Set&& other) noexcept {
	if (this == &other) return *this;

	delete[] array;

	capacity = other.capacity;
	array = other.array;
	other.array = nullptr;

	return *this;
}


void Set::set(size_t size, std::istream& in) {
	capacity = 0;
	delete[] array;
	array = nullptr;

	int buf;
	while (capacity < size) {
		std::cout << "Enter number [" << capacity + 1 << "] ";
		if (!Get_num(buf, in)) break;
		if (!insert(buf)) {
			std::cout << "Numbers can't repeat" << std::endl;
		}
	}
}

bool Set::insert(int val) {

	int* tmp;
	try {
		tmp = new int[capacity + 1];
	}
	catch (std::bad_array_new_length&) {
		throw std::overflow_error("Too big Set for this system");
	}
	memcpy(tmp, array, capacity * sizeof(int));
	delete[] array;
	array = tmp;

	unsigned int pos;
	if (!isexist(val, pos)) {
		int buf;
		while (pos != capacity) {
			buf = array[pos];
			array[pos++] = val;
			val = buf;
		}
		array[capacity++] = val;

		return true;
	}
	else return false; //repeat
}


void Set::push_back(int val) {
	int* tmp = new int[capacity + 1];
	memcpy(tmp, array, capacity * sizeof(int));
	delete[] array;
	array = tmp;

	array[capacity++] = val;
}


bool Set::isexist(int key, unsigned int& pos) const {
	int l = 0, r = capacity - 1;
	int m;

	while (l <= r) {
		m = (l + r) / 2;
		if (array[m] == key) {
			pos = m;
			return true;
		}
		if (array[m] < key) {
			l = m + 1;
		}
		else {
			r = m - 1;
		}
	}

	if (capacity > 0 && key > array[capacity - 1])
		pos = capacity;
	else
		pos = l;
	return false;
}


bool Set::isexist(int val) const {
	int l = 0, r = capacity - 1;
	int m;

	while (l <= r) {
		m = (l + r) / 2;
		if (array[m] == val) {
			return true;
		}
		if (array[m] < val) {
			l = m + 1;
		}
		else {
			r = m - 1;
		}
	}
	return false;
}

//--------------------------------------------------

Set operator&(const Set& first, const Set& second) {
	Set result;
	unsigned int p1 = 0, p2 = 0;
	while (p1 < first.capacity && p2 < second.capacity) {
		if (first.array[p1] == second.array[p2]) {
			result.push_back(first.array[p1]);
			p1 += 1;
			p2 += 1;
		}
		else if (first.array[p1] < second.array[p2]) {
			p1 += 1;
		}
		else p2 += 1;
	}
	return result;
}

Set operator|(const Set& first, const Set& second) {
	Set result;
	unsigned int p1 = 0, p2 = 0;
	while (p1 < first.capacity && p2 < second.capacity) {
		if (first.array[p1] <= second.array[p2]) {
			result.push_back(first.array[p1]);
			if (first.array[p1++] == second.array[p2]) p2 += 1;
		}
		else {
			result.push_back(second.array[p2++]);
		}
	}
	while (p1 < first.capacity) {
		result.push_back(first.array[p1++]);
	}

	while (p2 < second.capacity) {
		result.push_back(second.array[p2++]);
	}
	return result;
}

Set operator-(const Set& first, const Set& second) {
	Set result;
	unsigned int p1 = 0, p2 = 0;
	while (p1 < first.capacity && p2 < second.capacity) {
		if (first.array[p1] < second.array[p2]) {
			result.push_back(first.array[p1++]);
		}
		else if (first.array[p1] == second.array[p2]) {
			p1 += 1;
			p2 += 1;
		}
		else p2 += 1;
	}

	while (p1 < first.capacity) {
		result.push_back(first.array[p1++]);
	}
	return result;
}


void Set::show(std::ostream& out) const {
	if (capacity == 0) {
		out << "Empty set";
		return;
	}
	out << "{ ";
	for (unsigned int i = 0; i < capacity; i++) {
		out << array[i] << " ";
	}
	out << "}";
}

std::ostream& operator<<(std::ostream& out, const Set& set) {
	if (set.capacity == 0) {
		out << "Empty set";
		return out;
	}
	out << "{ ";
	for (unsigned int i = 0; i < set.capacity; i++) {
		out << set.array[i] << " ";
	}
	out << "}";

	return out;
}

std::istream& operator>>(std::istream& in, Set& set) {
	int buf;
	in >> buf;
	if(!in.good()){
		in.setstate(std::ios::failbit);
	}
	else{
		set.insert(buf);
	}
	return in;
}

bool operator== (const Set& first, const Set& second) {
	if (first.capacity != second.capacity) return false;

	for (unsigned int i = 0; i < first.capacity; i++) {
		if (first.array[i] != second.array[i])
			return false;
	}
	return true;
}

bool operator!= (const Set& first, const Set& second) {
	return !(first == second);
}