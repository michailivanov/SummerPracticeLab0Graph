#include "LZWpair.h"

LZWpair::LZWpair(const unsigned short& number_p, const char alfabet_letter)
{
	if (number_p > MAX_DICTIONARY_SIZE)
	{
		throw std::logic_error("p can't be bigger than " + MAX_DICTIONARY_SIZE);
	}
	
	p = number_p;

	size_t index = ALFABET.index(alfabet_letter);
	if (index == -1)
	{
		throw std::logic_error("There is no such letter in the alfabet!");
	}

	l = index;
}

LZWpair::LZWpair(const unsigned short& number_p, const unsigned char alfabet_index)
{
	if (number_p > MAX_DICTIONARY_SIZE)
	{
		throw std::logic_error("p can't be bigger than " + MAX_DICTIONARY_SIZE);
	}

	p = number_p;

	if (alfabet_index >= ALFABET.size())
	{
		throw std::logic_error("(alfabet_index can't be bigger than ALFABET.size())");
	}

	l = alfabet_index;
}

char LZWpair::getChar() const
{
	return ALFABET.at(l);
}

unsigned short LZWpair::getP() const
{
	return p;
}

unsigned char LZWpair::getLindex() const
{
	return l;
}
