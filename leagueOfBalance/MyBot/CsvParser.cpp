#include "CsvParser.h"
#include <fstream>
#include <sstream>

std::vector<std::string> CsvParser::SplitIntoRows(const std::string& fileName)
{
    std::fstream fin;

    fin.open(fileName, std::ios::in);

    std::vector<std::string> output;
    std::string line, word, temp;

    
    while (std::getline(fin, word, static_cast<char>('\n')))
    {
        output.push_back(word);
    }

    return output;
}

std::string CsvParser::GetRolesString(const std::string& row)
{
    std::string output;

    // Only works for "Mid, adc"
    size_t startIdx = row.find_first_of('"');
    size_t endIdx = row.find_last_of('"');

    size_t lastComma = row.find_last_of(',');
    if (lastComma == std::string::npos)
    {
        throw "no comma found";
    }

    // We did not find any "" which occurs when multiple roles are selected
    if (startIdx == std::string::npos || endIdx == std::string::npos)
    {
        // +1 to exclude the comma
        output = row.substr(lastComma + 1, row.length() - (lastComma + 1));
    }
    else
    {
        output = row.substr(startIdx + 1, endIdx - (startIdx + 1));
    }

    return output;
}

std::string CsvParser::ExtractRolesString(std::string& row)
{
    std::string output = GetRolesString(row);
    
    std::string::size_type size = row.find(output.c_str());
    size_t startIdx = row.find_first_of('"');
    size_t endIdx = row.find_last_of('"');
    // We did not find any "" which occurs when multiple roles are selected
    if (startIdx == std::string::npos || endIdx == std::string::npos)
    {

    }
    else
    {
        size -= 1;
        output.append(" ");
    }

    if (size != std::string::npos)
    {
        row.erase(size, output.length());
    }
    endIdx = row.find_last_of(',');
    row.erase(endIdx, row.length() - endIdx);

    return output;
}

PreferedRoles CsvParser::StringToRoles(const std::string& str)
{
    constexpr auto Str_Top = "Top";
    constexpr auto Str_Jngl = "Jngl";
    constexpr auto Str_Mid = "Mid";
    constexpr auto Str_Adc = "Adc";
    constexpr auto Str_Support = "Support";
    constexpr auto Str_Fill = "Fill";

    PreferedRoles roles;

    size_t topIdx = str.find(Str_Top);
    if (topIdx != std::string::npos)
    {
        roles.top = true;
    }
    size_t jnglIdx = str.find(Str_Jngl);
    if (jnglIdx != std::string::npos)
    {
        roles.jngl = true;
    }
    size_t midIdx = str.find(Str_Mid);
    if (midIdx != std::string::npos)
    {
        roles.mid = true;
    }
    size_t adcIdx = str.find(Str_Adc);
    if (adcIdx != std::string::npos)
    {
        roles.adc = true;
    }
    size_t suppIdx = str.find(Str_Support);
    if (suppIdx != std::string::npos)
    {
        roles.support = true;
    }
    size_t fillIdx = str.find(Str_Fill);
    if (fillIdx != std::string::npos)
    {
        roles.fill = true;
        roles.top = true;
        roles.jngl = true;
        roles.mid = true;
        roles.adc = true;
        roles.support = true;
    }

    return roles;
}

std::vector<std::string> CsvParser::SplitIntoElements(const std::string& row)
{
    std::vector<std::string> output;

    std::string rowCopy = row;
    std::string delimiter = ",";

    size_t pos = 0;
    std::string token;
    while ((pos = rowCopy.find(delimiter)) != std::string::npos) 
    {
        token = rowCopy.substr(0, pos);
        output.emplace_back(token);
        rowCopy.erase(0, pos + delimiter.length());
    }
    output.emplace_back(rowCopy);

    return output;
}
