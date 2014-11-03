/*
 * xpEventListenerProxy.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPEVENTLISTENERPROXY_H_
#define XPEVENTLISTENERPROXY_H_

#include "xpEventListener.h"
#include "xpDescriptor.h"

//!  xpEventListenerProxy is an proxy to an xpEventListenerImpl, defined by a properties list that brings informations concerning event listeners.
/*!
  An xpEventListenerProxy is a event listener proxy to an xpEventListenerImpl, registered on a xpDeviceImpl, that forwards events, emitted from related xpDeviceImpl, through the network.
  The class has to implements the pure virtual methods of the superclass.
*/
class xpEventListenerProxy : public xpEventListener {
public:
	/*! xpEventListenerProxy(long int type, std::string ip_port, std::string session)
	    \brief Constructor for the xpEventListenerProxy class.
		\details It instances an xpEventListenerProxy from the specified parameters.
		\param type The type of event listen as long int.
		\param ip_port The address of the listener as std::string formatted as "http://ip:port".
		\param session The session of the event listener.
		\return An instanced xpEventListenerProxy object.
	*/
	xpEventListenerProxy(long int type, std::string ip_port, std::string session);

	/*! xpEventListenerProxy(xpPropertyList properties)
	    \brief Constructor for the xpEventListenerProxy class.
		\details It rebuilds an xpEventListenerProxy from an xpPropertyList.
		\param properties The source xpPropertyList
		\return An instanced xpEventListenerProxy object
	*/
	xpEventListenerProxy(xpPropertyList properties);

	/*! ~xpEventListenerProxy()
	    \brief Destructor for the xpEventListenerProxy class.
	*/
    virtual ~xpEventListenerProxy();

	/*! \fn bool newEvent(xpEvent * event)
	    \brief The method implements the event forwarding.
		\param event The event to forward to an xpEventListenerImpl.
		\details The method implements the event forwarding to an xpEventListenerImpl.
		\return The method returns true if successful.
	*/
    virtual bool newEvent(xpEvent* event);

	/*! \fn std::string getDestinationDevice()
	    \brief The method returns the destination event listener.
		\details The method returns the destination event listener session as std::string.
		\return The method returns the destination listener session as std::string.
	*/
    std::string getDestinationDevice()
    {
        return getStringProperty("destinationDevice");
    }

	/*! \fn void setDestinationDevice(std::string destinationDevice)
	    \brief The method sets the destination event listener.
		\param destinationDevice The session of the destination listener.
		\details The method sets the destination specified by the parameter in the properties list.
	*/
    void setDestinationDevice(std::string destinationDevice)
    {
    	setProperty("destinationDevice", destinationDevice);
    }
};

#endif /* XPEVENTLISTENERPROXY_H_ */
