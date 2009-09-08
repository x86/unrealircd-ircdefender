/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   CLogging.cpp
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/
#include <string>
#include <stdio.h>
#include <pthread.h>

using namespace std;

#include "CLogging.h"
#include "CConnection.h"

// -----------------------------------------------------------
int CLogging::sendLog(string text)
{
	if(CConnection::enablelogging)
	{
		CConnection::sendData(":" + CConnection::botnick + " PRIVMSG " + CConnection::logchannel + " :" + text + "\r\n");
	}
	return 1;
}

int CLogging::sendMessage(string user, string text)
{
	CConnection::sendData(":" + CConnection::botnick + " NOTICE " + user + " :" + text + "\r\n");
	return 1;
}

int CLogging::sendConsole(char* text)
{
    time_t t = time(0);
    struct tm* lt = localtime(&t);
    printf("[%02d:%02d:%02d] Defender: %s\n", lt->tm_hour, lt->tm_min, lt->tm_sec, text);
    return 1;
}
