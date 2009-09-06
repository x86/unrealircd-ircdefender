/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   CConnection.h
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

#include <string>
#include <stdio.h>

using namespace std;

#include "CCommands.h"
#include "CLogging.h"
#include "CConnection.h"

CConnection connection;
CLogging logging;

// -----------------------------------------------------------
int CCommands::search(char *string, char *substring)
{
   int i=0, j=0, yes=0, index;

   while(string[i]!='\0')
   {
       if(string[i]==substring[j])
       {
            index=i;

            for (j=1;substring[j];j++)
            {
                i++;

                if(string[i]==substring[j])
				{
					 yes=1;
				}else{
					yes=0;
					break;
				}
			}
			if(yes==1) return index;
		}
		i++;
		j=0;
     }
   return -1;
}


int CCommands::handleCommands(char* data)
{
	string str = data;

	// Check
	size_t found = str.find(" PRIVMSG " + connection.botnick + " :");
	if (found == string::npos) { return 0; }

	// Example for requesting help:
	// :Sebas PRIVMSG Defender :help
	str = str.substr(1); // Hide :
	size_t foundName = str.find_first_of(" ");
	string user = str.substr(0, foundName).c_str();
	size_t commandName = str.find_first_of(":");
	string command = str.substr(commandName+1).c_str();
	string arguments = "";

	size_t anyArguments = command.find_first_of(" ");
	if(anyArguments != string::npos)
	{
		// We found arguments.
		arguments = command.substr(anyArguments).c_str();
		command = str.substr(commandName+1, anyArguments).c_str();
	}

	if(user == "NeoStats" || command == " VERSION") { return 0; }
	if(command == "") { return 0; }
	
	string status = "FAIL";

	// Commands
	if(command == "help" || command == "HELP")
	{
		status = "OK";
		logging.sendMessage(user, "IRCDefender Commands:");
		logging.sendMessage(user, "  Commands available to All:");
		logging.sendMessage(user, "     HELP                  Shows the help list.");
		logging.sendMessage(user, "     VERSION               Shows the version.");
		logging.sendMessage(user, "     CREDITS               Shows the credits.");
		logging.sendMessage(user, "  Commands available to Services Admins:");
		logging.sendMessage(user, "     SET                   Configure IRCDefender.");
		logging.sendMessage(user, "     SECURE                Modify the secure level.");
		logging.sendMessage(user, "     JOIN                  ");
		logging.sendMessage(user, "     PART                Modify the secure level.");
		logging.sendMessage(user, "  Commands available to Network Administrators:");
		logging.sendMessage(user, "     EXIT                  Terminate the program with no save.");
	}else
	if(command == "version" || command == "VERSION")
	{
		status = "OK";
		logging.sendMessage(user, "IRCDefender Version:");
		logging.sendMessage(user, "  Core: 1.0");
		logging.sendMessage(user, "  Defender: 1.0.0003");
	}else
	if(command == "credits" || command == "CREDITS")
	{
		status = "OK";
		logging.sendMessage(user, "IRCDefender Credits:");
		logging.sendMessage(user, "  Head developer(s):");
		logging.sendMessage(user, "  - i386 <sebasdevelopment@gmx.com>");
	}else
	if(command == "join" || command == "JOIN")
	{
		status = "OK";
        connection.sendData(":" + connection.botnick + " JOIN " + arguments + "\r\n");
        connection.sendData(":" + connection.botnick + " MODE " + arguments + " +ao " + connection.botnick + " " + connection.botnick + "\r\n");
	}else
	if(command == "part" || command == "PART")
	{
		status = "OK";
		connection.sendData(":" + connection.botnick + " PART " + arguments + " :Requested by: " + user + "\r\n");
	}else
	/*if(command == "check" || command == "CHECK")
	{
		status = "OK";
		if(getUserLevel(user, "a"))
		{
			connection.sendMessage(user, "You are Services Administrator!");
		}
		if(getUserLevel(user, "N"))
		{
			connection.sendMessage(user, "You are Network Administrator!");
		}
		if(getUserLevel(user, "o"))
		{
			connection.sendMessage(user, "You are Global Operator!");
		}
	}else*/
	if(command == "raw" || command == "RAW")
	{
		status = "OK";
		connection.sendData(arguments + "\r\n");
	}
	
	if(status == "FAIL")
	{
		logging.sendMessage(user, "Command not found, try: /msg " + connection.botnick + " HELP");
	}

	// Send the log!
	logging.sendMessage(user, "Notice: All actions will be logged!");
	logging.sendLog("[" + status + "] " + user + " requested command: " + command);
	return 1;
}
