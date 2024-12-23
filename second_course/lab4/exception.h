#include <string>


class Exception {
private:
	std::string classSender;
	std::string type;
	std::string what;

public:
	Exception(std::string classSender, std::string type, std::string what);

	std::string info();
};