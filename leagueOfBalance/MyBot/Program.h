#pragma once
#include <string>

struct ProgramArguments
{
	bool valid{ false };
	std::string formsAnswersPath{};
	std::string teamNamesPath{};
	size_t amountOfPlayersPerTeam{};
};

class Program final
{
public:
	Program() = delete;
	~Program() = default;

	Program(Program&& p) = delete;
	Program(const Program& p) = delete;
	Program& operator=(Program&& p) = delete;
	Program& operator=(const Program& p) = delete;

	// =============================================

	static ProgramArguments GetArguments();

	// Enters an infite loop where the user can only enter Y, y, N or n. Returns true when the answer is Y or y
	static bool GetYesOrNo();

private:
	static bool CheckIfFileExists(const std::string& path);
};

