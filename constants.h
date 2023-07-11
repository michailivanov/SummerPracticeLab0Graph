#pragma once
#include <string>
#include <cmath>
#include <vector>
#include "alfabet.h"

#include <mpi.h> // mpi

const alfabet ALFABET;
const std::string FILE_PATH = "files/file.txt";
const std::string COPY_PATH = "files/copy.txt";
const std::string BLOCKSIZES_PATH = "files/block_sizes.txt";


const int FILE_SIZE = 10000;
const unsigned char MAX_RLE_NUM_OF_BITS_FOR_L = 14;
const unsigned short MAX_RLE_L = std::pow(2, MAX_RLE_NUM_OF_BITS_FOR_L) - 1;

const unsigned short MAX_DICTIONARY_SIZE = 4096;
