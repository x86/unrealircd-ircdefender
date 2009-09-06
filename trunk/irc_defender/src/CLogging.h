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
using std::string;

class CLogging
{
	public:
		int sendConsole			(char* text);
		int sendLog				(string text);
		int sendMessage			(string user, string text);
};
#endif
