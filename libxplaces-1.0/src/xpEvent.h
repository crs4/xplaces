/*
 * xpEvent.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPEVENT_H_
#define XPEVENT_H_
#include <vector>
#include "xpMessage.h"

//!  xpEvent is a properties list that brings informations concerning events.
/*!
  An xpEvent is typically a response to a request, sent by an xpDeviceImpl, through an xpEventListenerProxy, to an xpEventListenerImpl. The class inherits the base skeleton from xpMessage, that implements information about the request sender and the request recipient.
*/
class xpEvent : public xpMessage {
public:
	/*! xpEvent()
	    \brief Constructor for the xpEvent class.
		\details Base constructor for the xpEvent class, the event will be instanced without properties or other informations.
		\return An empty xpEvent object
	*/
	xpEvent();

	/*! xpEvent(std::string event_type, std::string event_name, std::string destination, long int mask, std::vector<xpProperty> event_properties)
	    \brief Constructor for the xpEvent class.
	    \param event_type The type of the event as std::string.
	    \param event_name The name of the event as std::string.
	    \param destination The xpEventListenerImpl destination of the event as ip-port address, formatted as "http://ip:port".
	    \param mask The event type mask as long int.
	    \param event_properties The other properties of the event as std::vector\<xpProperty\>.
		\details Explicit constructor for the xpEvent class, used to keep control of the event typical informations.
		\return A instanced xpEvent object
	*/
	xpEvent(std::string event_type, std::string event_name, std::string destination, long int mask, std::vector<xpProperty> event_properties) ;

	/*! xpEvent(long int event_type, xpPropertyList properties)
	    \brief Constructor for the xpAction class.
	    \param event_type The type of the event as long int mask.
	    \param properties The properties of the event as xpPropertyList.
		\details Constructor for the xpEvent class, it explicit the type and the properties field, used at the event creation.
		\return A instanced xpEvent object
	*/
	xpEvent(long int event_type, xpPropertyList properties);

	/*! xpEvent(xpPropertyList properties)
	    \brief Constructor for the xpEvent class.
	    \param properties The source as xpPropertyList.
		\details Constructor for the xpEvent class, it builds an xpEvent from a source xpPropertyList.
		\return A instanced xpEvent object
	*/
	xpEvent(xpPropertyList properties);

	/*! xpEvent(xpMessage message)
	    \brief Constructor for the xpEvent class.
	    \param message The source as xpMessage.
		\details Constructor for the xpEvent class, it rebuilds an xpEvent from a received xpMessage.
		\code
		//Example of xpEvent rebuilding from an received xpMessage
		xpMessage message;
		xpEvent * newEvent = new xpEvent(message);
		\endcode
		\return A instanced xpEvent object
	*/
	xpEvent(xpMessage message);

	/*! ~xpEvent()
	    \brief Destructor for the xpEvent class.
	*/
	virtual ~xpEvent();

	/*! \fn long int getType()
	    \brief It returns the event type.
		\details It extracts the event type from the properties list.
		\return The xpEvent type as long int.
	*/
	long int getType(){
		return getLongIntProperty("__event_type");
	};

	/*! \fn void setType(long int value)
	    \brief It sets the event type.
	    \param value The event type value to set as a long int.
		\details It sets the event type property in the property list.
	*/
	void setType(long int value) {
		setProperty("__event_type", value);
	}

private:
	long int event_type;
};

#endif /* XPEVENT_H_ */
