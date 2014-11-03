/*
 * xpAddress.cpp
 *
 *  Created on: May 21, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpAddress.h"
#include <stdlib.h>
#include <string.h>

xpAddress::xpAddress(const std::string _ip_port, const std::string _session) : ip_port(_ip_port), session(_session){
	//this->ip_port = ip_port;
	//this->session = session;

}

xpAddress::~xpAddress() {
}

sockaddr_in xpAddress::toSocketAddress() {

#ifdef WIN32
	unsigned long ulAddr = INADDR_NONE;
#endif
	sockaddr_in socketAddr_in;

	//printf(">>>>>> %s\n", ip_port.c_str());
	std::string::size_type found = ip_port.find_last_of(':');

	//Ip_port is like http://aaa.bbb.ccc:port
	//printf(">>>>>> %d\n", found);
	std::string ip = ip_port.substr(7, found -7 );
	//printf(">>>>>> %s\n", ip.c_str());
	std::string port = ip_port.substr(found+1);


	memset((char *) &socketAddr_in, 0, sizeof(socketAddr_in));
	socketAddr_in.sin_family = AF_INET;
	socketAddr_in.sin_port = htons(atoi(port.c_str()));

#ifdef WIN32
	ulAddr  = inet_addr(ip.c_str());
	if (ulAddr == INADDR_NONE){
		printf("inet_addr failed and returned INADDR_NONE\n");
		WSACleanup();
	}
	if (ulAddr == INADDR_ANY) {
	        printf("inet_addr failed and returned INADDR_ANY\n");
	        WSACleanup();
	    }
	socketAddr_in.sin_addr.s_addr = ulAddr;
#else
	if (inet_aton(ip.c_str(), &socketAddr_in.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}
#endif

	return socketAddr_in;
}
