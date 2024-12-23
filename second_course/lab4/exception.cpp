#include "exception.h"


Exception::Exception(std::string classSender, std::string type, std::string what) : classSender(classSender), type(type), what(what) {

}


std::string Exception::info() {
	std::string res;

	res += "An error occured in class: ";
	res += classSender;
	res += '\n';
	res += "Error type: ";
	res += type;
	res += '\n';
	res += "Error message: ";
	res += what;
	res += '\n';

	return res;
}