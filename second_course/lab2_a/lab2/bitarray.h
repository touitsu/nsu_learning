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

	explicit BitArray(int32_t bitsNum, unsigned long value = 0);
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
};

bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);