/*
 * xpDeviceProperty.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPDEVICEPROPERTY_H_
#define XPDEVICEPROPERTY_H_

#include <string>
#include <stdlib.h>
#include <stdint.h>
#include "xpToolSet.h"
#include <string.h>
#include <stdio.h>

//!  xpProperty is a key-value couple.
/*!
  xpProperty defines methods to define a key-value couple and convert to and from a bytes array.
*/
class xpProperty {
public:
	/*! xpProperty(std::string key, std::string value)
	    \brief Constructor for the xpProperty class.
		\details It builds an xpProperty from a couple key-value.
		\param key The key as std::string.
		\param value The value as std::string.
		\return An instanced xpProperty object
	*/
	xpProperty(std::string key, std::string value);

	/*! xpProperty(std::string key, std::string value)
	    \brief Constructor for the xpProperty class.
		\details It builds an xpProperty from a couple key-value.
		\param key The key as std::string.
		\param value The value as long int.
		\return An instanced xpProperty object
	*/
	xpProperty(std::string key, long int value);

	/*! xpProperty(char* bytes)
	    \brief Constructor for the xpProperty class.
		\details It rebuilds an xpProperty from a bytes array.
		\param bytes The source byte array as char *.
		\return An instanced xpProperty object
	*/
	xpProperty(char* bytes){
		uint32_t lk = *((uint32_t*)bytes);
		lk = xpToolSet::toLocalEndianess(lk);
		key.assign(bytes + sizeof(uint32_t), lk);

		uint32_t lv = *((uint32_t*)(bytes + sizeof(uint32_t) + lk));
		lv = xpToolSet::toLocalEndianess(lv);
		value.assign(bytes + sizeof(uint32_t) + lk + sizeof(uint32_t), lv);
	};

    /*! ~xpProperty()
	    \brief Destructor for the xpProperty class.
	*/
	virtual ~xpProperty();

	/*! fn std::string getKey()
	    \brief It returns the key.
		\details It returns the key as std::string.
		\return The key as std::string.
	*/
	std::string getKey() {
		return key;
	}

	/*! fn std::string getValue()
	    \brief It returns the value.
		\details It returns the value as std::string.
		\return The value as std::string.
	*/
	std::string getValue() {
		return value;
	}

	/*! fn void setValue(std::string newValue)()
	    \brief It sets the value.
	    \param newValue The value to set.
		\details It sets the value as std::string.
	*/
	void setValue(std::string newValue){
		value = newValue;
	};

//	struct property toSoapFormat();

	/*! fn char* toByteArray()
	    \brief It converts the xpProperty to bytes array.
		\details It converts the xpProperty to bytes array as char *.
		\return The xpProperty converted to byte array as char *.
	*/
	char* toByteArray() {
		char *retval = new char[getPropertySize()];

		uint32_t lk = getKey().size();
		uint32_t send = xpToolSet::toStdEndianess(lk);
		memcpy(retval, &send, sizeof(uint32_t));
		memcpy(retval + sizeof(uint32_t), getKey().c_str(), lk);

		uint32_t lv = getValue().size();
		send = xpToolSet::toStdEndianess(lv);
		memcpy(retval + sizeof(uint32_t) + lk, &send, sizeof(uint32_t));
		memcpy(retval + sizeof(uint32_t) + lk + sizeof(uint32_t), getValue().c_str(), lv);

		return retval;
	}

	/*! fn int getPropertySize()
	    \brief It returns the xpProperty size.
		\details It returns the xpProperty bytes size as int.
		\return The xpProperty size as int.
	*/
	int getPropertySize() {
		return (2*sizeof(uint32_t) + getKey().size() + getValue().size());
	}

private:
	std::string key;
	std::string value;
};

#endif /* XPDEVICEPROPERTY_H_ */
