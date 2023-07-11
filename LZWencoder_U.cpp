#include "LZWencoder_U.h"

int LZWencoder_U::FDfuncLZW(const std::vector<std::string>& D, const std::string& text, int k)
{
    int l = 0;
    int p = 0;
    for (int i = 0; i < D.size(); i++)
    {
        int m = D.at(i).size();

        // form a word based on the text from k to k + m - 1
        std::string word("");
        for (int j = k; j < k + m; j++)
        {
            if (j >= text.size())
            {
                break;
            }

            word += text.at(j);
        }

        if (D.at(i) == word && m > l)
        {
            p = i;
            l = m;
        }
    }

    return p;
}

std::vector<LZWpair_U> LZWencoder_U::encodeLZW(const std::string& text)
{
    std::vector<LZWpair_U> res;

    // initial state of a dictionary
    std::vector<std::string> D;
    D.push_back("");

    size_t alert75 = text.size() * 3 / 4;
    size_t alert50 = text.size() / 2;
    size_t alert25 = text.size() / 4;

    for (size_t k = 0; k < text.size();) // k - num of a letter in the text
    {
        // р — index of the found word in the dictionary
        size_t p = FDfuncLZW(D, text, k); // TODO

        // l — length of the found word in the dictionary
        size_t l = D.at(p).size();

        // num of a word from the dictionary + additional letter
        if (k + l < text.size())
        {
            res.push_back(LZWpair_U(p, text.at(k + l), false));
            D.push_back(D.at(p) + text.at(k + l));
        }
        else // if it's the end of the text
        {
            res.push_back(LZWpair_U(p, '\0', true));
            D.push_back(D.at(p));
        }

        // move forward in text
        k += l + 1;

        // logging
        if (k > alert25)
        {
            std::cout << "25% ";
            alert25 = -1;
        }
        else if (k > alert50)
        {
            std::cout << "50% ";
            alert50 = -1;
        }
        else if (k > alert75)
        {
            std::cout << "75% \n";
            alert75 = -1;
        }
    }
    return res;
}

std::string LZWencoder_U::decodeLZW(const std::vector<LZWpair_U>& encoded)
{
    std::string res;

    // initial state of a dictionary
    std::vector<std::string> D;
    D.push_back("");

    for (int k = 0; k < encoded.size(); k++)
    {
        int p = encoded.at(k).getP(); // word index in the dictionary

        char q = encoded.at(k).getChar(); // additional letter (any char)

        if (encoded.at(k).is_L_null() == false)
        {
            res += D.at(p) + q; // save a word part and a letter 
            D.push_back(D.at(p) + q); // add new row in the dictionary
        }
        else
        {
            res += D.at(p); // save a word (without a letter)
            D.push_back(D.at(p)); // add new row in the dictionary
        }
    }

    return res;
}

LZWencoder_U::LZWencoder_U()
{
}
// original = 453 bytes
// encoded data elements count = 232 el
// 232 el * 20 bits = 4640 bit
// 4640 / 8 = 580 bytes

std::string LZWencoder_U::encode(const std::string& text)
{
    std::vector<LZWpair_U> encoded_data = encodeLZW(text);

    // calculate the total number of bits needed to store the data
    int total_bits = encoded_data.size() * 20;

    // Why 20?
    /* for every element:
          12 bit - encode p (num in dictionary) (0 - 4096)
          8 bit - encode l (additional char) (0 - 25)
          12 + 8 = 20 bit 
    */

    // calculate the number of bytes needed to store the data
    int total_bytes = (total_bits + 8 - 1) / 8; // + 8 - 1 round up div

    // create a string to store the encoded data
    std::string res(total_bytes, 0); // fill with 0-es

    // current position in the encoded data
    int pos = 0;
    // current bit in the current byte
    int bit = 7;

    // iterate over each Data element
    for (const LZWpair_U& d : encoded_data)
    {
        // encode the first element
        for (int i = 12 - 1; i >= 0; --i)
        {
            //(d.getP() >> i) & 1) - get element's bit in i position (<-)
            res[pos] |= ((d.getP() >> i) & 1) << bit;
            if (--bit < 0)
            {
                bit = 7;
                ++pos;
            }
        }

        // encode the second element
        for (int i = 8 - 1; i >= 0; --i) {
            res[pos] |= ((d.getChar() >> i) & 1) << bit;
            if (--bit < 0)
            {
                bit = 7;
                ++pos;
            }
        }
    }

    return res;
}

std::string LZWencoder_U::decode(const std::string& text)
{
    // current position in the encoded data
    int pos = 0;
    // current bit in the current byte
    int bit = 7;

    // get the number of bytes
    int total_bytes = text.size();

    // calculate the number of elements
    int num_elements = (total_bytes * 8) / 20;

    std::vector<LZWpair_U> encoded_data;

    // iterate over each Data element
    for (int i = 0; i < num_elements; ++i) {
        // decode the first element
        unsigned short p = 0;
        for (int j = 0; j < 12; ++j) {
            //(text[pos] >> bit) & 1) - get bit from 'bit' possition in pos byte
            p |= ((text[pos] >> bit) & 1) << (12 - 1 - j);
            if (--bit < 0)
            {
                bit = 7;
                ++pos;
            }
        }

        // decode the second element
        char l = 0;
        for (int j = 0; j < 8; ++j) {
            l |= ((text[pos] >> bit) & 1) << (8 - 1 - j);
            if (--bit < 0)
            {
                bit = 7;
                ++pos;
            }
        }
        try
        {
            if (i == num_elements - 1 && l == '\0')
            {
                encoded_data.push_back(LZWpair_U(p, '\0', true));
            }
            else
            {
                encoded_data.push_back(LZWpair_U(p, l, false));
            }
        }
        catch (const std::exception& ex)
        {
            std::cout << ex.what();
            throw std::logic_error("Perhaps, this text isn't encoded using LZW!");
        }
    }

    return decodeLZW(encoded_data);
}
