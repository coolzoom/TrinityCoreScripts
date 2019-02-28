/*
<--------------------------------------------------------------------------->
- Developer(s): WiiZZy
- Complete: 100%
- ScriptName: 'World chat'
- Comment: Fully tested
<--------------------------------------------------------------------------->
*/

#include "ScriptPCH.h"
#include "Config.h"
#include "Log.h"
#include "Chat.h"
#include "Common.h"
#include "World.h"
#include "WorldSession.h"
#include "RBAC.h"
#include <unordered_map>

/* VERSION */
float ver = 1.0f;

/* Colors */
std::string WORLD_CHAT_ALLIANCE_BLUE = "|cff3399FF";
std::string WORLD_CHAT_HORDE_RED = "|cffCC0000";
std::string WORLD_CHAT_WHITE = "|cffFFFFFF";
std::string WORLD_CHAT_GREEN = "|cff00cc00";
std::string WORLD_CHAT_RED = "|cffFF0000";
std::string WORLD_CHAT_BLUE = "|cff6666FF";
std::string WORLD_CHAT_BLACK = "|cff000000";
std::string WORLD_CHAT_GREY = "|cff808080";

/* Class Colors */
std::string world_chat_ClassColor[11] =
{
    "|cffC79C6E", // WARRIOR
    "|cffF58CBA", // PALADIN
    "|cffABD473", // HUNTER
    "|cffFFF569", // ROGUE
    "|cffFFFFFF", // PRIEST
    "|cffC41F3B", // DEATHKNIGHT
    "|cff0070DE", // SHAMAN
    "|cff40C7EB", // MAGE
    "|cff8787ED", // WARLOCK
    "", // ADDED IN MOP FOR MONK - NOT USED
    "|cffFF7D0A", // DRUID
};

/* Ranks */
std::string world_chat_GM_RANKS[4] =
{
    "Player",
    "MOD",
    "GM",
    "ADMIN",
};

/* BLIZZARD CHAT ICON FOR GM'S */
std::string world_chat_GMIcon = "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:13:13:0:-1|t";

/* COLORED TEXT FOR CURRENT FACTION || NOT FOR GMS */
std::string world_chat_TeamIcon[2] =
{
    "|cff3399FFAlliance|r",
    "|cffCC0000Horde|r"
};


/* CONFIG VARIABLES */
int config_ChatEnabled = 1;
int config_ChatCrossFactions = 1;

/* STRUCTURE FOR WorldChat map */
struct ChatElements
{
    uint8 chat = 0; // CHAT DISABLED BY DEFAULT
};

/* UNORDERED MAP FOR STORING IF CHAT IS ENABLED OR DISABLED */
std::unordered_map<uint32, ChatElements>WorldChat;


class World_Chat : public CommandScript
{
public:
    World_Chat() : CommandScript("World_Chat") { }



    static bool HandleWorldChatCommand(ChatHandler * pChat, const char * msg)
    {

        if (!config_ChatEnabled) {
            Player* player = pChat->GetSession()->GetPlayer();
            ChatHandler(pChat->GetSession()).PSendSysMessage("[WC] %sWorld Chat System is disabled.", WORLD_CHAT_RED);
            return true;
        }

        if (!*msg)
            return false;

        Player* player = pChat->GetSession()->GetPlayer();
        uint32 guid = player->GetGUID();

        if (!WorldChat[guid].chat) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is disabled. (.chat)", WORLD_CHAT_RED);
            return true;
        }

        char message[1024];


