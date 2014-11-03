/*
 * xpSocketException.h
 *
 *  Created on: Jun 11, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */


#ifndef xpSocketException_class
#define xpSocketException_class

#include <string>

//!  xpSocketException implements socket exceptions over xPlaces web services.
/*!
  xpSocketException implements the methods to inspect socket exceptions over xPlaces web .
 */
class xpSocketException
{
public:
	/*! xpSocketException ( std::string s )
		\brief Constructor for the xpSocketException class.
		\details It builds a xpSocketException.
		\return An instanced xpSocketException object.
	 */
	xpSocketException ( std::string s ) : m_s ( s ) {};

    /*! ~xpSocketException()
	    \brief Destructor for the xpSocketException class.
	*/
	~xpSocketException (){};

	/*! fn std::string description()
	    \brief It returns the exception description.
		\details It returns the exception description as std::string.
		\return The exception description as std::string.
	 */
	std::string description() {
		return m_s;
	}

private:
	std::string m_s;
};

#endif
