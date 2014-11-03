/*
 * xpProperty.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpProperty.h"
#include <sstream>

xpProperty::xpProperty(std::string key, std::string value) {
	this->key = key;
	this->value = value;

}

xpProperty::xpProperty(std::string, long int value) {
	std::ostringstream v;
	v << value;
	this->key = key;
	this->value = v.str();
}

xpProperty::~xpProperty() {
}

//struct property xpProperty::toSoapFormat() {
//	struct property retval;
//	retval.name = getKey();
//	retval.value = getValue();
//
//	return retval;
//}
