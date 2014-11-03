/*
 * xpClientSocket.h
 *
 *  Created on: Jun 11, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef WIN32

#ifndef xpClientSocket_class
#define xpClientSocket_class

#include "xpSocket.h"

//!  xpClientSocket is an interface to xPlaces web services.
/*!
  xpClientSocket defines the methods to obtain and send information over xPlaces web services.
 */
class xpClientSocket : private xpSocket
{
public:
	/*! xpClientSocket ( std::string host, int port )
		\brief Constructor for the xpClientSocket class.
		\details It builds an xpClientSocket from specified host and port.
		\param host The address to bind as std::string.
		\param port The port to bind as std::string.
		\return An instanced xpClientSocket object.
	 */
	xpClientSocket ( std::string host, int port );

    /*! ~xpClientSocket()
	    \brief Destructor for the xpClientSocket class.
	*/
	virtual ~xpClientSocket(){};

	/*! fn const xpClientSocket& operator << ( const std::string& )
	    \brief Operator to send requests to the webserver.
		\details This operator sends a std::string as request.
	*/
	const xpClientSocket& operator << ( const std::string& ) const;

	/*! fn const xpClientSocket& operator >> ( std::string& ) const
	    \brief Operator to obtain responses from the webserver.
		\details This operator obtains a std::string as response.
	*/
	const xpClientSocket& operator >> ( std::string& ) const;

};

#endif

#endif //WIN32
