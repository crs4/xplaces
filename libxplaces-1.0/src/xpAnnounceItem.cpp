/*
 * xpAnnounceItem.cpp
 *
 *  Created on: 7 May 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpAnnounceItem.h"
#include "xpNetwork.h"

xpAnnounceItem::xpAnnounceItem(xpDescriptor *descriptor) {

	this->descriptor = descriptor;
	this->timestamp = time(NULL);
//	this->session = descriptor->getSender()->session;

}

xpAnnounceItem::~xpAnnounceItem() {
    delete descriptor;
}


xpDescriptor *xpAnnounceItem::getDescriptor()
{
	return descriptor;
}

std::string xpAnnounceItem::getSession()
{
	return descriptor->getSenderSession();
}

time_t xpAnnounceItem::getTimestamp()
{
    return timestamp;
}

void xpAnnounceItem::updateTimestamp()
{
	this->timestamp = time(NULL);
}


bool xpAnnounceItem::isDead() {
    if((difftime(time(NULL), timestamp) > (xpNetwork::getInstance()->getAnnounceInterval() * 4) )) {
		return true;
	}
	else
		return false;
}

bool xpAnnounceItem::isAlive() {
	return !isDead();

}
