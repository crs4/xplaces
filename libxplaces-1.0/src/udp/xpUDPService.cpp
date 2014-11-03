/*
 * xpUDPService.cpp
 *
 *  Created on: Jul 6, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpUDPService.h"
#include "../xpNetwork.h"
#include <iostream>
#include <sstream>
#include <algorithm>


#define BUFLEN 512

using namespace std;

#include <sys/timeb.h>

unsigned int getSeconds(){
    timeb tb;
    ftime(&tb);
    unsigned int nCount = tb.time & 0xfffff;
    return nCount;
}

std::map<std::string, xpUDPService::Datagram_ctrl *> xpUDPService::_datagram_map = std::map<std::string, xpUDPService::Datagram_ctrl *>();

xpUDPService::xpUDPService(std::string ip_port) {


	xpAddress myAddress(ip_port, "");

	sockaddr_in address = myAddress.toSocketAddress();

	counter = 0;
	lastCheck = time(NULL);


	fprintf(stdout,"xpNetwork runs on %s\n", xpNetwork::getInstance()->getIpPort().c_str());
	fprintf(stdout,"UDP Server runs on %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

#ifdef WIN32

	WSADATA WinsockData;
	fprintf(stdout,"Starting up WinSock Service\n");
	if (WSAStartup(MAKEWORD(2, 2), &WinsockData) != 0) {
		        std::cout << "Failed to find Winsock 2.2!" << std::endl;
		        return;
		    }
	this->ReceiveSock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, 0);
	if (ReceiveSock == INVALID_SOCKET) {
		std::cout << "Failed to get a socket Receiver. Error " << WSAGetLastError() <<std::endl;
		return ;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(ReceiveSock, (SOCKADDR *) &address, sizeof(address));

#else
	if ((socket_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))== -1) {
		 perror("error while opening socket");
		 exit(1);
    }
    if (::bind(socket_id, (struct sockaddr *) &address, sizeof(address)) == -1) {
			 perror("error while binding");
			 exit(1);
		}
#endif

	 pthread_t tid;
	 pthread_create(&tid, NULL, (void*(*)(void*))handle_requests, (void*)this);
}

xpUDPService::~xpUDPService() {
	// TODO Auto-generated destructor stub
}

void *xpUDPService::handle_requests(void *p) {


	xpUDPService *service = (xpUDPService*) p;

    unsigned int clock_counter = 0;

	while(true) {
		sockaddr remote;
		socklen_t address_length = sizeof(remote);
        char  * packet = (char * )malloc(4096);

#ifdef WIN32

		int recBytes;
        if(recBytes = recvfrom(service->ReceiveSock, packet, 4096, 0, (SOCKADDR *)&remote,
			&address_length)!= 0){
#else
        if (recvfrom(service->socket_id, packet, 4096, 0, &remote, &address_length) != -1) {
#endif

            uint32_t datagram_id = xpToolSet::toLocalEndianess(*((uint32_t *)packet));
            u_int16_t total_packets = xpToolSet::toLocalEndianess(*((u_int16_t *)(packet + 4)));
            u_int16_t current_packet = xpToolSet::toLocalEndianess(*((u_int16_t *)(packet + 6)));

            std::stringstream ss;
            std::string datagram_id_string;
            ss << datagram_id;
            datagram_id_string = ss.str();

            clock_counter++;

            if(clock_counter%1000==0)
            {
                std::map<std::string, xpUDPService::Datagram_ctrl *>::iterator it = _datagram_map.begin();
                unsigned int time = getSeconds();

               unsigned int deleted = 0;
                while(it != _datagram_map.end())
                {
                    if(it->second->ttl < time - 5)
                    {
                        Datagram_ctrl * d_ctrl = it->second;

                        for(int i = 0; i < d_ctrl->total_packets; i++)
                        {
                            free(d_ctrl->packets[i]);
                        }

                        free(d_ctrl->packets);
                        free(d_ctrl);

                        _datagram_map.erase(it++);

                        deleted++;
                    }
                    else
                    {
                        it++;
                    }

                }

                if(deleted > 0)
                {
                    fprintf(stderr, "Deleted %d packets, %ld still in memory, average: %.6f\n", deleted, _datagram_map.size(), deleted/1000.0);
                }

            }

            if(total_packets == 1)
            {
                xpPropertyList pl(packet + 8);
                xpNetwork::getInstance()->dispatcher(pl);
                free(packet);
            }
            else
            {
                if(_datagram_map.count(datagram_id_string) == 0)
                {
                    Datagram_ctrl * d_ctrl = (Datagram_ctrl *)malloc(sizeof(Datagram_ctrl));
#ifdef _DEBUG
                    if(packets == NULL)
                    {
                        fprintf(stderr, "Out of memory, malloc failed ... exiting ...\n");
                        exit(1);
                    }
#endif
                    d_ctrl->packet_counter = 1;
                    d_ctrl->total_packets = total_packets;

                    char * * packets = (char * *)malloc(sizeof(char *) * total_packets);
#ifdef _DEBUG
                    if(packets == NULL)
                    {
                        fprintf(stderr, "Out of memory, malloc failed ... exiting ...\n");
                        exit(1);
                    }
#endif
                    memset(packets,0,sizeof(char *) * total_packets);

                    d_ctrl->packets = packets;
                    d_ctrl->packets[current_packet] = packet;
                    d_ctrl->ttl = getSeconds();

                    _datagram_map[datagram_id_string] = d_ctrl;
                }
                else
                {
                    Datagram_ctrl * d_ctrl = _datagram_map[datagram_id_string];
                    if(d_ctrl->packets[current_packet] == 0)
                    {

                        d_ctrl->packet_counter++;
                        d_ctrl->packets[current_packet] = packet;
                        d_ctrl->ttl = getSeconds();
                    }

                    if(d_ctrl->packet_counter == d_ctrl->total_packets)
                    {
                        uint32_t round_lenght = 4088 * total_packets;
                        char  * datagram = (char * )malloc(round_lenght);
#ifdef _DEBUG
                    if(packets == NULL)
                    {
                        fprintf(stderr, "Out of memory, malloc failed ... exiting ...\n");
                        exit(1);
                    }
#endif
                        char * datagram_packet_pointer = datagram;

                        for(int i = 0; i < total_packets; i++)
                        {
                            memcpy(datagram_packet_pointer, d_ctrl->packets[i] + 8, 4088);
                            datagram_packet_pointer += 4088;

                            free(d_ctrl->packets[i]);
                        }

                        xpPropertyList pl(datagram);
                        xpNetwork::getInstance()->dispatcher(pl);

                        free(datagram);
                        free(d_ctrl->packets);
                        free(d_ctrl);

                        _datagram_map.erase(datagram_id_string);

                        fflush(stdout);
                    }
                }
            }

		}
	}

	return NULL;
}

int xpUDPService::announce(xpMessage device){
	device.setMessageType(XP_ANNOUNCE);
	xpAddress address(device.getRecipientIpPort(), device.getRecipientSession());
	sockaddr_in sock_in = address.toSocketAddress();

	char *datagram = device.toByteArray();
	int retval = sendDatagram(sock_in, datagram);

	delete datagram;
	return retval;
}

int xpUDPService::action(xpMessage action){
	action.setMessageType(XP_ACTION);
	xpAddress address(action.getRecipientIpPort(), action.getRecipientSession());
	sockaddr_in sock_in = address.toSocketAddress();

	char *datagram = action.toByteArray();
	int retval = sendDatagram(sock_in, datagram);

	delete datagram;
	return retval;
}

int xpUDPService::event(xpMessage event){
	event.setMessageType(XP_EVENT);
	xpAddress address(event.getRecipientIpPort(), event.getRecipientSession());
	sockaddr_in sock_in = address.toSocketAddress();

	char *datagram = event.toByteArray();
	int retval = sendDatagram(sock_in, datagram);

	delete datagram;
	return retval;
}

int xpUDPService::addEventListener(xpMessage device, long type){
	device.setMessageType(XP_ADD_LISTENER);
	xpAddress address(device.getRecipientIpPort(), device.getRecipientSession());
	sockaddr_in sock_in = address.toSocketAddress();

	char *datagram = device.toByteArray();
	int retval = sendDatagram(sock_in, datagram);

	delete datagram;
	return retval;
}

int xpUDPService::removeEventListener(xpMessage device){
	device.setMessageType(XP_REMOVE_LISTENER);
	xpAddress address(device.getRecipientIpPort(), device.getRecipientSession());
	sockaddr_in sock_in = address.toSocketAddress();

	char *datagram = device.toByteArray();
	int retval = sendDatagram(sock_in, datagram);

	delete datagram;
	return retval;
}


int xpUDPService::sendDatagram(struct sockaddr_in destination, const char* datagram) {
	int slen = sizeof(destination);

    uint32_t length_endian = *((uint32_t*)datagram);

    uint32_t h_size = 2 * sizeof(uint32_t);

    char * packet;

    uint32_t length = xpToolSet::toLocalEndianess(length_endian);
    uint32_t datagram_id = xpToolSet::toStdEndianess((uint32_t)random());
    u_int16_t num_packets = length / 4088 + 1;
    u_int16_t current_packet = 0;

    uint32_t current_pointer = 0;

    while(current_packet < num_packets)
    {
        uint32_t size_of_packet = length < 4088 ? length : 4088;
        size_of_packet += h_size;

        packet = (char *)malloc(size_of_packet);

        //copy datagram
        memcpy(packet + h_size, datagram + current_pointer, size_of_packet - h_size);

        //copy id
        memcpy(packet, &datagram_id, sizeof(uint32_t));
        //copy num_packets
        u_int16_t num_packets_endian = xpToolSet::toStdEndianess(num_packets);
        memcpy(packet + 4, &num_packets_endian, sizeof(u_int16_t));

        //copy current packet counter
        u_int16_t current_packet_endian = xpToolSet::toStdEndianess(current_packet);
        memcpy(packet + 6, &current_packet_endian, sizeof(u_int16_t));

        current_pointer += (size_of_packet - h_size);
        current_packet++;


#ifdef WIN32
        SOCKET SendSock;
        WSADATA WinsockData;
        if (WSAStartup(MAKEWORD(2, 2), &WinsockData) != 0) {
                    std::cout << "Failed to find Winsock 2.2!" << std::endl;
                    return -1;
                }
        SendSock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);
        if (SendSock == INVALID_SOCKET) {
            std::cout << "Failed to get a socket Sender. Error " << WSAGetLastError() <<std::endl;
            return -1;
        }

        if (sendto(SendSock, packet, size_of_packet, 0, (struct sockaddr *) &destination, slen) == -1) {
                fprintf(stdout, "Error while sending datagram\n");
                return -2;
            }

        closesocket(SendSock);

#else
        int s = sizeof(destination);
        if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {

            fprintf(stderr, "Error while opening socket\n");
            return -1;
        }

        if (sendto(s, packet, size_of_packet, 0, (struct sockaddr *) &destination, slen) == -1) {
            fprintf(stderr, "Error while sending datagram\n");
            return -2;
        }

        close(s);
#endif

        free(packet);
    }

	return 0;
}

void xpUDPService::checkCounter() {
	counter++;
	double diff = difftime(time(NULL), lastCheck) ;
	if (diff > 2.0) {
		fprintf(stderr, "Events forwarded/sec %f \n", counter/diff);

		lastCheck = time(NULL);
		counter = 0;
	}
}


