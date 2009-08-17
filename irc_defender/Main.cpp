/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   Main.cpp
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

#include <string.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv [])
{
    if(argc == 2)
    {
        if (strcmp ( argv[1], "--help" ) == 0)
        {
            printf ( "Usage: defender [OPTION]\n\n" );
            printf ( "  --c [FILE]      Alternate defender config file\n" );
            printf ( "  --help          Shows this.\n" );
            return 1;
        }
    }else
	if(argc == 0)
	{
		printf("Run server..");
	}
    return 1;
}