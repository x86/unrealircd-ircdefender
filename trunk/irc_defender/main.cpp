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

// Vars
int ircSocket;
pthread_t t;

// Functions
int sendConsole(char* text);
void startServer();
void closesocket(int socket);
int sendData(char* text);
void *messageThread(void* x);
void onDataReceived(char* msg);

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
    }

    // Read config
    //parse_ini_file("defender.conf");
    sendConsole("Server started..\n");
    startServer();

    // Close server
    sendConsole("Stopping server..\n");
    // TODO: Remove sockets..
    return 1;
}

void startServer()
{
        // Vars
        struct sockaddr_in destination;

        // Create Socket
        destination.sin_family = AF_INET;
        ircSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ircSocket < 0)
        {
                sendConsole("PANIC -> Socket Creation FAILED!");
                return;
        }

        // Connect to irc
        destination.sin_port = htons(3827);
        destination.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (connect(ircSocket, (struct sockaddr *)&destination, sizeof(destination)) != 0)
        {
                sendConsole("PANIC -> Socket Connection FAILED!");
                if (ircSocket)
                {
                        closesocket(ircSocket);
                }
                return;
        }

        pthread_create(&t, 0, messageThread, NULL);

        // Send auth
        sendData("PASS :PASSWORD\r\n");
        sendData("PROTOCTL NOQUIT\r\n");
        sendData("SERVER defender.servername.nl 1 :IRC Defender\r\n");

        // Create bots..
        sendData("SQLINE Defender :reserved 4 IRC Defender\r\n");
        sendData("NICK Defender 1 0001 Defender defender.servername.nl defender.servername.nl 001 :IRC Defender\r\n");
        sendData(":Defender MODE Defender +Sq\r\n");
        sendData(":Defender JOIN #services\r\n");
        sendData(":Defender MODE #services +o Defender\r\n");

        // ....
        sendConsole("OK -> Connected!");
        while(true) { int y=0; y=1; }
}

void closesocket(int socket)
{
        close(socket);
        sendConsole("INFO -> Socket closed!");
        return;
}

int sendData(char* text)
{
        send(ircSocket, text, strlen(text), 0);
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
        // Check for "PING"
        if(strncmp(msg, "PING", 4) == 0)
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

    while((select(0, &fdSetRead, NULL, NULL, &timeout)) != -1)
    {
         sendConsole("Hoi");
         char buf[1024];
         int i = recv(ircSocket, buf, 1024, 0);
         if(i > 0)
         {
                buf[i] = '\0';
                char part[512];
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
         //Sleep(50);
    }
}