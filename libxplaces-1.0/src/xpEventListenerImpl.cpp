/*
 * xpEventListenerImpl.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpEventListenerImpl.h"
#include "xpNetwork.h"
#include "xpToolSet.h"
#include "xplaces.h"
#include "xpAnnounceItem.h"


xpEventListenerImpl::xpEventListenerImpl(xpNode * node) {
//  setSenderIpPort(xpNetwork::getInstance()->getIpPort());
//	setSenderSession(xpToolSet::randomString(64, true, true, true));

    setSenderSession(node->getDescriptor()->getSenderSession());
    setSenderIpPort(node->getDescriptor()->getSenderIpPort());

	pthread_mutex_init(&sessionsMutex, NULL);
	fprintf(stderr, "New listener, session key: %s\n", getSenderSession().c_str());

}

xpEventListenerImpl::~xpEventListenerImpl() {
}

bool xpEventListenerImpl::newEvent(xpEvent *event)
{
	eventReceived(event);

	return true;
}

void xpEventListenerImpl::addDeviceToListen(xpDescriptor *device) {
	pthread_mutex_lock(&sessionsMutex);
    addDeviceToListenNoBlocking(device);
	pthread_mutex_unlock(&sessionsMutex);
}

void xpEventListenerImpl::addDeviceToListenNoBlocking(xpDescriptor *device) {
    deviceList.push_back(device);
}


void xpEventListenerImpl::removeDeviceFromList(std::string session) {
	pthread_mutex_lock(&sessionsMutex);
    removeDeviceFromListNoBlocking(session);
	pthread_mutex_unlock(&sessionsMutex);
}


void xpEventListenerImpl::removeDeviceFromListNoBlocking(std::string session) {
    xpDescriptor * descriptor = checkDeviceExistsNoBlocking(session);
    deviceList.remove(descriptor);
}

xpDescriptor * xpEventListenerImpl::checkDeviceExistsNoBlocking(std::string sessionToCheck) {

    std::list<xpDescriptor *>::iterator iter1 = deviceList.begin();
    while( iter1 != deviceList.end() ) {
        if ((*iter1)->getSenderSession().compare(sessionToCheck) == 0) {
            return *iter1;
        }
        ++iter1;
    }
    return NULL;
}


xpDescriptor * xpEventListenerImpl::checkDeviceExists(std::string sessionToCheck) {

	pthread_mutex_lock(&sessionsMutex);
    xpDescriptor * checkSessionExists = checkDeviceExistsNoBlocking(sessionToCheck);
	pthread_mutex_unlock(&sessionsMutex);

    return checkSessionExists;
}

void xpEventListenerImpl::connectionReEstablished()
{

}

void xpEventListenerImpl::checkDevices(xpAnnounceItem * deadNode)
{
    bool reestablishedListener = false;

    pthread_mutex_lock(&sessionsMutex);
    std::list<xpDescriptor *> dList = deviceList;
    pthread_mutex_unlock(&sessionsMutex);


    for (int i = 0; i < dList.size(); i++ ) {

        if(checkDeviceExistsNoBlocking(deadNode->getDescriptor()->getSenderSession()))
        {
            setRecipientIpPort(deadNode->getDescriptor()->getSenderIpPort());
            setRecipientSession(deadNode->getDescriptor()->getSenderSession());

            if(xpNetwork::getInstance()->getDeliveryService()->addEventListener(*this, getEventTypeMask()) == 0)
            {
                fprintf(stderr, "Connection re-established, listener added! %s\n", getSenderSession().c_str());

                xpNetwork::getInstance()->addToListenerList(this);

                reestablishedListener = true;
            }
        }
    }

    if(reestablishedListener) {
        connectionReEstablished();
    }
}
