#pragma once
#include "constants.h"

// pair of two values p (0 - 4096) number of row in dictionary and l any char element
class LZWpair_U
{
	unsigned short p; // num in dictionary (0 - 4096)
	char l; // any element

	bool isLnull; // true if need to ignore l value

public:
	LZWpair_U(const unsigned short& number_p, const char letter, const bool _isLnull);

	// Return char
	char getChar() const;

	// Return number of row in dictionary (0 - 4096)
	unsigned short getP() const;

	bool is_L_null() const;
};
