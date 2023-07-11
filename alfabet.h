#pragma once
#include "alfabet_const.h"
#include <fstream>

class alfabet
{
	std::string m_alfabet;
public:
	alfabet();

	// get char of an alfabet
	char at(const unsigned char i) const; 

	// get an index of char in alfabet (in my case: 0 - 24) or if not found: -1 (size_t)
	size_t index(const char ch) const;

	bool has(const char ch) const;

	size_t size() const;
};

