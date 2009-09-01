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
#include <fstream>
#include <string>
#include <map>
#include <list>

// Scripts
#include "configreader.h"

using namespace std;

// Vars
int ircSocket;
pthread_t t;

// Config vars
map<string, string> config;
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
int closesocket(int socket);
int sendData(std::string text);
void *messageThread(void* x);
void onDataReceived(char* msg);
int readConfig(char* filename);

/* ------------------------------------------------------------------------------------------------------*/
/* Main */
int main(int argc, char* argv[])
{
    if(argc != 0 && argc == 2)
    {
        // TODO: add --config [file]  (or not)
        if(strcmp(argv[1], "--help") == 0)
        {
            sendConsole("\n------------[IRC Defender]------------\n");
            sendConsole("Usage: defender [OPTION]");
            sendConsole("  --config [FILE]      Alternative defender config file");
            sendConsole("  --help               Shows this.");
            sendConsole("  --credits            Shows the credits.\n");
            sendConsole("\n------------[IRC Defender]------------");
            return 1;
        }
        if(strcmp(argv[1], "--credits") == 0)
        {
            sendConsole("\n------------[IRC Defender Credits]------------\n");
            sendConsole("  Head developer(s):");
            sendConsole(" - i386           <sebasdevelopment@gmx.com");
            sendConsole("\n");
            sendConsole("  Special thanks:");
            sendConsole(" - UnrealIRCd\n");
            sendConsole("\n------------[IRC Defender Credits]------------");
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
		sendConsole("Server started..\n");
		startServer("defender.conf");
	}

    // Close server
    sendConsole("Stopping server..\n");
    closesocket(ircSocket);

    return 1;
}

void startServer(char* configfile)
{
        // Vars
        struct sockaddr_in destination;

		// Read config and Set data..
		readConfig(configfile);
		Config config(configfile, "A");

		ircadres = config.pString("irc");
		ircport = config.pInt("port");
		ircpass = config.pString("password");
		servicesname = config.pString("ulinename");
		botnick = config.pString("botnick");
		logchannel = config.pString("logchannel");
		enablelogging = config.pBool("enablelogging");

        // Create Socket
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
                if (ircSocket)
                {
                        closesocket(ircSocket);
                }
                return;
        }

		// Start thread
        pthread_create(&t, 0, messageThread, NULL);

        // Send auth
        sendData("PASS :" + ircpass + "\r\n");
        sendData("PROTOCTL NOQUIT\r\n");
        sendData("SERVER " + servicesname + " 1 :IRC Defender\r\n");
        sendData("EOS\r\n");

        // Create bot..
        sendData("SQLINE " + botnick + " :reserved 4 IRC Defender\r\n");
        sendData("NICK " + botnick + " 1 0001 " + botnick + " " + servicesname + " " + servicesname + " 001 :IRC Defender\r\n");
        sendData(":" + botnick + " MODE " + botnick + " +Sq\r\n");
        sendData(":" + botnick + " JOIN " + logchannel + "\r\n");
        sendData(":" + botnick + " MODE " + logchannel + " +o " + botnick + "\r\n");
        sendData(":" + botnick + " PRIVMSG " + logchannel + " :Logging here..\r\n");

        // ....
        sendConsole("INFO -> Connected!");
        while(true) { } // Keep server alive!
}

int closesocket(int socket)
{
        close(socket);
        sendConsole("INFO -> Socket closed!");
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

void onDataReceived(char* msg)
{
        if(strncmp(msg, "PING", 4) == 0) // Check for "PING"
        {
                // Send "PONG" back
                msg[1] = 'O';
                sendData("PONG :REPLY\r\n");
                sendConsole("Ping received, ponged back.");
        }

        sendConsole(msg);
        return;
}

void *messageThread(void* x)
{
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
		usleep(70); // MS
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

int readConfig(char* filename)
{
    char _buff[1024], _ch=' ', tag[24];
    float val;
    ifstream cfg(filename);

    while(!cfg.eof())
	{
        _ch = cfg.get();
        printf("[%c]\n",_ch);
        if(_ch != '#' && _ch != '\n' && _ch != '//')
		{
            cfg.getline(_buff, 1024);
            //puts(_buff);
            sscanf(_buff, "%s %*s %f",tag, &val);
            printf("Tag: [%c]%s Value: %f\n", _ch, tag, val);
        }
        cfg.ignore(1024,'\n');
        _ch = cfg.peek();
        while(_ch==' ' && _ch=='\n')
		{
            cfg.ignore(1024,'\n');
            _ch = cfg.peek();
        }
    }

    cfg.close();
    return 0;
}
