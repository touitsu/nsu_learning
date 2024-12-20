#include <gtest/gtest.h>
#include "AudioProcessor.h"


using namespace AudioProcessor;


TEST(AudioProcessorBytesToIntTest, intToBytesTest) {
	size_t num = 10;
	std::string res;

	res = intToBytes(num);

	ASSERT_EQ(res[0], num);
}


TEST(AudioProcessorBytesToIntTest, bytesToInt) {
	size_t res;
	std::string str(" ");

	res = bytesToInt(str, 1, 0);

	ASSERT_EQ(res, 32);
}


TEST(AudioProcessorBytesToIntTest, signedBytesToInt) {
	int32_t res;
	std::string str({ static_cast<char>(128) });

	res = signedBytesToInt(str, 1, 0);

	ASSERT_EQ(res, -128);
}


TEST(AudioProcessorBytesToIntTest, signedBytesToIntArray) {
	int16_t* res;
	std::string str({ static_cast<char>(128) });

	res = signedBytesToIntArray(str, 1);

	ASSERT_EQ(*res, -128);

	_aligned_free(res);
}