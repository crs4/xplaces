/*
 * xpInitParameters.cpp
 *
 *  Created on: 28 Jul 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpInitParameters.h"
#include "xpToolSet.h"

xpInitParameters::xpInitParameters(int nodePort, std::string serverIpPort, int announceInterval, std::string nodeName, std::string nodeType, std::string devicePath, bool testMode) {
	this->nodePort = nodePort;
	std::string completeAddress = "http://";
	completeAddress.append(serverIpPort);
	this->serverIpPort = completeAddress;
	xpToolSet::testAddress(this->serverIpPort);
	this->announceInterval = announceInterval;
	this->nodeName = nodeName;
	this->nodeType = nodeType;
	this->devicePath = devicePath;
	this->testMode = testMode;
}

xpInitParameters::~xpInitParameters() {
}

