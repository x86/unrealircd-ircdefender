/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   CCConnection::h
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

// Define vars.
string CConnection::botnick;

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
	size_t found = str.find(" PRIVMSG " + CConnection::botnick + " :");
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
		CLogging::sendMessage(user, "IRCDefender Commands:");
		CLogging::sendMessage(user, "  Commands available to All:");
		CLogging::sendMessage(user, "     HELP                  Shows the help list.");
		CLogging::sendMessage(user, "     VERSION               Shows the version.");
		CLogging::sendMessage(user, "     CREDITS               Shows the credits.");
		CLogging::sendMessage(user, "  Commands available to Services Admins:");
		CLogging::sendMessage(user, "     SET                   Configure IRCDefender.");
		CLogging::sendMessage(user, "     SECURE                Modify the secure level.");
		CLogging::sendMessage(user, "     JOIN                  ");
		CLogging::sendMessage(user, "     PART                Modify the secure level.");
		CLogging::sendMessage(user, "  Commands available to Network Administrators:");
		CLogging::sendMessage(user, "     EXIT                  Terminate the program with no save.");
	}else
	if(command == "version" || command == "VERSION")
	{
		status = "OK";
		CLogging::sendMessage(user, "IRCDefender Version:");
		CLogging::sendMessage(user, "  Core: 1.0");
		CLogging::sendMessage(user, "  Defender: 1.0.0003");
	}else
	if(command == "credits" || command == "CREDITS")
	{
		status = "OK";
		CLogging::sendMessage(user, "IRCDefender Credits:");
		CLogging::sendMessage(user, "  Head developer(s):");
		CLogging::sendMessage(user, "  - i386 <sebasdevelopment@gmx.com>");
	}else
	if(command == "join" || command == "JOIN")
	{
		status = "OK";
        CConnection::sendData(":" + CConnection::botnick + " JOIN " + arguments + "\r\n");
        CConnection::sendData(":" + CConnection::botnick + " MODE " + arguments + " +ao " + CConnection::botnick + " " + CConnection::botnick + "\r\n");
	}else
	if(command == "part" || command == "PART")
	{
		status = "OK";
		CConnection::sendData(":" + CConnection::botnick + " PART " + arguments + " :Requested by: " + user + "\r\n");
	}else
	/*if(command == "check" || command == "CHECK")
	{
		status = "OK";
		if(getUserLevel(user, "a"))
		{
			CConnection::sendMessage(user, "You are Services Administrator!");
		}
		if(getUserLevel(user, "N"))
		{
			CConnection::sendMessage(user, "You are Network Administrator!");
		}
		if(getUserLevel(user, "o"))
		{
			CConnection::sendMessage(user, "You are Global Operator!");
		}
	}else*/
	if(command == "raw" || command == "RAW")
	{
		status = "OK";
		CConnection::sendData(arguments + "\r\n");
	}
	
	if(status == "FAIL")
	{
		CLogging::sendMessage(user, "Command not found, try: /msg " + CConnection::botnick + " HELP");
	}

	// Send the log!
	CLogging::sendMessage(user, "Notice: All actions will be logged!");
	CLogging::sendLog("[" + status + "] " + user + " requested command: " + command);
	return 1;
}
