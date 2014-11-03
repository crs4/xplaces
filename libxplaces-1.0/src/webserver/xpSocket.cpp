/*
 * Socket.cpp
 *
 *  Created on: Jun 11, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpSocket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <sstream>

#ifndef WIN32
#include <ifaddrs.h>
#include <netdb.h>
#include <sys/param.h>     // only needed for definition of MAXHOSTNAMELEN for part 1
#endif
#include <stdio.h>

#ifndef WIN32
xpSocket::xpSocket() :
  m_sock ( -1 )
{

  memset ( &m_addr,
	   0,
	   sizeof ( m_addr ) );

}

xpSocket::~xpSocket()
{
  if ( is_valid() )
    ::close ( m_sock );
}

bool xpSocket::create()
{
  m_sock = socket ( AF_INET,
		    SOCK_STREAM,
		    0 );

  if ( ! is_valid() )
    return false;


  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;


  return true;

}



bool xpSocket::bind (const std::string address, const int port )
{

	if ( ! is_valid() )
	{
		return false;
	}

	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = inet_addr(address.c_str());
	m_addr.sin_port = htons ( port );

	int bind_return = ::bind ( m_sock, ( struct sockaddr * ) &m_addr,  sizeof ( m_addr ) );


	if ( bind_return == -1 )
	{
		return false;
	}

	return true;
}


bool xpSocket::listen() const
{
  if ( ! is_valid() )
    {
      return false;
    }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );


  if ( listen_return == -1 )
    {
      return false;
    }

  return true;
}


bool xpSocket::accept ( xpSocket& new_socket ) const
{
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
  else
    return true;
}

bool xpSocket::send ( const std::string s ) const
{


	std::string retVal;

	retVal.append("HTTP/1.1 200 OK Date: Thu, 10 Jun 2010 12:07:31 GMT\nServer: Simple Server\n");
	retVal.append("Last-Modified: Tue, 16 Jul 2002 14:39:24 GMT\nETag: \"1190091-a1d-93173f00\"\n");
	retVal.append("Accept-Ranges: bytes\nContent-Length: ");

	std::ostringstream v;
	v << s.size();
	retVal.append(v.str());


	retVal.append("\nCache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0");
	retVal.append("\nPragma: no-cache\nConnection: close\nContent-Type: text/html\n\n");
	retVal.append(s);

	std::cout << "Size " << s.size() << std::endl;

#ifdef __APPLE__
	int status = ::send ( m_sock, retVal.c_str(), retVal.size(), MSG_HAVEMORE );
#else
	int status = ::send ( m_sock, retVal.c_str(), retVal.size(), MSG_NOSIGNAL );
#endif
	if ( status == -1 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool xpSocket::send (const char *bytes, int size) const
{
	//TODO Send dell'immagine: lavorare su JPEG

	std::string retVal;

	retVal.append("HTTP/1.1 200 OK Date: Thu, 10 Jun 2010 12:07:31 GMT\nServer: Simple Server\n");
//	retVal.append("Content-Type: multipart/x-mixed-replace; boundary=--myboundary");
	retVal.append("\nCache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0");
	retVal.append("\nPragma: no-cache\nConnection: close\n");


//	retVal.append("--myboundary\n");
	retVal.append("Content-Type: image/jpeg\n");
	retVal.append("Content-Length: ");
	std::ostringstream v;
	v << size << "\n\n";
	retVal.append(v.str());

//Check OS type
#ifdef __APPLE__
	::send ( m_sock, retVal.c_str(), retVal.size(), MSG_HAVEMORE );
#else
	::send ( m_sock, retVal.c_str(), retVal.size(), MSG_NOSIGNAL );
#endif

	// try to change to mjpeg
    unsigned char magic[4] = {0xff, 0xd8, 0xff, 0xfe};

#ifdef __APPLE__
	::send (m_sock, magic, 4, MSG_HAVEMORE);
	::send (m_sock, bytes+4, size, MSG_HAVEMORE );
#else
	::send (m_sock, magic, 4, MSG_NOSIGNAL);
	::send (m_sock, bytes+4, size, MSG_NOSIGNAL );
#endif

	return true;
}




int xpSocket::recv ( std::string& s ) const
{
  char buf [ MAXRECV + 1 ];

  s = "";

  memset ( buf, 0, MAXRECV + 1 );

  int status = ::recv ( m_sock, buf, MAXRECV, 0 );

  if ( status == -1 )
    {
	  std::cout << "status == -1   errno == " << errno << "  in xpSocket::recv\n";
      return 0;
    }
  else if ( status == 0 )
    {
      return 0;
    }
  else
    {
      s = buf;
      return status;
    }
}



bool xpSocket::connect ( const std::string host, const int port )
{
  if ( ! is_valid() ) return false;

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}

void xpSocket::set_non_blocking ( const bool b )
{

  int opts;

  opts = fcntl ( m_sock,
		 F_GETFL );

  if ( opts < 0 )
    {
      return;
    }

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock,
	  F_SETFL,opts );

}
#endif
