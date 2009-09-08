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
		static int sendConsole			(char* text);
		static int sendLog				(std::string text);
		static int sendMessage			(std::string user, std::string text);
};
#endif
