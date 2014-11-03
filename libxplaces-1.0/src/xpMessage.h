/*
 * xpMessage.h
 *
 *  Created on: May 21, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPMESSAGE_H_
#define XPMESSAGE_H_

#include "xpAddress.h"
#include "xpPropertyList.h"

//!  It holds the network endpoint informations.
/*!
  An xpMessage keep informations to and from the xplaces network as sender and recipient.
*/
class xpMessage : public xpPropertyList {
public:
	/*! xpMessage()
	    \brief Constructor for the xpMessage class.
		\details Base constructor for the xpMessage class, the message will be instanced without properties or other informations.
		\return An empty xpMessage object
	*/
	xpMessage();

	/*! xpMessage(xpPropertyList properties)
	    \brief Constructor for the xpMessage class.
		\details It rebuilds an xpMessage from an xpPropertyList.
		\code
		//Example of xpMessage rebuilding from an received xpPropertyList
		xpPropertyList properties;
		xpMessage * xpMessage = new xpMessage(properties);
		\endcode
		\param properties The source xpPropertyList
		\return An instanced xpMessage object
	*/
	xpMessage(xpPropertyList properties);

	/*! ~xpMessage()
	    \brief Destructor for the xpMessage class.
	*/
	virtual ~xpMessage();

	/*! \fn void setSenderSession(std::string value)
	    \brief It sets the sender session.
		\details It sets the sender session indicated by the param.
		\param value The value to set as sender session.
	*/
    void setSenderSession(std::string value)
    {
    	setProperty("sender_session", value);
    }

	/*! \fn void getSenderSession()
	    \brief It returns the sender session.
		\details It returns the sender session active at related address.
		\return The session as std::string
	*/
    std::string getSenderSession() {
    	return getStringProperty("sender_session");
    }

	/*! \fn void setSenderIpPort(std::string value)
	    \brief It sets the sender address.
		\details It sets the sender address indicated by the param ad std::string formatted ad "http://ip:port".
		\param value The value to set as sender address.
	*/
    void setSenderIpPort(std::string value)
    {
    	setProperty("sender_ip_port", value);
    }

	/*! \fn void getSenderIpPort()
	    \brief It returns the sender address.
		\details It returns the sender address formatted ad "http://ip:port".
		\return The address as std::string formatted as "http://ip:port"
	*/
    std::string getSenderIpPort() {
    	return getStringProperty("sender_ip_port");
    }

	/*! \fn void setRecipientSession(std::string value)
	    \brief It sets the recipient session.
		\details It sets the recipient session indicated by the param.
		\param value The value to set as recipient session.
	*/
    void setRecipientSession(std::string value)
    {
    	setProperty("recipient_session", value);
    }

	/*! \fn void getRecipientSession()
	    \brief It returns the recipient session.
		\details It returns the recipient session active at related address.
		\return The session as std::string
	*/
    std::string getRecipientSession() {
    	return getStringProperty("recipient_session");
    }

	/*! \fn void setRecipientIpPort(std::string value)
	    \brief It sets the recipient address.
		\details It sets the recipient address indicated by the param ad std::string formatted ad "http://ip:port".
		\param value The value to set as recipient address.
	*/
    void setRecipientIpPort(std::string value)
    {
    	setProperty("recipient_ip_port", value);
    }

	/*! \fn void getRecipientIpPort()
	    \brief It returns the recipient address.
		\details It returns the recipient address formatted ad "http://ip:port".
		\return The address as std::string formatted as "http://ip:port"
	*/
    std::string getRecipientIpPort() {
    	return getStringProperty("recipient_ip_port");
    }

	/*! \fn int getMessageType()
	    \brief It returns the message type.
		\details It returns the message type as integer.
		\return An integer from valid message types list
	*/
	int getMessageType(){
		return getIntProperty("message_type");
	};

	/*! \fn void setMessageType(int value)
	    \brief It sets the message type.
		\details It sets the message type using the param.
		\param value The value as integer to set as message type.
	*/
	void setMessageType(int value) {
		setProperty("message_type", value);
	}

	/*! \fn std::string getSenderType()
	    \brief It returns the message sender type.
		\details It returns the message sender type as std::string.
		\return A std::string from valid message sender types list
	*/
	std::string getSenderType() {
		return getStringProperty("sender_type");
	}

	/*! \fn void setSenderType(std::string value)
	    \brief It sets the message sender type.
		\details It sets the message sender type using the param.
		\param value The value as std::string to set as message sender type.
	*/
	void setSenderType(std::string value) {
		setProperty("sender_type", value);
	}

	/*! \fn std::string getSenderName()
	    \brief It returns the message sender name.
		\details It returns the message sender name as std::string.
		\return A std::string as sender name.
	*/
	std::string getSenderName() {
		return getStringProperty("sender_name");
	}

	/*! \fn void setSenderName(std::string value)
	    \brief It sets the message sender name.
		\details It sets the message sender name using the param.
		\param value The value as std::string to set as message sender name.
	*/
	void setSenderName(std::string value) {
		setProperty("sender_name", value);
	}

};

#endif /* XPMESSAGE_H_ */
