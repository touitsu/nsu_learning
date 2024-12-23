#pragma once

#include <string>


class Exception {
protected:
	std::string func;
	std::string what;

public:
	Exception(std::string func, std::string what);

	virtual ~Exception();

	virtual const std::string info() noexcept;
};


class RuntimeException : public virtual Exception {
public:
	RuntimeException(std::string func, std::string what);

	~RuntimeException();

	const std::string info() noexcept override;
};


class InvalidArgumentException : public virtual Exception {
public:
	InvalidArgumentException(std::string func, std::string what);

	~InvalidArgumentException();

	const std::string info() noexcept override;
};