#pragma once
#include "constants.h"

// pair of two values p (0 - 4096) number of row in dictionary and l (0 - 24) number in alfabet\
   if l - empty char than l == 25 (in alfabet '\0') (in case if we need to save only p)
class LZWpair
{
	unsigned short p; // номер в dictionary (от 0 до 4096)
	unsigned char l; // номер элемента в alfabet (от 0 до 24)

public:
	// Constructor (check values to be in proper bounds)
	LZWpair(const unsigned short& number_p, const char alfabet_letter);
	LZWpair(const unsigned short& number_p, const unsigned char alfabet_index);

	// Return char (according to the alfabet)
	char getChar() const;

	// Return number of row in dictionary (0 - 4096)
	unsigned short getP() const;

	// Return Index of l in alfabet (0 - 24)
	unsigned char getLindex() const;
};
