/*
 * xpPropertyList.cpp
 *
 *  Created on: May 7, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpPropertyList.h"
#include <stdlib.h>
#include <sstream>

xpPropertyList::xpPropertyList() {

}


//xpPropertyList::xpPropertyList(std::vector<struct property> properties) {
//	for(int i = 0; i < (int) properties.size(); i++) {
//		struct property app = properties.at(i);
//		addProperty(app.name, app.value);
//	}
//}

xpPropertyList::~xpPropertyList() {
}

int xpPropertyList::checkPropertyExists(std::string key) {
	for (size_type i = 0; i < size(); i++) {
		if (at(i).getKey().compare(key) == 0) {
			return i;
		}
	}
	return -1;
}

void xpPropertyList::setProperty(char* key, char* value) {
	setProperty(std::string(key), std::string(value));
}


void xpPropertyList::setProperty(std::string key, std::string value){
	int retVal = checkPropertyExists(key);
	if(retVal == -1)
		push_back(xpProperty(key, value));
	else
		at(retVal).setValue(value);
};

void xpPropertyList::setProperty(std::string key, int value){
	std::ostringstream v;
	v << value;

	int retVal = checkPropertyExists(key);
	if(retVal == -1)
		push_back(xpProperty(key, v.str()));
	else
		at(retVal).setValue(v.str());
};

void xpPropertyList::setProperty(std::string key, long int value){
	std::ostringstream v;
	v << value;

	int retVal = checkPropertyExists(key);
	if(retVal == -1)
		push_back(xpProperty(key, v.str()));
	else
		at(retVal).setValue(v.str());
};

void xpPropertyList::setProperty(std::string key, long long int value){
	std::ostringstream v;
	v << value;

	int retVal = checkPropertyExists(key);
	if(retVal == -1)
		push_back(xpProperty(key, v.str()));
	else
		at(retVal).setValue(v.str());
};

void xpPropertyList::setProperty(std::string key, double value){
	std::ostringstream v;
	v << value;

	int retVal = checkPropertyExists(key);
	if(retVal == -1)
		push_back(xpProperty(key, v.str()));
	else
		at(retVal).setValue(v.str());
};

void xpPropertyList::setProperty(std::string key, bool value){
	std::ostringstream v;
	v << ((value) ? "true" : "false");

	int retVal = checkPropertyExists(key);
	if(retVal == -1)
		push_back(xpProperty(key, v.str()));
	else
		at(retVal).setValue(v.str());
};

int xpPropertyList::getIntProperty(std::string key, int index){
	return atoi(getProperty(key, index).c_str());
};

long int xpPropertyList::getLongIntProperty(std::string key, int index){
	return atol(getProperty(key, index).c_str());
};

long long int xpPropertyList::getLongLongIntProperty(std::string key, int index){

#ifdef WIN32
	return atol(getProperty(key, index).c_str());
#else
	return atoll(getProperty(key, index).c_str());
#endif
};

std::string xpPropertyList::getStringProperty(std::string key, int index){
	return getProperty(key, index);
};

bool xpPropertyList::getBoolProperty(std::string key, int index){
	return getProperty(key, index).compare("true") == 0 ?  true : false;
};

double xpPropertyList::getDoubleProperty(std::string key, int index){
	return atof(getProperty(key, index).c_str());
	//return 0.0;

};

std::string xpPropertyList::getProperty(std::string key, int index) {
	int count = 0;

	for (size_type i = 0; i < size(); i++) {
		if (at(i).getKey().compare(key) == 0) {
			if (count++ == index) {
				return at(i).getValue();
			}
		}
	}

	return std::string("");
}

//std::vector<xpProperty> xpPropertyList::toXpPropertyVector() {
//	std::vector<xpProperty> retVal;
//	for (size_type i = 0; i < size(); i++) {
//		xpProperty app = xpProperty(at(i).getKey(), at(i).getValue());
//		retVal.push_back(app);
//	}
//	return retVal;
//}


//std::vector<struct property> xpPropertyList::toSoapFormat() {
//
//	std::vector<struct property> retval;
//
//	struct property p;
//	for(unsigned int count = 0; count < size(); count++) {
//		xpProperty property = at(count);
//		p.name = property.getKey();
//		p.value = property.getValue();
//		retval.push_back(p);
//	}
//	return retval;
//}

void xpPropertyList::addProperty(char* key, char* value) {
	addProperty(std::string(key), std::string(value));
}

void xpPropertyList::addProperty(std::string key, std::string value){
	push_back(xpProperty(key, value));
};

void xpPropertyList::addProperty(std::string key, int value){
	std::ostringstream v;
	v << value;

	push_back(xpProperty(key, v.str()));
};

void xpPropertyList::addProperty(std::string key, long int value){
	std::ostringstream v;
	v << value;

	push_back(xpProperty(key, v.str()));
};

void xpPropertyList::addProperty(std::string key, long long int value){
	std::ostringstream v;
	v << value;

	push_back(xpProperty(key, v.str()));
};

void xpPropertyList::addProperty(std::string key, double value){
	std::ostringstream v;
	v << value;

	push_back(xpProperty(key, v.str()));
};

void xpPropertyList::addProperty(std::string key, bool value){
	std::ostringstream v;
	v << ((value) ? "true" : "false");

	push_back(xpProperty(key, v.str()));
};
