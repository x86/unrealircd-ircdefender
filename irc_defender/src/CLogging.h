/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   CConnection.h
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

#ifndef __CLOGGING_H
#define __CLOGGING_H

#include <stdio.h>
#include <string>

class CLogging
{
	public:
		int CLogging::sendConsole			(char* text);
		int CLogging::sendLog				(std::string text);
		int CLogging::sendMessage			(std::string user, std::string text);
};
#endif
