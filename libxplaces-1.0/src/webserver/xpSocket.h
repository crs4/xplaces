/*
 * xpSocket.h
 *
 *  Created on: Jun 11, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef xpSocket_class
#define xpSocket_class


#include <string>
#include <stdio.h>


#ifndef WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifndef WIN32
const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 410398 * 4;

//!  xpSocket implements xPlaces web services.
/*!
  xpSocket defines the methods to handle the create a web server, handle requests/responses and send requests/responses over xPlaces web services.
 */
class xpSocket {
public:
	/*! xpSocket()
		\brief Constructor for the xpSocket class.
		\details It builds a xpSocket.
		\return An instanced xpSocket object.
	 */
	xpSocket();

    /*! ~xpSocket()
	    \brief Destructor for the xpSocket class.
	*/
	virtual ~xpSocket();

	/*! fn bool create()
	    \brief It initializes a xpSocket server object.
		\details It initializes a xpSocket object to realise a server node.
		\return True is initialized.
	*/
	bool create();

	/*! fn bool bind (const std::string address, const int port )
	    \brief It binds an address.
	    \param address The address to bind as std::string formatted as dotted.
		\param port The port to bind as int
		\details It binds an address to listen/send informations.
		\return True is successful.
	*/
	bool bind (const std::string address, const int port );

	/*! fn bool listen() const
	    \brief It listens for connection.
		\details It listens for connection on the binded address.
		\return True is successful.
	*/
	bool listen() const;

	/*! fn bool accept ( xpSocket& ) const
	    \brief It accepts a new connection.
		\details It accepts connections on the binded address.
		\return True is successful.
	*/
	bool accept ( xpSocket& ) const;

	/*! fn bool connect ( const std::string host, const int port )
	    \brief It initializes a xpSocket client object.
	    \param host The address as std::string formatted as dotted.
		\param port The port as int
		\details It initializes a xpSocket object to realise a client node.
		\return True is initialized.
	*/
	bool connect ( const std::string host, const int port );

	/*! fn bool send ( const std::string data ) const
	    \brief It sends informations.
	    \param data The information to forward as std::string.
		\details It sends informations, contained in a std::string, as request/responses.
		\return True is initialized.
	*/
	bool send ( const std::string data ) const;

	/*! fn bool send ( const char*, int) const
	    \brief It sends informations.
	    \param data The information to forward as char *.
		\param size The data size as int.
		\details It sends informations, pointed by a char *, as request/responses.
		\return True is initialized.
	*/
	bool send ( const char* data, int size) const;

	/*! fn int recv ( std::string& ) const
	    \brief It receives informations.
		\details It receives informations.
		\return It returns 0 is successful.
	*/
	int recv ( std::string& ) const;

	/*! fn void set_non_blocking ( const bool value )
	    \brief It sets the non-blocking behaviour.
	    \param value The boolean value for non-blocking behaviour.
		\details It sets the non-blocking behaviour by looking at boolean value.
	*/
	void set_non_blocking ( const bool value);

	/*! fn bool is_valid() const
	    \brief It gives informations about socket validity.
		\details It gives informations about socket validity.
		\return It returns true is successful.
	*/
	bool is_valid() const {
		return m_sock != -1;
	}

private:

	int m_sock;
	sockaddr_in m_addr;


};
#endif //WIN32


#endif
