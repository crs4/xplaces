/*
 * xpUDPService.h
 *
 *  Created on: Jul 6, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPUDPSERVICE_H_
#define XPUDPSERVICE_H_

#ifndef WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#else
#include "winsock2.h"
#include "ws2tcpip.h"
#endif

#include <stdio.h>
#include <map>

#include "../xpMessage.h"

//!  xpUDPService is an UDP network interface.
/*!
  xpUDPService implements the methods to send and receive informations from and through an xplaces network by forwarding xpMessage objects.
*/
class xpUDPService {
public:
	/*! xpUDPService(std::string ip_port)
	    \brief Constructor for the xpUDPService class.
		\details Constructor for the xpUDPService class, the service will be listen on the specified address.
		\param ip_port The address that the service will binds starting ad std::string formatted as "http://ip:port".
		\return An xpUDPService object
	*/
	xpUDPService(std::string ip_port);

	/*! ~xpUDPService()
	    \brief Destructor for the xpUDPService class.
	*/
	virtual ~xpUDPService();

	/*! \fn int announce(xpMessage device)
	    \brief It forwards an announce message.
		\details It forwards an announce message to the specified destination, the properties of the device are indicated inside the param.
		\param device The announce message to send, it contains the properties fields as xpMessage.
		\return An integer indicating the status of the forwarding, 0 if success.
	*/
	virtual	int announce(xpMessage device);

	/*! \fn int action(xpMessage action)
	    \brief It forwards an action message.
		\details It forwards an action message to the specified destination, the properties of the action are indicated inside the param.
		\param action The action message to send, it contains the properties fields as xpMessage.
		\return An integer indicating the status of the forwarding, 0 if success.
	*/
	virtual	int action(xpMessage action);

	/*! \fn int event(xpMessage event)
	    \brief It forwards an event message.
		\details It forwards an event message to the specified destination, the properties of the event are indicated inside the param.
		\param event The event message to send, it contains the properties fields as xpMessage.
		\return An integer indicating the status of the forwarding, 0 if success.
	*/
	virtual	int event(xpMessage event);

	/*! \fn int addEventListener(xpMessage device, long type)
	    \brief It forwards an addEventListener request.
		\details It forwards an addEventListener request to the specified destination that implements an xpDeviceImpl. The listener session and address will be added the the destination related list.
		\param device The request to forward, it contains the listener informations as ip, port and session.
		\param type
		\return An integer indicating the status of the forwarding, 0 if success.
	*/
	virtual	int addEventListener(xpMessage device, long type);

	/*! \fn int removeEventListener(xpMessage device)
	    \brief It forwards an removeEventListener request.
		\details It forwards an removeEventListener request to the specified destination that implements an xpDeviceImpl. The listener session and address will be removed from destination related list.
		\param device The request to forward, it contains the listener informations as ip, port and session.
		\return An integer indicating the status of the forwarding, 0 if success.
	*/
	virtual	int removeEventListener(xpMessage device);

private:

	int sendDatagram(sockaddr_in destination, const char* buf);
	static void *handle_requests(void *udpService);
	int socket_id;

#ifdef WIN32
	SOCKET ReceiveSock;
#endif

	int counter;

	time_t lastCheck;

    typedef struct {
        u_int16_t total_packets;
        u_int16_t packet_counter;
        u_int32_t ttl;
        char * * packets;
    } Datagram_ctrl;

    static std::map <std::string, Datagram_ctrl *> _datagram_map;

	void checkCounter();

};

#endif /* XPUDPSERVICE_H_ */
