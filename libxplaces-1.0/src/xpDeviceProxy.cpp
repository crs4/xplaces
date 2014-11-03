/*
 * xpDeviceProxy.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpDeviceProxy.h"
#include "xpEventListenerImpl.h"
#include "xpNetwork.h"

#include <iostream>


xpDeviceProxy::xpDeviceProxy(xpDescriptor * deviceRef) : xpDevice(deviceRef) {
}

xpDeviceProxy::~xpDeviceProxy() {
}

bool xpDeviceProxy::sendAction(xpAction *actionToSend) {

	actionToSend->setRecipientIpPort(getDescriptor()->getSenderIpPort());
	actionToSend->setRecipientSession(getDescriptor()->getSenderSession());
	actionToSend->setSenderType(getDescriptor()->getType());
	actionToSend->setSenderName(getDescriptor()->getName());

    std::cout <<  "sending xpAction " << getDescriptor()->getSenderSession().c_str() << " " << getDescriptor()->getName().c_str() << std::endl;

	if(xpNetwork::getInstance()->getDeliveryService()->action(*actionToSend) == 0)
		return true;
	else
		return false;
}


bool xpDeviceProxy::addEventListener(xpEventListener *listener, long int event_mask) {
	xpEventListenerImpl * listenerImpl = (xpEventListenerImpl *)listener;
	// Add the event type mask to listener
	listenerImpl->setEventTypeMask(event_mask);
	// Add a new session to list, this field will be checked when a new event occurs...
    listenerImpl->addDeviceToListen(getDescriptor());

	listenerImpl->setRecipientIpPort(getDescriptor()->getSenderIpPort());
	listenerImpl->setRecipientSession(getDescriptor()->getSenderSession());

    listenerImpl->setEventTypeMask(event_mask);

	if(getDescriptor())
	{
        if(xpNetwork::getInstance()->getDeliveryService()->addEventListener(*listenerImpl, listenerImpl->getEventTypeMask()) == 0
                && xpNetwork::getInstance()->addToListenerList(listenerImpl)) {
			return true;
		}
		else
			return false;
	}
	else
	{
		fprintf(stderr,"Device Proxy add Listener Error NULL Device Descriptor on %s", listenerImpl->getRecipientSession().c_str());
		return false;
	}
}

bool xpDeviceProxy::removeEventListener(xpEventListener * listener) {
    xpEventListenerImpl * listenerImpl = (xpEventListenerImpl *)listener;
    // Remove a session from the session list in listenerImpl...
    listenerImpl->removeDeviceFromList(getDescriptor()->getSenderSession());

    listenerImpl->setRecipientIpPort(getDescriptor()->getSenderIpPort());
    listenerImpl->setRecipientSession(getDescriptor()->getSenderSession());

    if(getDescriptor())
    {
        if(xpNetwork::getInstance()->getDeliveryService()->removeEventListener(*listenerImpl) == 0) {
            fprintf(stderr, "Removing Listener session %s ", listenerImpl->getSenderSession().c_str());
            //Se questo listener non è più in ascolto su nessun device, allora va rimosso dalle tabelle di xpNetwork
            if (listenerImpl->getDeviceListSize() == 0)
                xpNetwork::getInstance()->removeFromListenerList(listenerImpl);

            return true;
        }
        else
            return false;
    }

    else
    {
        fprintf(stderr,"Device Proxy remove Listener Error NULL Device Descriptor on %s", listener->getSenderSession().c_str());
        return false;
    }
}

bool xpDeviceProxy::removeEventListener(std::string sessionName)
{
    return false;
}


