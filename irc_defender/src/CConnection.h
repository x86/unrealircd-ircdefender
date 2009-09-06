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

#include <string>
#include <stdio.h>
#include <pthread.h>

class CConnection
{
	public:
		static	void startServer		(char* configfile);
		static	int stopServer			();
		static	int closesocket			(int socket);
		static	int sendData			(std::string text);
		static	void* messageThread		(void* x);
		static	void onDataReceived		(char* msg);
	//private:
		int ircSocket;
		pthread_t t;
		std::string ircadres;
		int ircport;
		std::string ircpass;
		std::string servicesname;
		std::string botnick;
		std::string logchannel;
		bool enablelogging;
};
#endif
