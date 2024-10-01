#pragma once

#include <cstdint>
#include <iostream>
#include <cmath>

class BitArray {
private:
	uint8_t* bits;
	int32_t bitsNum;

public:
	BitArray();
	~BitArray();

	explicit BitArray(int32_t bitsNum, uint32_t value = 0);
	BitArray(const BitArray& b);

	void swap(BitArray& b);

	BitArray& operator=(const BitArray& b);

	void resize(int32_t bitsNum, bool value = false);

	void clear();

	void pushBack(bool bit);

	BitArray& operator&=(const BitArray& b);
	BitArray& operator|=(const BitArray& b);
	BitArray& operator^=(const BitArray& b);

	BitArray& operator<<=(int32_t n);
	BitArray& operator>>=(int32_t n);
	BitArray operator<<(int32_t n) const;
	BitArray operator>>(int32_t n) const;

	BitArray& set(int32_t n, bool val = true);

	BitArray& set();

	BitArray& reset(int32_t n);

	BitArray& reset();

	bool any() const;

	bool none() const;

	BitArray operator~() const;

	int32_t count() const;

	bool operator[](int32_t i) const;

	int32_t size() const;
	bool empty() const;

	std::string toString() const;

	class Iterator : public std::iterator<std::input_iterator_tag, uint8_t, uint8_t, const uint8_t*, bool> {

	private:
		uint32_t index;
		const BitArray* bits;

	public:
		explicit Iterator(const BitArray* bitArray, uint32_t index);

		Iterator& operator++();

		Iterator operator++(int32_t);

		bool operator==(Iterator it) const;

		bool operator!=(Iterator it) const;

		reference operator*() const;
	};

	Iterator begin();

	Iterator end();

};


bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);