        SessionMap sessions = sWorld->GetAllSessions();

        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second)
                continue;


            Player* target = itr->second->GetPlayer();
            uint32 guid2 = target->GetGUID();

            if (WorldChat[guid2].chat == 1)
            {
                if (config_ChatCrossFactions) {
                    if (player->IsGameMaster()) {
                        snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_GMIcon.c_str(), world_chat_ClassColor[player->getClass() - 1].c_str(), player->GetName().c_str(), WORLD_CHAT_WHITE.c_str(), msg);
                    }
                    else
                        snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_TeamIcon[player->GetTeamId()].c_str(), world_chat_ClassColor[player->getClass() - 1].c_str(), player->GetName().c_str(), WORLD_CHAT_WHITE.c_str(), msg);
                    ChatHandler(target->GetSession()).PSendSysMessage(message);
                }
                else
                {
                    if (player->GetTeamId() == target->GetTeamId())
                    {
                        if (player->IsGameMaster()) {
                            snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_GMIcon.c_str(), world_chat_ClassColor[player->getClass() - 1].c_str(), player->GetName().c_str(), WORLD_CHAT_WHITE.c_str(), msg);
                        }
                        else
                            snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_TeamIcon[player->GetTeamId()].c_str(), world_chat_ClassColor[player->getClass() - 1].c_str(), player->GetName().c_str(), WORLD_CHAT_WHITE.c_str(), msg);
                        ChatHandler(target->GetSession()).PSendSysMessage(message);
                    }
                    else if (target->IsGameMaster()) {
                        if (player->IsGameMaster()) {
                            snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_GMIcon.c_str(), world_chat_ClassColor[player->getClass() - 1].c_str(), player->GetName().c_str(), WORLD_CHAT_WHITE.c_str(), msg);
                        }
                        else
                            snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_TeamIcon[player->GetTeamId()].c_str(), world_chat_ClassColor[player->getClass() - 1].c_str(), player->GetName().c_str(), WORLD_CHAT_WHITE.c_str(), msg);
                        ChatHandler(target->GetSession()).PSendSysMessage(message);
                    }
                }
            }
        }

        return true;
    }

    static bool HandleWorldChatHordeCommand(ChatHandler * pChat, const char * msg)
    {

        if (!config_ChatEnabled) {
            Player* player = pChat->GetSession()->GetPlayer();
            ChatHandler(pChat->GetSession()).PSendSysMessage("[WC] %sWorld Chat System is disabled.|r", WORLD_CHAT_RED);
            return true;
        }

        if (!*msg)
            return false;

        Player* player = pChat->GetSession()->GetPlayer();
        uint32 guid = player->GetGUID();

        if (!WorldChat[guid].chat) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is disabled. (.chat)|r", WORLD_CHAT_RED);
            return true;
        }

        char message[1024];


        SessionMap sessions = sWorld->GetAllSessions();

        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second)
                continue;


            Player* target = itr->second->GetPlayer();
            uint32 guid2 = target->GetGUID();

            if (WorldChat[guid2].chat == 1 && target->GetTeamId() == TEAM_HORDE || WorldChat[guid2].chat == 1 && target->IsGameMaster())
            {
                snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_GMIcon.c_str(), world_chat_ClassColor[player->getClass() - 1].c_str(), player->GetName().c_str(), WORLD_CHAT_WHITE.c_str(), msg);
                ChatHandler(target->GetSession()).PSendSysMessage(message);
            }
        }

        return true;
    }

    static bool HandleWorldChatAllianceCommand(ChatHandler * pChat, const char * msg)
    {

        if (!config_ChatEnabled) {
            Player* player = pChat->GetSession()->GetPlayer();
            ChatHandler(pChat->GetSession()).PSendSysMessage("[WC] %sWorld Chat System is disabled.|r", WORLD_CHAT_RED);
            return true;
        }

        if (!*msg)
            return false;

        Player* player = pChat->GetSession()->GetPlayer();
        uint32 guid = player->GetGUID();

        if (!WorldChat[guid].chat) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is disabled. (.chat)|r", WORLD_CHAT_RED);
            return true;
        }

        char message[1024];


        SessionMap sessions = sWorld->GetAllSessions();

        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second)
                continue;


            Player* target = itr->second->GetPlayer();
            uint32 guid2 = target->GetGUID();

            if (WorldChat[guid2].chat == 1 && target->GetTeamId() == TEAM_ALLIANCE || WorldChat[guid2].chat == 1 && target->IsGameMaster())
            {
                snprintf(message, 1024, "[World][%s][%s%s|r]: %s%s|r", world_chat_GMIcon.c_str(), world_chat_ClassColor[player->getClass() - 1].c_str(), player->GetName().c_str(), WORLD_CHAT_WHITE.c_str(), msg);
                ChatHandler(target->GetSession()).PSendSysMessage(message);
            }
        }

        return true;
    }

    static bool HandleWorldChatOnCommand(ChatHandler* handler, const char* msg)
    {
        Player* player = handler->GetSession()->GetPlayer();
        uint32 guid = player->GetGUID();

        if (!config_ChatEnabled) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat System is disabled.|r", WORLD_CHAT_RED);
            return true;
        }

        if (WorldChat[guid].chat) {
            ChatHandler(player->GetSession()).PSendSysMessage("%s[WC] World Chat is already visible.|r", WORLD_CHAT_RED);
            return true;
        }

        WorldChat[guid].chat = 1;

        char message[512];


        ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is now visible. (.chat off to hide)|r", WORLD_CHAT_GREEN);
        sWorld->SendGlobalText(message, NULL);

        return true;
    };

    static bool HandleWorldChatOffCommand(ChatHandler* handler, const char* msg)
    {
        Player* player = handler->GetSession()->GetPlayer();
        uint32 guid = player->GetGUID();

        if (!config_ChatEnabled) {
            ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat System is disabled.|r", WORLD_CHAT_RED);
            return true;
        }

        if (!WorldChat[guid].chat) {
            ChatHandler(player->GetSession()).PSendSysMessage("%s[WC] World Chat is already hidden.|r", WORLD_CHAT_RED);
            return true;
        }

        WorldChat[guid].chat = 0;

        char message[512];

        ChatHandler(player->GetSession()).PSendSysMessage("[WC] %sWorld Chat is now hidden. (.chat on to show)|r", WORLD_CHAT_GREEN);
        sWorld->SendGlobalText(message, NULL);

        return true;
    };

    std::vector<ChatCommand> GetCommands() const
    {
        static std::vector<ChatCommand> wcCommandTable =
        {
            { "on",      rbac::RBAC_PERM_COMMAND_WORLD_CHAT,     true,     &HandleWorldChatOnCommand,      "" },
            { "off",     rbac::RBAC_PERM_COMMAND_WORLD_CHAT,     true,    &HandleWorldChatOffCommand,       "" },
            { "",        rbac::RBAC_PERM_COMMAND_WORLD_CHAT,     false,    &HandleWorldChatCommand,       "" },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "chat", rbac::RBAC_PERM_COMMAND_WORLD_CHAT, true, NULL , "" , wcCommandTable},
            { "chath", rbac::RBAC_PERM_COMMAND_WORLD_CHAT_HORDE, true, &HandleWorldChatHordeCommand , ""},
            { "chata", rbac::RBAC_PERM_COMMAND_WORLD_CHAT_ALLIANCE, true, &HandleWorldChatAllianceCommand , ""},
        };
        return commandTable;
    }
};

class World_Chat_Config : public WorldScript
{
public: World_Chat_Config() : WorldScript("World_Chat_Config") { };

        virtual void OnConfigLoad(bool /*reload*/)
        {
            TC_LOG_INFO("server.loading", "___________________________________");
            TC_LOG_INFO("server.loading", "|     WiiZZy World Chat Script    |");
            TC_LOG_INFO("server.loading", "|-                               -|");
            TC_LOG_INFO("server.loading", "|____________VER:%.2f_____________|", ver);

            config_ChatEnabled = sConfigMgr->GetIntDefault("World_Chat.Enable", 1);
            config_ChatCrossFactions = sConfigMgr->GetIntDefault("World_Chat.CrossFactions", 0);

            TC_LOG_INFO("server.loading", "|    WiiZZy World Chat : Loaded   |");
        }
};

void AddSC_World_Chat()
{
    new World_Chat_Config;
    new World_Chat;
}
