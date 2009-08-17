/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   Main.h
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

class CMain;

#ifndef __CMAIN_H
	#define __CMAIN_H

	#include <string>
	using namespace std;

	class CMain
	{
		public:
			static int		sendConsole         (char* text);
			static int		main				(int argc, char* argv []);
	};
#endif