/*
 * xpAddress.h
 *
 *  Created on: May 21, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPADDRESS_H_
#define XPADDRESS_H_

#include <string>
#include <stdio.h>


#ifndef WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif


//!  It holds the network endpoint informations.
/*!
  An xpAddress keep informations concerning network endpoints and implements the methods to obtain them.
*/
class xpAddress {
public:
	/*! xpAddress(const std::string ip_port, const std::string session)
	    \brief Constructor for the xpAddress class.
		\param ip_port The address as std:string, formatted ad "http://ip:port".
		\param session The session active at the indicated address as std::string.
		\details Explicit constructor for the xpAddress class, used to keep control of the address typical informations.
		\return An instanced xpAddress object.
	*/
	xpAddress(const std::string ip_port, const std::string session);

	/*! ~xpAddress()
	    \brief Destructor for the xpAddress class.
	*/
	virtual ~xpAddress();

	/*! \fn sockaddr_in toSocketAddress()
	    \brief It returns the xpAddress as sockaddr_in.
		\details xpAddress must be converted to sock_in to be used when forwarding information through UDP.
		\return The xpAddress object as sockaddr_in.
	*/
	sockaddr_in toSocketAddress();

public:
	const std::string ip_port; /*!< The std::string representing the xpAddress formatted ad "http://ip:port" */
	const std::string session; /*!< The std::string representing the active session on ip_port */
};

#endif /* XPADDRESS_H_ */
