#include "Program.h"

#include <dpp/dpp.h>
#include <dpp/webhook.h>

#include "WebhookSimplifier.h"

#include "Tokens.h"

#include "CsvParser.h"

#include "Rank.h"
#include "Player.h"
#include "Team.h"
#include "RawFormsAnswer.h"

#include <sstream>
#include <chrono>

void DppWebhookTest();
void WarderBotTest();

int main()
{
    const ProgramArguments progArgs = Program::GetArguments();
    if (!progArgs.valid)
    {
        return 1;
    }

    auto startTime = std::chrono::steady_clock::now();

    auto teamNames = CsvParser::SplitIntoRows(progArgs.teamNamesPath);

#pragma region FileToRawFormsAnswer
    auto rows = CsvParser::SplitIntoRows(progArgs.formsAnswersPath);
    std::vector<RawFormsAnswer> answers;
    for (auto& row : rows)
    {
        RawFormsAnswer answer;
        answer.roles = CsvParser::StringToRoles(CsvParser::ExtractRolesString(row));
        auto elements = CsvParser::SplitIntoElements(row);
        answer.timeStamp = elements[0];
        answer.discord = elements[1];
        answer.opGgMain = elements[2];
        answer.opGgRegion = elements[3];

        answers.emplace_back(answer);
    }
    // Substitute for testing purposes
    {
        RawFormsAnswer sub;
        sub.timeStamp = "irr";
        sub.discord = "SubPlayer#1243";
        sub.opGgMain = "irr";
        sub.opGgRegion = "";
        answers.emplace_back(sub);
    }
#pragma endregion
    
    std::vector<Player> players;
    for (const auto& answer : answers)
    {
        players.push_back(Player{ &answer,Rank::GetRandomRankTier() });
    }

    std::vector<Team> teams;
    try
    {
        teams = Team::SplitIntoTeams(players,progArgs.amountOfPlayersPerTeam);
    }
    catch (const unequal_player_divide& e)
    {
        std::cout << e.what() << '\n';
    }

    // Print each teams avarage mmr
    //std::for_each(teams.begin(), teams.end(), [](const Team& t) {std::cout << *t.AvarageMMR() << '\n';});

    // Format discord webhook message
    std::stringstream message{"This is a test message!\n\n"};
    for (auto& team : teams)
    {
        message << "**" << "Team " << teamNames[rand() % teamNames.size()] << "**" << " - [Aprox MMR: " << std::to_string(*team.AvarageMMR()) << "]\n";
        message << team.Format();
    }

    auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> program_elapsed_seconds = endTime - startTime;
    std::cout << "Program execution was done in " << program_elapsed_seconds.count() << "s\n";

    std::cout << "\n";
    std::cout << "Webhook message preview:\n";
    std::cout << message.str() << "\n";
    std::cout << "Send to webhook? (Y/N)\n";
    if (!Program::GetYesOrNo())
    {
        return 0;
    }

    auto startTimeWebhook = std::chrono::steady_clock::now();
    // Discord webhook
    WBS_MASTER->ChangeURL(MASTER_WEBHOOK_URL);
    WBS_MASTER->ChangeMessageToEdit(MASTER_MESSAGE_ID);
    WBS_MASTER->UpdateMessage(message.str());

    auto endTimeWebhook = std::chrono::steady_clock::now();
    std::chrono::duration<double> webhook_elapsed_seconds = endTimeWebhook - startTimeWebhook;
    std::cout << "Webhook execution was done in " << webhook_elapsed_seconds.count() << "s\n";

    return 0;
}

void DppWebhookTest()
{
    dpp::cluster bot(""); // normally, you put your bot token in here. But to just run a webhook its not required

    bot.on_log(dpp::utility::cout_logger());

    dpp::webhook whTest{ MASTER_WEBHOOK_URL };

    /* send a message with this webhook */
    //bot.execute_webhook_sync(whTest, dpp::message("This is a test with D++\n Nice!"));

    auto prevMsg = bot.get_webhook_message_sync(whTest, MASTER_MESSAGE_ID);

    prevMsg.content = "Some tomfoolery";

    bot.edit_webhook_message_sync(whTest, prevMsg);
}

void WarderBotTest()
{
    dpp::cluster bot{WARDERBOT_TOKEN};

    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    auto pingCommand = [](const dpp::interaction_create_t& event)
    {
        if (event.command.get_command_name() == "ping")
        {
            event.reply("Pong!");
        }
    };

#if 0
    bot.on_interaction_create(pingCommand);

    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) 
        {
        /* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
            if (dpp::run_once<struct register_bot_commands>()) 
            {
                bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
            }
        });
#endif
#if 0
    bot.on_ready([&bot](const dpp::ready_t& event)
        {
            if (dpp::run_once<struct register_bot_commands>())
            {
                bot.global_command_delete(915018557794963467);
            }
        }
    );
#endif

    /* Start the bot */
    bot.start(false);
}