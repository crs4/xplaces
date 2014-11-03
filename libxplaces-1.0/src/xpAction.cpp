/*
 * xpAction.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpAction.h"

xpAction::xpAction()
{

}

xpAction::xpAction(std::string action_type, std::string action_name, std::string destination, std::vector<xpProperty> action_properties)
{
	setType(atol(action_type.c_str()));


	for (int i = 0; i < (int) action_properties.size(); i++) {
		setProperty(action_properties[i].getKey(), action_properties[i].getValue());
	}
}

xpAction::xpAction(long int action_type, xpPropertyList properties) {
	setType(action_type);

	for (int i = 0; i < (int) properties.size(); i++) {
		setProperty(properties[i].getKey(), properties[i].getValue());
	}
}

xpAction::xpAction(xpPropertyList properties) {
	for (int i = 0; i < (int) properties.size(); i++) {
		setProperty(properties[i].getKey(), properties[i].getValue());
	}
}

xpAction::xpAction(xpMessage message) {
	for (int i = 0; i < (int) message.size(); i++) {
		setProperty(message[i].getKey(), message[i].getValue());
	}
}

xpAction::~xpAction() {
}



