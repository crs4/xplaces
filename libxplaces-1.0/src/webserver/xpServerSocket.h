/*
 * xpServerSocket.h
 *
 *  Created on: Jun 11, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef WIN32

#ifndef xpServerSocket_class
#define xpServerSocket_class

#include "xpSocket.h"

//!  xpServerSocket is an xPlaces web server.
/*!
  xpServerSocket defines the methods to handle the request and send the related responses over xPlaces web services.
 */
class xpServerSocket : public xpSocket
{
public:
	/*! xpServerSocket ( std::string address, int port )
		\brief Constructor for the xpServerSocket class.
		\details It builds an xpServerSocket from specified host and port.
		\param address The address to bind as std::string.
		\param port The port to bind as std::string.
		\return An instanced xpServerSocket object.
	 */
	xpServerSocket ( std::string address, int port );

	/*! xpServerSocket()
		\brief Constructor for the xpServerSocket class.
		\details It builds an xpServerSocket.
		\return An instanced xpServerSocket object.
	 */
	xpServerSocket () {};

    /*! ~xpServerSocket()
	    \brief Destructor for the xpServerSocket class.
	*/
	virtual ~xpServerSocket();

	/*! fn const xpServerSocket& operator << ( const std::string& ) const
	    \brief Operator to send responses to the client.
		\details This operator sends a std::string as response to a related request.
	*/
	const xpServerSocket& operator << ( const std::string& ) const;

	/*! fn const xpServerSocket& operator >> ( std::string& ) const
	    \brief Operator to obtain requests from the client.
		\details This operator obtains a std::string as request from the client.
	*/
	const xpServerSocket& operator >> ( std::string& ) const;

	/*! fn void accept ( xpServerSocket& )
	    \brief It accepts a connection from the client.
		\details This method accept a pending connection with a client.
	*/
	void accept ( xpServerSocket& );
};

#endif

#endif //WIN32
