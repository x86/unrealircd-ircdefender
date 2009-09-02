/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   Main.cpp
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

/* *UNIX */
#include <time.h>
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
#include <iostream>

using namespace std;

// Scripts
#include "ConfigFile.h"

// Vars
int ircSocket;
pthread_t t;

// Config vars
string ircadres;
int ircport;
string ircpass;
string servicesname;
string botnick;
string logchannel;
bool enablelogging;

// Functions
int sendConsole(char* text);
void startServer(char* configfile);
int stopServer();
int closesocket(int socket);
int sendData(std::string text);
int handleCommands(char* data);
void *messageThread(void* x);
void onDataReceived(char* msg);

/* ------------------------------------------------------------------------------------------------------*/
/* Main */
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
			sendConsole("Server started..\n");
			startServer(argv[2]);
		}
	}else{
		// Start server
		sendConsole("Server started..");
		startServer("defender.conf");
	}

    // Close server
    sendConsole("Stopping server..\n.");
    closesocket(ircSocket);

    return 1;
}

void startServer(char* configfile)
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
                sendConsole("PANIC -> Socket Creation FAILED!");
                return;
        }

        // Connect to irc
		const char *ircadresChar;
		ircadresChar = ircadres.c_str();

        destination.sin_port = htons(ircport);
        destination.sin_addr.s_addr = inet_addr(ircadresChar);
        if (connect(ircSocket, (struct sockaddr *)&destination, sizeof(destination)) != 0)
        {
                sendConsole("PANIC -> Socket Connection FAILED!");
                return;
        }

		// Start thread
        pthread_create(&t, 0, messageThread, NULL);

        // Send auth
        sendData("PASS :" + ircpass + "\r\n");
        sendData("PROTOCTL NOQUIT\r\n");
        sendData("SERVER " + servicesname + " 1 :IRC Defender by i386\r\n");
        sendData("EOS\r\n");

        // Create bot..
        sendData("SQLINE " + botnick + " :reserved 4 IRC Defender by i386\r\n");
        sendData("NICK " + botnick + " 1 0001 " + botnick + " " + servicesname + " " + servicesname + " 001 :IRC Defender by i386\r\n");
        sendData(":" + botnick + " MODE " + botnick + " +Sq\r\n");
        sendData(":" + botnick + " JOIN " + logchannel + "\r\n");
        sendData(":" + botnick + " MODE " + logchannel + " +o " + botnick + "\r\n");
		if(enablelogging)
		{
			sendData(":" + botnick + " PRIVMSG " + logchannel + " :Logging here..\r\n");
		}

        // ....
        sendConsole("INFO -> Connected!");
        while(true) { } // Keep server alive!
}

int stopServer()
{
		sendConsole("PANIC -> There was an error found, see the log files!");
		closesocket(ircSocket);
		return 1;
}

int closesocket(int socket)
{
		if(socket > 0)
		{
			close(socket);
			sendConsole("INFO -> Socket closed!");
		}
		exit(1);
        return 1;
}

int sendData(std::string text)
{
		string output = text;
		send(ircSocket, output.c_str(), output.length(), 0);
        return 1;
}

int sendConsole(char* text)
{
        time_t t = time(0);
        struct tm* lt = localtime(&t);
        printf("[%02d:%02d:%02d] Defender: %s\n", lt->tm_hour, lt->tm_min, lt->tm_sec, text);
        return 1;
}

int handleCommands(char* data)
{

	return 1;
}

void onDataReceived(char* msg)
{
		// Error handler
		if(strncmp(msg, "ERROR :", 7) == 0)
		{
			stopServer();
			// ERROR :Link denied
		}

		// Ping handler
        if(strncmp(msg, "PING", 4) == 0)
        {
                msg[1] = 'O';
                sendData("PONG :REPLY\r\n");
                sendConsole("Ping received, ponged back.");
        }

		// Command handler
		handleCommands(msg);

		// Post debug to server.
        sendConsole(msg);
        return;
}

void *messageThread(void* x)
{
	usleep(50);
    fd_set fdSetRead;
    timeval timeout;
    FD_ZERO(&fdSetRead);
    FD_SET(ircSocket, &fdSetRead);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

	// Thread created!
    sendConsole("INFO -> Thread started (SUCCES)!");

	// While!
    while(true)
    {
        int rc = select(0, &fdSetRead, NULL, NULL, &timeout);
        if(rc != -1)
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
                                onDataReceived(part);
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
}
