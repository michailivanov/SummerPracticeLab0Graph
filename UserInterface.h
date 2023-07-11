#pragma once
#include <iostream>
#include <string>
#include "constants.h"

class UserInterface
{
	char inputOneSymbol(const std::string& textBeforeInput = "") const;

public:
	std::string inputString(const std::string& textBeforeInput = "") const;
	int inputInt(const std::string& textBeforeInput = "") const;
	char inputLetter(const std::string& textBeforeInput = "") const;
};

