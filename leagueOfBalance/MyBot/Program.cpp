#include "Program.h"
#include <iostream>
#include <fstream>

ProgramArguments Program::GetArguments()
{
	ProgramArguments result;

    using namespace std;

    string defFormAnswersPath = "formsAnswers.csv";
    string defTeamNamesPath = "Skinlines.csv";
    int defAmountOfPlayersPerTeam = 5;

    string formsAnswersPath;
    string amountOfPlayersPerTeamAnswer;
    string teamNamesPath;
    int amountOfPlayerPerTeam;
    cout << "------ BALANCING SOFTWARE ------\n\n";
    cout << "Enter '.' when default is sufficient.\n";

    // SOURCE FILE
    //
    cout << "- Form answers path: (default: " << defFormAnswersPath << ")\n";
    cin >> formsAnswersPath;
    if (formsAnswersPath == ".") formsAnswersPath = defFormAnswersPath;

    if (!CheckIfFileExists(formsAnswersPath))
    {
        cout << "File does not exist! Make sure the extension of the file is correct\n";
        result.valid = false;
        return result;
    }

    // PLAYERS PER TEAM
    //
    cout << "- Amount of players per team: (default: "<< defAmountOfPlayersPerTeam << ")\n";
    cin >> amountOfPlayersPerTeamAnswer;
    if (amountOfPlayersPerTeamAnswer == ".")
    {
        amountOfPlayerPerTeam = defAmountOfPlayersPerTeam;
    }
    else
    {
        try
        {
            amountOfPlayerPerTeam = std::stoi(amountOfPlayersPerTeamAnswer);
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << '\n';
            result.valid = false;
            return result;
        }
        if (amountOfPlayerPerTeam <= 0 || amountOfPlayerPerTeam > 5)
        {
            cout << "Invalid amount of players per team!\n";
            result.valid = false;
            return result;
        }
    }

    // TEAM NAMES
    //
    cout << "- Filepath to team names: (default: " << defTeamNamesPath << ")\n";
    cin >> teamNamesPath;
    if (teamNamesPath == ".") teamNamesPath = defTeamNamesPath;

    if (!CheckIfFileExists(teamNamesPath))
    {
        cout << "File does not exist! Make sure the extension of the file is correct\n";
        result.valid = false;
        return result;
    }
    

    cout << "------ ------ ------\n";

    result.valid = true;
    result.amountOfPlayersPerTeam = amountOfPlayerPerTeam;
    result.formsAnswersPath = formsAnswersPath;
    result.teamNamesPath = teamNamesPath;

    return result;
}

bool Program::GetYesOrNo()
{
    std::string answer{ ' ' };
    size_t failCounter{ 0 };
    while (!(answer == "Y" || answer == "y" || answer == "N" || answer == "n"))
    {
        answer.clear();
        if (failCounter > 4)
        {
            std::cout << "Please respond with Y or N\n";
        }
        std::getline(std::cin, answer);
        ++failCounter;
    }

    if (answer == "N" || answer == "n")
    {
        return false;
    }
    return true;
}

bool Program::CheckIfFileExists(const std::string& path)
{
    std::ifstream f(path.c_str());
    return !f.bad();
}
