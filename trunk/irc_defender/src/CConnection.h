/*
    --------------------------------------------
    Project:    IRC Defender v1.0
    Filename:   CConnection.h
    Date:       09 August 2009
    Developers: i386 <sebasdevelopment@gmx.com>
    --------------------------------------------
*/

#ifndef __CCONNECTION_H
#define __CCONNECTION_H

#include <string.h>
#include <stdio.h>
#include <pthread.h>
using std::string;

class CConnection
{
	public:
		void startServer		(char* configfile);
		int stopServer			();
		int closesocket			(int socket);
		int sendData			(string text);
		void *messageThread		(void* x);
		void onDataReceived		(char* msg);
	private:
		int ircSocket;
		pthread_t t;
		string ircadres;
		int ircport;
		string ircpass;
		string servicesname;
		string botnick;
		string logchannel;
		bool enablelogging;
};
#endif
