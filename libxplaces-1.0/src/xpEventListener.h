/*
 * xpEventListener.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPEVENTLISTENER_H_
#define XPEVENTLISTENER_H_

#include "xpEvent.h"
#include "xpDescriptor.h"
//#include "xpNode.h"

//!  xpEventListener is a properties list that brings informations concerning event listeners.
/*!
  An xpEventListener is a event listener template, implemented by xpEventListenerProxy and xpEventListenerImpl. The class inherits the base skeleton from xpMessage, that implements information about the request sender and the request recipient.
*/
class xpEventListener : public xpMessage {
public:
	/*! xpEventListener()
	    \brief Constructor for the xpEventListener class.
		\details Base constructor for the xpEventListener class, the event will be instanced without properties or other informations.
		\return An empty xpEventListener object
	*/
	xpEventListener();

	/*! ~xpEventListener()
	    \brief Destructor for the xpEventListener class.
	*/
	virtual ~xpEventListener();

	/*! \fn bool newEvent(xpEvent * event)
	    \brief Pure virtual method for the event forwarding/receiving.
		\param event The event to forward if this method is implemented by the xpEventListenerProxy or the received event if this method is implemented by the xpEventListenerImpl.
		\details This method implements the event forwarding/receiving.
		\return The method must return true if successful.
	*/
	virtual bool newEvent(xpEvent * event)=0;

	/*! \fn long getEventTypeMask()
	    \brief It returns the event type mask.
		\details The method returns the event type mask of the listener as long int.
		\return The event type mask of the listener as long int.
	*/
	long getEventTypeMask() {
		return getLongIntProperty("eventTypeMask");
	}

	/*! \fn void setEventTypeMask(long value)
	    \brief It sets the event type mask.
	    \param value The event type mask to set in the properties list.
		\details The method sets the event type mask in the properties list as long int.
	*/
    void setEventTypeMask(long value)
    {
    	setProperty("eventTypeMask", value);
    }

	/*! \fn long int getTimestamp()
	    \brief It returns the timestam.
		\details It extracts the timestamp from the properties list.
		\return The xpAliveAction timestamp as long int.
	*/
	long int getTimestamp() {
		return getLongIntProperty("timestamp");
	}

	/*! \fn void setTimestamp()
	    \brief It sets the timestamp.
		\details The method sets the timestamp in the properties list as long int.
	*/
	void setTimestamp() {
		setProperty("timestamp", time(NULL));
	}

	friend class xpDeviceProxy;
};

#endif /* XPEVENTLISTENER_H_ */
