#pragma once
#include <string>
#include <vector>
#include "LZWpair.h"
#include <iostream>
class LZWencoder
{	
protected:
	// index of the biggest word in a dictionary (return 0 if there is no such word)
	static int FDfuncLZW(const std::vector<std::string>& D, const std::string& text, int k);

	// Encode a text using LZW algorithm
	static std::vector<LZWpair> encodeLZW(const std::string& text);

	// Decode an encoded Text using LZW algorithm
	static std::string decodeLZW(const std::vector<LZWpair>& encoded);

public:
	static std::string encode(const std::string& text);
	static std::string decode(const std::string& text);
};

