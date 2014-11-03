/*
 * xpDeviceDescriptor.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpDescriptor.h"
#include <string.h>
#include <cstdlib> // rand()
#include "xpToolSet.h"

xpDescriptor::xpDescriptor(std::string name, std::string type, xpPropertyList propertiesVector) {
	setName(name);
	setType(type);

	for (unsigned int i = 0 ; i < propertiesVector.size(); i++) {
		push_back(propertiesVector[i]);
	}
}


xpDescriptor::xpDescriptor(xpPropertyList propertiesVector) {
	for (unsigned int i = 0 ; i < propertiesVector.size(); i++) {
		addProperty(propertiesVector[i].getKey(), propertiesVector[i].getValue());
	}
}

xpDescriptor::~xpDescriptor() {
}


//std::string xpDescriptor::randomString(int length, bool letters, bool numbers, bool symbols) {
//    std::string allPossible; // this will contain all necessary characters
//    std::string str; // the random string
//    if (letters == true) { // if you passed true for letters, we'll add letters to the possibilities
//        for (int i = 65; i <= 90; i++) {
//            allPossible += static_cast<char>(i);
//            allPossible += static_cast<char>(i+32); // add a lower case letter, too!
//        }
//    } if (numbers == true) { // if you wanted numbers, we'll add numbers
//        for (int i = 48; i <= 57; i++) {
//            allPossible += static_cast<char>(i);
//        }
//    } if (symbols == true) { // if you want symbols, we'll add symbols (note, their ASCII values are scattered)
//        for (int i = 33; i <= 47; i++) {
//            allPossible += static_cast<char>(i);
//        } for (int i = 58; i <= 64; i++) {
//            allPossible += static_cast<char>(i);
//        } for (int i = 91; i <= 96; i++) {
//            allPossible += static_cast<char>(i);
//        } for (int i = 123; i <= 126; i++) {
//            allPossible += static_cast<char>(i);
//        }
//    }
//    // get the number of characters to use (used for rand())
//    int numberOfPossibilities = allPossible.length();
//
//    srand ( time(NULL) );
//    for (int i = 0; i < length; i++) {
//        str += allPossible[rand() % numberOfPossibilities];
//    }
//
//    return str;
//}

//std::vector<struct property> xpDescriptor::toSoapFormat() {
//	std::vector<struct property> soapProperties = xpPropertyList::toSoapFormat();
//	soapProperties.push_back(xpProperty(std::string("name"), getName()).toSoapFormat());
//	soapProperties.push_back(xpProperty(std::string("type"), getType()).toSoapFormat());
//
//	return soapProperties;
//}
