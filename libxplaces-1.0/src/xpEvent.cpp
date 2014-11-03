/*
 * xpEvent.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpEvent.h"

xpEvent::xpEvent() {

}

xpEvent::xpEvent(std::string event_type, std::string event_name, std::string destination,long int mask, std::vector<xpProperty> event_properties)
{
	for (int i = 0; i < (int) event_properties.size(); i++) {
		addProperty(event_properties[i].getKey(), event_properties[i].getValue());
	}

	setMessageType(XP_EVENT);
	setType(atol(event_type.c_str()));
}


xpEvent::xpEvent(long int event_type, xpPropertyList properties) {
	for (int i = 0; i < (int) properties.size(); i++) {
		addProperty(properties[i].getKey(), properties[i].getValue());
	}

	setMessageType(XP_EVENT);
	setType(event_type);
}

xpEvent::xpEvent(xpPropertyList properties) {

	for (int i = 0; i < (int) properties.size(); i++) {
		addProperty(properties[i].getKey(), properties[i].getValue());
	}

}

xpEvent::xpEvent(xpMessage message) {

	for (int i = 0; i < (int) message.size(); i++) {
		addProperty(message[i].getKey(), message[i].getValue());
	}
}

xpEvent::~xpEvent() {
}

//std::vector<struct property> xpEvent::toSoapFormat() {
//	std::vector<struct property> soapProperties = xpPropertyList::toSoapFormat();
//	soapProperties.push_back(xpProperty(std::string("type"), getType()).toSoapFormat());
//	return soapProperties;
//}
