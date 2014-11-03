/*
 * xpClientSocket.cpp
 *
 *  Created on: Jun 11, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef WIN32

#include "xpClientSocket.h"
#include "xpSocketException.h"


xpClientSocket::xpClientSocket ( std::string host, int port )
{
  if ( ! xpSocket::create() )
    {
      throw xpSocketException ( "Could not create client socket." );
    }

  if ( ! xpSocket::connect ( host, port ) )
    {
      throw xpSocketException ( "Could not bind to port." );
    }

}


const xpClientSocket& xpClientSocket::operator << ( const std::string& s ) const
{
  if ( ! xpSocket::send ( s ) )
    {
      throw xpSocketException ( "Could not write to socket." );
    }

  return *this;

}


const xpClientSocket& xpClientSocket::operator >> ( std::string& s ) const
{
  if ( ! xpSocket::recv ( s ) )
    {
      throw xpSocketException ( "Could not read from socket." );
    }

  return *this;
}

#endif //WIN32
