#include <malloc.h>
#include "bitarray.h"


//simple constructor, allocates memory for bit array, also defines a value for bitsNum.
BitArray::BitArray() {
	(*this).bits = (uint8_t*)calloc(1, sizeof(uint8_t));
	bitsNum = 0;
}


//simple destructor, frees allocated by constructor memory.
BitArray::~BitArray() {
	if ((*this).bits != nullptr) {
		free((*this).bits);
	}
}


//constructor with arguments, recieves number of bits and value, then constructs a bit array with given length and value.
//(int32_t)ceil((double)bitsNum / 8) - constraints given by the nature of my bit array. I allocate it by chunks of 8 bits (uint8_t), so first 8 bits are at this->bits[0].
BitArray::BitArray(int32_t bitsNum, unsigned long value) {
	(*this).bits = (uint8_t*)calloc((int32_t)ceil((double)bitsNum / 8), sizeof(uint8_t));
	(*this).bitsNum = bitsNum;

	if (bits) {

		for (int32_t i = 0; i < sizeof(unsigned long) && i < (int32_t)ceil((double)bitsNum / 8); i++) {
			bits[i] = value & 0x00000000000000FF;
			value = value >> 8;
		}
	}

}


//constructor that creates a copy.
BitArray::BitArray(const BitArray& b) {
	(*this).bitsNum = b.bitsNum;

	(*this).bits = (uint8_t*)calloc((int32_t)ceil((double)(*this).bitsNum / 8), sizeof(uint8_t));

	if ((*this).bits) {
		for (int32_t i = 0; i < (int32_t)ceil((double)(*this).bitsNum / 8); i++) {
			(*this).bits[i] = b.bits[i];
		}
	}

}


//swaps two bit arrays
void BitArray::swap(BitArray& b) {
	uint8_t* tmpBits;
	int32_t tmpBitsNum;

	tmpBits = b.bits;
	tmpBitsNum = b.bitsNum;

	b.bits = (*this).bits;
	b.bitsNum = (*this).bitsNum;

	(*this).bits = tmpBits;
	(*this).bitsNum = tmpBitsNum;
}


//works similar to a copying constructor, but frees previously allocated memory
BitArray& BitArray::operator=(const BitArray& b) {
	uint8_t* newBits;

	newBits = (uint8_t*)calloc((int32_t)ceil((double)b.bitsNum / 8), sizeof(uint8_t));

	for (int32_t i = 0; i < (int32_t)ceil((double)b.bitsNum / 8); i++) {
		newBits[i] = b.bits[i];
	}

	if ((*this).bits) {
		free((*this).bits);
	}
	(*this).bits = newBits;
	(*this).bitsNum = b.bitsNum;

	return *this;
}


//Resizes bit array by creating allocating new memory and freeing previously allocated block and fills any new bits with value
void BitArray::resize(int32_t bitsNum, bool value) {
	if (bitsNum != (*this).bitsNum) {
		uint8_t* newBits;

		newBits = (uint8_t*)calloc((int32_t)ceil((double)bitsNum / 8), sizeof(uint8_t));

		if (bitsNum > (*this).bitsNum) {

			for (int32_t i = 0; i < (int32_t)ceil((double)((*this).bitsNum) / 8); i++) {
				newBits[i] = (*this).bits[i];
			}

			for (int32_t i = (*this).bitsNum; i < bitsNum; i++) {
				newBits[(int32_t)ceil((double)(i + 1) / 8) - 1] += value << (i % 8);
			}

		}
		else if (bitsNum < (*this).bitsNum) {

			if (newBits) {
				for (int32_t i = 0; i < (int32_t)ceil((double)bitsNum / 8); i++) {
					newBits[i] = (*this).bits[i];
				}

			}
		}

		if ((*this).bits != nullptr) {
			free((*this).bits);
		}

		(*this).bits = newBits;

		(*this).bitsNum = bitsNum;
	}
	
}


//Just makes bit array empty by setting its' size to zero.
void BitArray::clear() {
	(*this).bitsNum = 0;
	if ((*this).bits != nullptr) {
		free((*this).bits);
	}
	(*this).bits = nullptr;
}


