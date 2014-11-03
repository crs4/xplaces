/*
 * xpEventListenerProxy.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpEventListenerProxy.h"
#include "udp/xpUDPService.h"
#include "xpNetwork.h"

xpEventListenerProxy::xpEventListenerProxy(long int type, std::string ip_port, std::string session) {
	setSenderIpPort(ip_port);
	setSenderSession(session);
	setEventTypeMask(type);
}

xpEventListenerProxy::xpEventListenerProxy(xpPropertyList properties) {
	for(int i = 0; i < (int)properties.size(); i++) {
		addProperty(properties[i].getKey(), properties[i].getValue());
	}
}

xpEventListenerProxy::~xpEventListenerProxy() {
}

bool xpEventListenerProxy::newEvent(xpEvent *eventToSend) {

	if(xpNetwork::getInstance()->getDeliveryService()->event(*eventToSend) == 0)
		return true;
	else
		return false;
}
