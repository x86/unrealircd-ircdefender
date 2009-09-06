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

#include <string.h>
#include <stdio.h>
using std;

class CLogging
{
	public:
		int sendConsole			(char* text);
		int sendLog				(std::string text);
		int sendMessage			(std::string user, std::string text);
};
#endif
