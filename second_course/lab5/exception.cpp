#include "exception.h"


Exception::Exception(std::string func, std::string what) {
	this->func = func;
	this->what = what;
}


Exception::~Exception() {
	
}


const std::string Exception::info() noexcept {
	std::string res;

	res += "Standard exception occured in ";
	res += this->func;
	res += '.\n';
	res += "Description: ";
	res += this->what;
	res += '\n';

	return res;
}


RuntimeException::RuntimeException(std::string func, std::string what) : Exception(func, what) {

}


RuntimeException::~RuntimeException() {

}


const std::string RuntimeException::info() noexcept {
	std::string res;

	res += "Runtime exception occured in ";
	res += this->func;
	res += '.\n';
	res += "Description: ";
	res += this->what;
	res += '\n';

	return res;
}


InvalidArgumentException::InvalidArgumentException(std::string func, std::string what) : Exception(func, what) {

}


InvalidArgumentException::~InvalidArgumentException() {

}


const std::string InvalidArgumentException::info() noexcept {
	std::string res;

	res += "Invalid argument exception occured in ";
	res += this->func;
	res += '.\n';
	res += "Description: ";
	res += this->what;
	res += '\n';

	return res;
}