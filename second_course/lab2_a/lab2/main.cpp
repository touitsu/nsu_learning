#include <iostream>
#include <cstdint>
#include "bitarray.h"


int32_t main() {
	BitArray bit1 = BitArray(8, 255);
	BitArray bit2 = BitArray(16, 21845);

	std::cout << bit2.toString() << std::endl;

	std::cout << bit2[9] << std::endl;
	
	bit2[15] = 1;

	std::cout << bit2[9] << std::endl;

	std::cout << bit2.toString() << std::endl;

	return 0;
}