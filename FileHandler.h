#pragma once
#include "constants.h"
#include <iostream>
#include <fstream>
#include <exception>
#include "LZWencoder.h"
#include "LZWencoder_U.h"

class FileHandler
{
	static std::string getFileText(const std::string& file_path);
	static void pushTextToFile(const std::string& file_path, const std::string& text, const int& rank);
	static void pushSize_tToFile(const std::string& file_path, const size_t& val, const int& rank);
	static std::vector<size_t> popSize_tFromFile(const std::string& file_path);
public:
	static int getFileSize(const std::string& file_path);
	static void generateFileInfo(const std::string& file_path, const int& size);

	// Return true if the file contains valid info (corresponding to alfabet)
	static bool isFileValid(const std::string& file_path);

	// Return true if "file.txt" and "encoded.txt" are equal
	static bool areFilesEqual(const std::string& file1_path, const std::string& file2_path);
	static void copyFile(const std::string& source_path, const std::string& destination_path);

	static void encodeFileUsingLZW(const std::string& file_path, const bool U);
	static void decodeFileUsingLZW(const std::string& file_path, const bool U);
};

