#include "UserInterface.h"
#include <regex>

std::string UserInterface::inputString(const std::string& textBeforeInput) const
{
    if (textBeforeInput != "")
    {
        std::cout << textBeforeInput << ' ';
    }
    std::string user_input;
    std::cin >> user_input;

    //Отчистка cin буфера
    while (std::cin.rdbuf()->in_avail()) { std::cin.get(); }
    std::cin.clear();
    return user_input;
}

char UserInterface::inputOneSymbol(const std::string& textBeforeInput) const
{
    std::string user_input;
    bool is_correct = false;
    char res = '\0';
    do
    {
        user_input = inputString(textBeforeInput);

        if (user_input.size() == 1)
        {
            res = user_input[0];
            is_correct = true;
        }
        else
        {
            std::cout << "Input has to be only 1 char long!\n";
        }

    } while (!is_correct);

    return res;
}



int UserInterface::inputInt(const std::string& textBeforeInput) const
{
    std::string user_input;
    bool is_correct = false;
    int res;
    do
    {
        user_input = inputString(textBeforeInput);

        try
        {
            res = std::stoi(user_input);
            is_correct = true;
        }
        catch (...)
        {
            // stoi выкидывает исключение, если не может преобразовать в int
            std::cout << "Value is incorrect! try again:\n";
        }
    } while (!is_correct);

    return res;
}

char UserInterface::inputLetter(const std::string& textBeforeInput) const
{
    char c;
    bool is_correct = false;
    do
    {
        c = inputOneSymbol(textBeforeInput);
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            is_correct = true;
        }
        else
        {
            std::cout << "Input has to be a char A-Z or a-z!\n";
        }
    } while (!is_correct);
    
    return c;
}
