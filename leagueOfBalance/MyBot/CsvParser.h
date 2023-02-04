#pragma once
#ifndef CSV_PARSER
#define CSV_PARSER
#include <vector>
#include <string>

struct PreferedRoles final
{
    bool top{ false };
    bool jngl{ false };
    bool mid{ false };
    bool adc{ false };
    bool support{ false };
    bool fill{ false };

    size_t Amount() const
    {
        size_t amount{ 0 };
        amount += static_cast<int>(top);
        amount += static_cast<int>(jngl);
        amount += static_cast<int>(mid);
        amount += static_cast<int>(adc);
        amount += static_cast<int>(support);
        amount += static_cast<int>(fill);
        return amount;
    }
};

class CsvParser final
{
public:
    /*
    * \param fileName: Name of the file
    * \return vector of the rows
    */
	static std::vector<std::string> SplitIntoRows(const std::string& fileName);
    /*
    * \param row: row containing a forms answer
    * \return string of all roles in the forms answer
    */
    static std::string GetRolesString(const std::string& row);
    /*
    * REMOVES ROLES FROM ANSWER
    * \param row: row containing a forms answer
    * \return string of all roles in the forms answer
    */
	static std::string ExtractRolesString(std::string& row);
    /*
    * \param str: string containing roles (Top, Jngl, Mid, Adc, Support, Fill)
    * \return struct containing flags of the answers
    */
    static PreferedRoles StringToRoles(const std::string& str);
    /*
    * \param row: row containing a forms answer
    * \return vector of each element of the answer
    */
    static std::vector<std::string> SplitIntoElements(const std::string& row);
};

#endif // !CSV_PARSER

