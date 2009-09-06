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
		void CConnection::startServer		(char* configfile);
		int CConnection::stopServer			();
		int CConnection::closesocket			(int socket);
		int CConnection::sendData			(std::string text);
		void *CConnection::messageThread		(void* x);
		void CConnection::onDataReceived		(char* msg);
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
