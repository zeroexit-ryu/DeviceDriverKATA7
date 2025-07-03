#pragma once
#include <exception>

class ReadFailException : public std::exception {
public:
	char const* what() const override {
		return "read fail occured";
	}
};

class WriteFailException : public std::exception {
public:
	char const* what() const override {
		return "write fail occured";
	}
};
	