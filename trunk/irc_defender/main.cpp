/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   Main.cpp
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

/* *UNIX */
#include <iostream>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Scripts */
#include "Main.h"
//#include "iniparser.h"

/* Namespace */
using namespace std;

/* Main */
int CMain::main(int argc, char* argv[])
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
		// Read config
		//parse_ini_file("defender.conf");
		sendConsole("Run server..");
	}
    return 1;
}

/*int CMain::parse_ini_file(char* ini_name)
{
	dictionary	*	ini;
	int				b ;
	int				i ;
	double			d ;
	char		*	s ;

	ini = iniparser_load(ini_name);
	if (ini == NULL)
	{
		sendConsole("Cannot read config file!");
		return -1;
	}
	iniparser_dump(ini, stderr);

	s = iniparser_getstring(ini, "irc:irc", NULL);
    printf("IRC Adres:     [%s]\n", s ? s : "UNDEF");
    i = iniparser_getint(ini, "irc:port", -1);
    printf("Year:      [%d]\n", i);
	s = iniparser_getstring(ini, "irc:password", NULL);
    printf("Password:     [%s]\n", s ? s : "UNDEF");

	printf("Wine:\n");
	s = iniparser_getstring(ini, "wine:grape", NULL);
    printf("Grape:     [%s]\n", s ? s : "UNDEF");
	
    i = iniparser_getint(ini, "wine:year", -1);
    printf("Year:      [%d]\n", i);

	s = iniparser_getstring(ini, "wine:country", NULL);
    printf("Country:   [%s]\n", s ? s : "UNDEF");
	
    d = iniparser_getdouble(ini, "wine:alcohol", -1.0);
    printf("Alcohol:   [%g]\n", d);

	iniparser_freedict(ini);
	return 0;
}*/

int CMain::sendConsole(char* text)
{
	time_t t = time(0);
	struct tm* lt = localtime(&t);
	printf("[%02d:%02d:%02d] Defender: %s\n", lt->tm_hour, lt->tm_min, lt->tm_sec, text);
	return 1;
}