//Adds bits to the start of bit array, but when you output bit array it will be in the back, so it's called pushBack.
void BitArray::pushBack(bool bit) {
	uint8_t* newBits;
	
	(*this).bitsNum++;

	newBits = (uint8_t*)calloc((int32_t)ceil((double)bitsNum / 8), sizeof(uint8_t));
	
	for (int32_t i = 0; i < (int32_t)ceil((double)((*this).bitsNum - 1) / 8); i++) {
		newBits[i] = (*this).bits[i];
	}

	for (int32_t i = ((int32_t)ceil((double)((*this).bitsNum) / 8) - 1); i > 0; i--) {
		newBits[i] <<= 1;
		newBits[i] += (newBits[i - 1] & 0x80) / 128;
	}

	newBits[0] <<= 1;
	newBits[0] += (uint8_t)bit;

	if ((*this).bits != nullptr) {
		free((*this).bits);
	}

	(*this).bits = newBits;
}


//Performs bitwise "and" on equally sized bit arrays, if two arrays are not equal in size throws an error.
BitArray& BitArray::operator&=(const BitArray& b) {
	if ((*this).bitsNum == b.bitsNum) {
		for (int32_t i = 0; i < (int32_t)ceil((double)(*this).bitsNum / 8); i++) {
			(*this).bits[i] &= b.bits[i];
		}
	}
	else {
		throw std::range_error("Bit arrays must have equal sizes to perform bitwise operations.");
	}

	return (*this);
}


//Performs bitwise "or" on equally sized bit arrays, if two arrays are not equal in size throws an error.
BitArray& BitArray::operator|=(const BitArray& b) {
	if ((*this).bitsNum == b.bitsNum) {
		for (int32_t i = 0; i < (int32_t)ceil((double)(*this).bitsNum / 8); i++) {
			(*this).bits[i] |= b.bits[i];
		}
	}
	else {
		throw std::range_error("Bit arrays must have equal sizes to perform bitwise operations.");
	}

	return (*this);
}


//Performs bitwise "xor" on equally sized bit arrays, if two arrays are not equal in size throws an error.
BitArray& BitArray::operator^=(const BitArray& b) {
	if ((*this).bitsNum == b.bitsNum) {
		for (int32_t i = 0; i < (int32_t)ceil((double)(*this).bitsNum / 8); i++) {
			(*this).bits[i] ^= b.bits[i];
		}
	}
	else {
		throw std::range_error("Bit arrays must have equal sizes to perform bitwise operations.");
	}

	return (*this);
}


//Performs bitwise left shift for n bits by shifting bytes n times and transfering bits from one byte to other, if necessary.
BitArray& BitArray::operator<<=(int32_t n) {
	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = (int32_t)ceil((double)(*this).bitsNum / 8) - 1; j >= 0; j--) {
			if (((*this).bits[j] & 0x80) == 0x80 && j < (int32_t)ceil((double)(*this).bitsNum / 8) - 1) {
				(*this).bits[j + 1] += 1;
			}
			(*this).bits[j] <<= 1;
		}
	}

	return (*this);
}


//Performs bitwise left shift for n bits by shifting bytes n times and transfering bits from one byte to other, if necessary.
BitArray& BitArray::operator>>=(int32_t n) {
	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < (int32_t)ceil((double)(*this).bitsNum / 8); j++) {
			if (((*this).bits[j] & 0x01) == 0x01 && j > 0) {
				(*this).bits[j - 1] += 128;
			}
			(*this).bits[j] >>= 1;
		}
	}

	return (*this);
}


//Does the same thing as <<= operator, but returns a new bit array.
BitArray BitArray::operator<<(int32_t n) const {
	BitArray tmp(*this);

	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = (int32_t)ceil((double)tmp.bitsNum / 8) - 1; j >= 0; j--) {
			if ((tmp.bits[j] & 0x80) == 0x80 && j < (int32_t)ceil((double)tmp.bitsNum / 8) - 1) {
				tmp.bits[j + 1] += 1;
			}
			tmp.bits[j] <<= 1;
		}
	}

	return tmp;
}


//Does the same thing as >>= operator, but returns a new bit array.
BitArray BitArray::operator>>(int32_t n) const {
	BitArray tmp(*this);

	for (int32_t i = 0; i < n; i++) {
		for (int32_t j = 0; j < (int32_t)ceil((double)tmp.bitsNum / 8); j++) {
			if ((tmp.bits[j] & 0x01) == 0x01 && j > 0) {
				tmp.bits[j - 1] += 128;
			}
			tmp.bits[j] >>= 1;
		}
	}

	return tmp;
}


