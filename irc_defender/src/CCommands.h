/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   CConnection.h
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

#ifndef __CCOMMANDS_H
#define __CCOMMANDS_H

#include <stdio.h>

class CCommands
{
	public:
		static int search				(char *string, char *substring);
		static int handleCommands		(char* data);
};
#endif
