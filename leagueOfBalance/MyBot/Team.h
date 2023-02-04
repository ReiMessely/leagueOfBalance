#pragma once
#include <optional>
#include <vector>
#include <algorithm>
#include <sstream>
#include "Player.h"
#include "RawFormsAnswer.h"

typedef unsigned int UINT;

class unequal_player_divide
{
public:
    const char* what() const
    {
        return "Can not equally divide players in teams";
    }
};

struct Team final
{
    size_t maxTeamSize{ 5 };
    std::vector<Player*> players{};

    inline void AddPlayer(Player* p)
    {
        if (players.size() < maxTeamSize)
        {
            players.push_back(p);
            return;
        }
        auto emptyIt = std::find(players.begin(), players.end(), nullptr);
        if (emptyIt == players.end())
        {
            throw "";
        }
        players.emplace(emptyIt, p);
    }

    inline bool NeedsPlayers() const
    {
        if (players.size() < maxTeamSize)
        {
            return true;
        }
        for (size_t i{ 0 }; i < maxTeamSize; ++i)
        {
            if (players.at(i) == nullptr)
            {
                return true;
            }
        }
        return false;
    }

    inline std::optional<UINT> AvarageMMR() const
    {
        auto isPlayerNullptr = [](Player* p) {return (p == nullptr);};
        if (std::all_of(players.begin(), players.end(), isPlayerNullptr))
        {
            return std::nullopt;
        }
        UINT mmr{ 0 };
        UINT count{ 0 };

        auto accumalate = [&](Player* p) 
        {
            if (p)
            {
                mmr += Rank::GetMMR(p->rank);
                ++count;
            }
        };
        std::for_each(players.begin(), players.end(), accumalate);

        return mmr / count;
    }

    inline std::string Format(const std::string& prefix = "```\n", const std::string& suffix = "```\n", const std::string& bulletPoint = "- ") const
    {
        std::stringstream message;
        message << prefix;
        for (auto player : players)
        {
            message << bulletPoint << player->formsAnswer->discord << "\n";
        }
        message << suffix;

        return message.str();
    }

    static std::vector<Team> SplitIntoTeams(std::vector<Player>& players, size_t playersPerTeam = 5)
    {
        if (!((players.size() % playersPerTeam) == 0))
        {
            throw unequal_player_divide{};
        }

        std::vector<Team> teams;
        for (size_t i{ 0 }; i < (players.size() / playersPerTeam); ++i)
        {
            teams.emplace_back(Team{ playersPerTeam });
        }
        // Sort from lowest rank to highest rank
        std::sort(players.begin(), players.end(), [](const Player& p1, const Player& p2) { return Rank::GetMMR(p1.rank) < Rank::GetMMR(p2.rank); });
        size_t backIdx{ players.size() - 1 };
        size_t frontIdx{ 0 };
        bool anyOfTeamsNeedPlayers = true;

        auto isT2BetterthanT1 = [](const Team& t1, const Team& t2) {return t1.AvarageMMR() < t2.AvarageMMR();};

        while (anyOfTeamsNeedPlayers)
        {
            // Sort from lowest rank to highest rank
            std::sort(teams.begin(), teams.end(), isT2BetterthanT1);
            for (auto& team : teams)
            {
                if (team.NeedsPlayers())
                {
                    team.AddPlayer(&players.at(backIdx));
                    --backIdx;
                }
            }
            // Sort from highest rank to lowest rank
            std::sort(teams.rbegin(), teams.rend(), isT2BetterthanT1);
            for (auto& team : teams)
            {
                if (team.NeedsPlayers())
                {
                    team.AddPlayer(&players.at(frontIdx));
                    ++frontIdx;
                }
            }
            anyOfTeamsNeedPlayers = std::any_of(teams.begin(), teams.end(), [](const Team& t) {return t.NeedsPlayers();});
        }

        return teams;
    }
};