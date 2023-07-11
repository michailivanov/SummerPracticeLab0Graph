#include "alfabet.h"

alfabet::alfabet()
{
    std::ifstream alfabet_file(ALFABET_PATH);

    // Open file of an alfabet
    if(!alfabet_file)
    {
        throw std::logic_error("Can't open alfabet's file!\n");
    }

    std::getline(alfabet_file, m_alfabet);
    m_alfabet += '\0';
    alfabet_file.close();
}

char alfabet::at(const unsigned char i) const
{
    return m_alfabet.at(i);
}

size_t alfabet::index(const char ch) const
{
    return m_alfabet.find(ch);
}

bool alfabet::has(const char ch) const
{
    if (m_alfabet.find(ch) != -1)
    {
        return true;
    }
    return false;
}

size_t alfabet::size() const
{
    return m_alfabet.size();
}
