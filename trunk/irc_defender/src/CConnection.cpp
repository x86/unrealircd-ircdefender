/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   CConnection.cpp
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

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

#include "CConfig.h"
#include "CConnection.h"
#include "CCommands.h"
#include "CLogging.h"

// -----------------------------------------------------------
void CConnection::startServer(char* configfile)
{
		// Read config and Set data..
		ConfigFile config(configfile);

		config.readInto(ircadres, "irc");
		config.readInto(ircport, "port");
		config.readInto(ircpass, "password");
		config.readInto(servicesname, "ulinename");
		config.readInto(botnick, "botnick");
		config.readInto(logchannel, "logchannel");
		config.readInto(enablelogging, "enablelogging");

        // Create Socket
        struct sockaddr_in destination;
        destination.sin_family = AF_INET;
        ircSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ircSocket < 0)
        {
                CLogging::sendConsole("PANIC -> Socket Creation FAILED!");
                return;
        }

        // Connect to irc
		const char *ircadresChar;
		ircadresChar = ircadres.c_str();

        destination.sin_port = htons(ircport);
        destination.sin_addr.s_addr = inet_addr(ircadresChar);
        if (connect(ircSocket, (struct sockaddr *)&destination, sizeof(destination)) != 0)
        {
                CLogging::sendConsole("PANIC -> Socket Connection FAILED!");
                return;
        }

		// Start thread
		pthread_create(&t, 0, CConnection::messageThread, NULL);

        // Send auth
        CConnection::sendData("PASS :" + ircpass + "\r\n");
        CConnection::sendData("PROTOCTL NOQUIT\r\n");
        CConnection::sendData("SERVER " + servicesname + " 1 :IRCDefender\r\n");
        CConnection::sendData("EOS\r\n");

        // Create bot..
        CConnection::sendData("SQLINE " + botnick + " :reserved 4 IRCDefender\r\n");
        CConnection::sendData("NICK " + botnick + " 1 0001 " + botnick + " " + servicesname + " " + servicesname + " 001 :IRCDefender\r\n");
        CConnection::sendData(":" + botnick + " MODE " + botnick + " +Sq\r\n");
        CConnection::sendData(":" + botnick + " JOIN " + logchannel + "\r\n");
        CConnection::sendData(":" + botnick + " MODE " + logchannel + " +o " + botnick + "\r\n");

		if(enablelogging)
		{
			CConnection::sendData(":" + botnick + " PRIVMSG " + logchannel + " :Logging here..\r\n");
		}

        // ....
        CLogging::sendConsole("INFO -> Connected!");
        while(true) { } // Keep server alive!
}

int CConnection::stopServer()
{
		CLogging::sendConsole("PANIC -> There was an error found, see the log files!");
		CConnection::closesocket(ircSocket);
		return 1;
}

int CConnection::sendData(std::string text)
{
		string output = text;
		send(ircSocket, output.c_str(), output.length(), 0);
        return 1;
}

int CConnection::closesocket(int socket)
{
		if(socket > 0)
		{
			close(socket);
			CLogging::sendConsole("INFO -> Socket closed!");
		}
		exit(1);
        return 1;
}


void CConnection::onDataReceived(char* msg)
{
		// Error handler
		if(strncmp(msg, "ERROR :", 7) == 0)
		{
			// ERROR :Link denied
			CConnection::stopServer();
		}

		// Ping handler
        if(strncmp(msg, "PING", 4) == 0)
        {
                msg[1] = 'O';
				CConnection::sendData("PONG :REPLY\r\n");
                CLogging::sendConsole("Ping received, ponged back.");
        }

		// Command handler
		CCommands::handleCommands(msg);

		// Post debug to server.
		// TODO: Hide this when releasing.
        CLogging::sendConsole(msg);
		// -----

        return;
}

void *CConnection::messageThread(void* x)
{
    while(true)
    {
         char buf[1024];
         int i = recv(ircSocket, buf, 1024, 0);
         if(i > 0)
         {
                buf[i] = '\0';
                char part[1024];
                for (i = 0; i < (int)strlen(buf); i++)
                {
                        if (buf[i] == '\n')
                        {
								CConnection::onDataReceived(part);
                                memset(&part, 0, sizeof(part));
								usleep(70); // MS
                        }else
                        if (buf[i] != '\r')
                        {
                                part[strlen(part)] = buf[i];
                        }
                }
         }
    }
}