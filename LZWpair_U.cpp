#include "LZWpair_U.h"

LZWpair_U::LZWpair_U(const unsigned short& number_p, const char alfabet_letter, const bool _isLnull) : l(alfabet_letter), isLnull(_isLnull)
{
	if (number_p > MAX_DICTIONARY_SIZE)
	{
		throw std::logic_error("p can't be bigger than " + MAX_DICTIONARY_SIZE);
	}
	
	p = number_p;
}

char LZWpair_U::getChar() const
{
	return l;
}

unsigned short LZWpair_U::getP() const
{
	return p;
}

bool LZWpair_U::is_L_null() const
{
	return isLnull;
}
