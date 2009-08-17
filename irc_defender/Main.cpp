/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   Main.cpp
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

/* Includes */
#include "Main.h"
#include <iostream>
#include <time.h>
#include <string.h>

/* Namespace */
using namespace std;

/* Main */
int CMain::main(int argc, char* argv [])
{
    if(argc == 1)
    {
        if(strcmp(argv[1], "--help") == 0)
        {
            sendConsole("Usage: defender [OPTION]\n");
            sendConsole("  --c [FILE]      Alternative defender config file");
            sendConsole("  --help          Shows this." );
            return 1;
        }
    }else
	if(argc == 0)
	{
		sendConsole("Run server..");
	}
    return 1;
}

int CMain::sendConsole(char* text)
{
	time_t t = time(0);
	struct tm* lt = localtime(&t);
	char time_str[15];
	sprintf_s(time_str, "%02d:%02d:%02d", lt->tm_hour, lt->tm_min, lt->tm_sec);
	printf("[%s] Defender: %s\n", time_str, text);
	return 1;
}
