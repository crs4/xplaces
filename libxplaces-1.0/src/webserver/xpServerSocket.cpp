/*
 * xpServerSocket.cpp
 *
 *  Created on: Jun 11, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef WIN32

#include "xpServerSocket.h"
#include "xpSocketException.h"

xpServerSocket::xpServerSocket (std::string address, int port )
{
  if ( ! xpSocket::create() )
    {
      throw xpSocketException ( "Could not create server socket." );
    }

  if ( ! xpSocket::bind (address, port ) )
    {
      throw xpSocketException ( "Could not bind to port." );
    }

  if ( ! xpSocket::listen() )
    {
      throw xpSocketException ( "Could not listen to socket." );
    }

}

xpServerSocket::~xpServerSocket()
{
}


const xpServerSocket& xpServerSocket::operator << ( const std::string& s ) const
{
  if ( ! xpSocket::send ( s ) )
    {
      throw xpSocketException ( "Could not write to socket." );
    }

  return *this;

}


const xpServerSocket& xpServerSocket::operator >> ( std::string& s ) const
{
  if ( ! xpSocket::recv ( s ) )
    {
      throw xpSocketException ( "Could not read from socket." );
    }

  return *this;
}

void xpServerSocket::accept ( xpServerSocket& sock )
{
  if ( ! xpSocket::accept ( sock ) )
    {
      throw xpSocketException ( "Could not accept socket." );
    }
}

#endif //WIN32
