#include <iostream>
#include <cstdint>
#include "bitarray.h"


int32_t main() {
	BitArray bit1 = BitArray(8, 128);
	BitArray bit2 = BitArray(16, 21845);

	std::cout << bit2.toString() << std::endl;

	bit2.pushBack(1);

	std::cout << bit2.toString() << std::endl;

	bit2.pushBack(0);

	std::cout << bit2[0] << std::endl;
	std::cout << bit2.toString() << std::endl;

	return 0;
}