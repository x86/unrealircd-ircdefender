/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   main.cpp
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

/* *UNIX */
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include <cstdlib>
#include <sys/select.h>
using namespace std;

#include "CConnection.h"
#include "CLogging.h"

// Functions
//bool getUserLevel(std::string user, char* level);

// -----------------------------------------------------------
int main(int argc, char* argv[])
{
    if(argc != 0 && argc == 2)
    {
        if(strcmp(argv[1], "--help") == 0)
        {
            printf("\n---------------------[IRC Defender]---------------------\n");
            printf("Usage: defender [OPTION] [ARGUMENT]");
            printf("  --config [FILE]      Alternative defender config file");
            printf("  --help               Shows this.");
            printf("  --credits            Shows the credits.\n");
            printf("\n---------------------[IRC Defender]---------------------");
            return 1;
        }
        if(strcmp(argv[1], "--credits") == 0)
        {
            printf("\n---------------------[IRC Defender Credits]-------------\n");
            printf("  Head developer(s):");
            printf(" - i386           <sebasdevelopment@gmx.com");
            printf("\n");
            printf("  Special thanks:");
            printf(" - UnrealIRCd\n");
            printf("\n---------------------[IRC Defender Credits]-------------");
            return 1;
        }
		if(strcmp(argv[1], "--config") == 0)
        {
			// Start server
			CLogging::sendConsole("Server started..\n");
			CConnection::startServer(argv[2]);
		}
	}else{
		// Start server
		CLogging::sendConsole("Server started..");
		CConnection::startServer("defender.conf");
	}

    // Close server
	CLogging::sendConsole("Stopping server..\n.");
	CConnection::closesocket(ircSocket);

    return 1;
}

/*bool getUserLevel(std::string user, char* level)
{
	if(level == "") { return false; }

	CConnection::sendData(":" + botnick + " WHOIS " + user + "\r\n");
	char buf[1024];
	int i = recv(ircSocket, buf, 1024, 0);
	if(i > 0)
	{
		printf("%s", buf);

		if(search(buf, level) != -1)
		{
			return true;
		}
	}
	return false;
}
*/