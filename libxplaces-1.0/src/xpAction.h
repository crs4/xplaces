/*
 * xpAction.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPACTION_H_
#define XPACTION_H_
#include <vector>
#include "xpMessage.h"

//!  xpAction is a properties list that brings informations concerning actions.
/*!
  An xpAction is typically a request, sent through an xpDeviceProxy, to an xpDeviceImpl. The class inherits the base skeleton from xpMessage, that implements information about the request sender and the request recipient.
*/
class xpAction : public xpMessage {
public:
	/*! xpAction()
	    \brief Constructor for the xpAction class.
		\details Base constructor for the xpAction class, the action will be instanced without properties or other informations.
		\return An empty xpAction object
	*/
	xpAction();

	/*! xpAction(std::string action_type, std::string action_name, std::string destination, std::vector<xpProperty> action_properties)
	    \brief Constructor for the xpAction class.
	    \param action_type The type of the action as std::string.
	    \param action_name The name of the action as std::string.
	    \param destination The xpDeviceImpl destination of the action as ip-port address, formatted as "http://ip:port".
	    \param action_properties The other properties of the action as std::vector\<xpProperty\>.
		\details Explicit constructor for the xpAction class, used to keep control of the action typical informations.
		\return A instanced xpAction object
	*/
	xpAction(std::string action_type, std::string action_name, std::string destination, std::vector<xpProperty> action_properties);

	/*! xpAction(long int action_type, xpPropertyList properties)
	    \brief Constructor for the xpAction class.
	    \param action_type The type of the action as long int.
	    \param properties The properties of the action as xpPropertyList.
		\details Constructor for the xpAction class, it explicit the type and the properties field, used at the action creation.
		\return A instanced xpAction object
	*/
	xpAction(long int action_type, xpPropertyList properties);

	/*! xpAction(xpPropertyList properties)
	    \brief Constructor for the xpAction class.
	    \param properties The source as xpPropertyList.
		\details Constructor for the xpAction class, it builds an xpAction from a source xpPropertyList.
		\return A instanced xpAction object
	*/
	xpAction(xpPropertyList properties);

	/*! xpAction(xpMessage properties)
	    \brief Constructor for the xpAction class.
	    \param properties The source as xpMessage.
		\details Constructor for the xpAction class, it rebuilds an xpAction from a received xpMessage.
		\code
		//Example of xpAction rebuilding from an received xpMessage
		xpMessage message;
		xpAction * newAction = new xpAction(message);
		\endcode
		\return A instanced xpAction object
	*/
	xpAction(xpMessage properties);

	/*! ~xpAction()
	    \brief Destructor for the xpAction class.
	*/
	virtual ~xpAction();

	/*! \fn long int getType()
	    \brief It returns the action type.
		\details It extracts the action type from the properties list.
		\return The xpAction type as long int.
	*/
	long int getType(){
		return getLongIntProperty("action_type");
	}

	/*! \fn void setType(long int value)
	    \brief It sets the action type.
	    \param value The action type value to set as a long int.
		\details It sets the action type property in the property list.
	*/
	void setType(long int value) {
		setProperty("action_type", value);
	}
};

#endif /* XPACTION_H_ */
