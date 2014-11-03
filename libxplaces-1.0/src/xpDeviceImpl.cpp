/*
 * xpDevice.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpDeviceImpl.h"
#include "xpEventListenerProxy.h"
#include <stdio.h>
#include "xpAddress.h"
#include "xpNetwork.h"
#include "xpAnnounceItem.h"

xpDeviceImpl::xpDeviceImpl(xpDescriptor * deviceRef) : xpDevice(deviceRef) {

    pthread_mutex_init(&listenersMutex, NULL);
}

xpDeviceImpl::~xpDeviceImpl() {
}

void xpDeviceImpl::checkDeadListeners(xpAnnounceItem * deadNode) {

    pthread_mutex_lock(&listenersMutex);
    for (int i = eventListeners.size() - 1; i >= 0; i--) {
        if (eventListeners[i]->getSenderSession().compare(deadNode->getDescriptor()->getSenderSession()) == 0) {
            eventListeners.erase(eventListeners.begin() + (i));
            fprintf(stderr, "\nListener removed %d because is a dead node\n", deadNode->getDescriptor()->getSenderSession().c_str());
            pthread_mutex_unlock(&listenersMutex);
            return;
        }
    }
    pthread_mutex_unlock(&listenersMutex);
    return;
}


bool xpDeviceImpl::sendAction(xpAction *action) {

	actionReceived(action);

	return true;
}

void xpDeviceImpl::notifyListeners(xpEvent *event) {

	event->setSenderIpPort(getDescriptor()->getSenderIpPort());
	event->setSenderSession(getDescriptor()->getSenderSession());
	event->setSenderType(getDescriptor()->getType());
	event->setSenderName(getDescriptor()->getName());

	pthread_mutex_lock(&listenersMutex);
    if ((int) eventListeners.size() > 0) {

		std::vector<xpEventListener*>::iterator it;

		for ( it=eventListeners.begin() ; it != eventListeners.end(); it++ ) {
            xpEventListenerProxy *listener = (xpEventListenerProxy*)*it;

			if (event->getType() & listener->getEventTypeMask()) {
				event->setRecipientIpPort(listener->getSenderIpPort());
				event->setRecipientSession(listener->getSenderSession());
                listener->newEvent(event);
			}

		}
	}
	pthread_mutex_unlock(&listenersMutex);

	//TODO Device Impl: Se il programmatore finale crea un oggetto event, deve avere il controllo su esso, per cui non è possibile invocare la distruzione di event in libreria
	//delete event;

}

bool xpDeviceImpl::addEventListener(xpEventListener *event_listener, long int event_mask) {
	pthread_mutex_lock(&listenersMutex);
    bool listenerExists = false;
    for(int i=0; i < (int) eventListeners.size(); i++) {
        if(event_listener->getSenderSession().compare(eventListeners[i]->getSenderSession()) == 0) {
            listenerExists = true;
        }
        if(event_listener->getSenderIpPort().compare(eventListeners[i]->getSenderIpPort()) == 0 &&
                event_listener->getSenderName().compare(eventListeners[i]->getSenderName()) == 0 &&
                event_listener->getSenderType().compare(eventListeners[i]->getSenderType()) == 0)
        {
            listenerExists = true;
        }
    }
    if(!listenerExists) {
        event_listener->setTimestamp();
        eventListeners.push_back(event_listener);
        fprintf(stderr, "\n%s added the following listener:\n\tip:port %s\n\tsession %s\n\tevent type(s) %ld\n\n", getDescriptor()->getName().c_str(), event_listener->getSenderIpPort().c_str(), event_listener->getSenderSession().c_str(), event_listener->getEventTypeMask());
    }
    pthread_mutex_unlock(&listenersMutex);
	return true;
}

bool xpDeviceImpl::removeEventListener(xpEventListener *eventListener) {

	pthread_mutex_lock(&listenersMutex);
    for (int i = eventListeners.size() - 1; i >= 0; i--) {
		if (eventListeners[i]->getSenderSession().compare(eventListener->getSenderSession()) == 0) {
			eventListeners.erase(eventListeners.begin() + (i));
            fprintf(stderr, "\nListener removed! %s\n", eventListener->getSenderSession().c_str());
			pthread_mutex_unlock(&listenersMutex);
			return true;
		}
	}
	pthread_mutex_unlock(&listenersMutex);
	return false;
}

bool xpDeviceImpl::removeEventListener(std::string sessionName)
{
    pthread_mutex_lock(&listenersMutex);
    for (int i = eventListeners.size() - 1; i >= 0; i--) {
        if (eventListeners[i]->getSenderSession().compare(sessionName) == 0) {
            eventListeners.erase(eventListeners.begin() + (i));
            fprintf(stderr, "\nListener removed! %s\n", sessionName.c_str());
            pthread_mutex_unlock(&listenersMutex);
            return true;
        }
    }
    pthread_mutex_unlock(&listenersMutex);
    return false;
}