//Sets bit n to value val with bitwise operations.
BitArray& BitArray::set(int32_t n, bool val) {
	if (val) {
		if (((*this).bits[n / 8] & (1 << (n % 8))) == 0) {
			(*this).bits[n / 8] |= 1 << (n % 8);
		}
	}
	else {
		if (((*this).bits[n / 8] & (1 << (n % 8))) == (1 << (n % 8))) {
			(*this).bits[n / 8] ^= 1 << (n % 8);
		}
	}

	return (*this);
}


//Sets all bits to 1.
BitArray& BitArray::set() {
	for (int32_t i = 0; i < (int32_t)ceil((double)(*this).bitsNum / 8); i++) {
		(*this).bits[i] = 255;
	}

	return (*this);
}


//Sets bit n to zero.
BitArray& BitArray::reset(int32_t n) {
	if (((*this).bits[n / 8] & (1 << (n % 8))) == (1 << (n % 8))) {
		(*this).bits[n / 8] ^= 1 << (n % 8);
	}

	return (*this);
}


//Sets all bits to zero.
BitArray& BitArray::reset() {
	for (int32_t i = 0; i < (int32_t)ceil((double)(*this).bitsNum / 8); i++) {
		(*this).bits[i] = 0;
	}

	return (*this);
}


//Checks if there are any bit with value 1 in the bit array.
bool BitArray::any() const {
	for (int32_t i = 0; i < (int32_t)ceil((double)(*this).bitsNum / 8); i++) {
		if ((*this).bits[i] > 0) {
			return true;
		}
	}

	return false;
}


//Checks if there are none bit with value 1 in the bit array.
bool BitArray::none() const {
	return !(*this).any();
}


//Performs bitwise not on bit array by performing bitwise not on entire bytes.
BitArray BitArray::operator~() const {
	BitArray tmp(*this);

	for (int32_t i = 0; i < (int32_t)ceil((double)tmp.bitsNum / 8); i++) {
		tmp.bits[i] = ~tmp.bits[i];
	}

	return tmp;
}


//Counts all 1's in the bit array by getting value of individual byte.
int32_t BitArray::count() const {
	int32_t res;

	res = 0;

	for (int32_t i = 0; i < (*this).bitsNum; i++) {
		if ((*this).bits[(int32_t)ceil((double)(i + 1) / 8) - 1] & (1 << (i % 8))) {
			res++;
		}
	}

	return res;
};


//Returns a value of bit with index i.
bool BitArray::operator[](int32_t i) const {
	if (i >= (*this).bitsNum || i < 0) {
		throw std::out_of_range("index is out of range.");
	}

	return (bool)((*this).bits[(int32_t)ceil((double)(i + 1) / 8) - 1] & (1 << (i % 8)));
}


//Returns size of the bit array.
int32_t BitArray::size() const {
	return (*this).bitsNum;
}


//Check if bit array has been cleaned.
bool BitArray::empty() const {
	return (*this).bits == nullptr;
}


//Converts bit array to string while also miroring it to make it readable in the console.
std::string BitArray::toString() const {
	std::string res;

	for (int32_t i = (*this).bitsNum - 1; i >= 0; i--) {
		res.push_back(((*this).bits[(int32_t)ceil((double)(i + 1) / 8) - 1] & (1 << (i % 8))) ? '1' : '0');
	}

	return res;
}



//Checks if values of two given bit arrays are equal (two diffrent bit arrays MUST have diffrent pointers to their bits because of the way destructor works).
bool operator==(const BitArray& a, const BitArray& b) {
	if (a.size() != b.size()) {
		return false;
	}
	for (int32_t i = 0; i < a.size(); i++) {
		if (a[i] != b[i]) {
			return false;
		}
	}

	return true;
}


//Obviously does the opposite thing of the equals operator.
bool operator!=(const BitArray& a, const BitArray& b) {
	return !(a == b);
}


//Performs bitwise and on two bit arrays and returns a new one by creating a copy of the first and then performing a bitwise and with the second.
BitArray operator&(const BitArray& b1, const BitArray& b2) {
	BitArray res(b1);
	res &= b2;

	return res;
}


//Performs bitwise or on two bit arrays and returns a new one by creating a copy of the first and then performing a bitwise and with the second.
BitArray operator|(const BitArray& b1, const BitArray& b2) {
	BitArray res(b1);
	res |= b2;

	return res;
}


//Performs bitwise xor on two bit arrays and returns a new one by creating a copy of the first and then performing a bitwise and with the second.
BitArray operator^(const BitArray& b1, const BitArray& b2) {
	BitArray res(b1);
	res ^= b2;

	return res;
}