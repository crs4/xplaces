/*
 * xpPropertyList.h
 *
 *  Created on: May 7, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPPROPERTYLIST_H_
#define XPPROPERTYLIST_H_

#include "xpProperty.h"
#include <vector>
#include "xplaces.h"

//!  xpPropertyList is a key-value couple vector.
/*!
  xpPropertyList defines methods to define a key-value couple vector and convert to and from a bytes array.
*/
class xpPropertyList : public std::vector<xpProperty> {
public:
	/*! xpPropertyList()
	    \brief Constructor for the xpPropertyList class.
		\details Base constructor for the xpPropertyList class, the vector will be instanced without properties or other informations.
		\return An empty xpPropertyList object
	*/
	xpPropertyList();

	/*! xpPropertyList(char* bytes)
	    \brief Constructor for the xpPropertyList class.
		\details It rebuilds an xpPropertyList from a bytes array.
		\param bytes The source byte array as char *.
		\return An instanced xpPropertyList object
	*/
	xpPropertyList(char* bytes){
		uint32_t n = xpToolSet::toLocalEndianess(*((uint32_t*)(bytes+sizeof(uint32_t))));

		uint32_t offset = 2 *sizeof(uint32_t);
		for (uint32_t i = 0; i < n; i++) {
			xpProperty p(bytes + offset);
			addProperty(p.getKey(), p.getValue());
			offset += p.getPropertySize();
		}

	};

    /*! ~xpPropertyList()
	    \brief Destructor for the xpPropertyList class.
	*/
	virtual ~xpPropertyList();

	/*! fn void setProperty(char* key, char* value)
	    \brief It sets a key-value couple.
	    \param key The key to search as char *.
	    \param value the value as char *.
		\details It sets a key-value couple from char * parameters in the vector.
	*/
	void setProperty(char* key, char* value);

	/*! fn void setProperty(std::string key, std::string value)
	    \brief It sets a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as std::string.
		\details It sets a key-value couple from std::string parameters in the vector.
	*/
	void setProperty(std::string key, std::string value);

	/*! fn void setProperty(std::string key, long int value)
	    \brief It sets a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as long int.
		\details It sets a key-value couple from specified parameters in the vector.
	*/
	void setProperty(std::string key, long int value);

	/*! fn void setProperty(std::string key, long long int value)
	    \brief It sets a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as long long int.
		\details It sets a key-value couple from specified parameters in the vector.
	*/
	void setProperty(std::string key, long long int value);

	/*! fn void setProperty(std::string key, int value)
	    \brief It sets a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as int.
		\details It sets a key-value couple from specified parameters in the vector.
	*/
	void setProperty(std::string key, int value);

	/*! fn void setProperty(std::string key, double value)
	    \brief It sets a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as double.
		\details It sets a key-value couple from specified parameters in the vector.
	*/
	void setProperty(std::string key, double value);

	/*! fn void setProperty(std::string key, bool value)
	    \brief It sets a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as bool.
		\details It sets a key-value couple from specified parameters in the vector.
	*/
	void setProperty(std::string key, bool value);

	/*! fn void addProperty(char* key, char* value)
	    \brief It adds a key-value couple.
	    \param key The key to search as char *.
	    \param value the value as char *.
		\details It adds a key-value couple from char * parameters in the vector.
	*/
	void addProperty(char* key, char* value);

	/*! fn void addProperty(std::string key, std::string value)
	    \brief It adds a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as std::string.
		\details It adds a key-value couple from specified parameters in the vector.
	*/
	void addProperty(std::string key, std::string value);

	/*! fn void addProperty(std::string key, long int value)
	    \brief It adds a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as long int.
		\details It adds a key-value couple from specified parameters in the vector.
	*/
	void addProperty(std::string key, long int value);

	/*! fn void addProperty(std::string key, long long int value)
	    \brief It adds a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as long long int.
		\details It adds a key-value couple from specified parameters in the vector.
	*/
	void addProperty(std::string key, long long int value);

	/*! fn void addProperty(std::string key, int value)
	    \brief It adds a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as int.
		\details It adds a key-value couple from specified parameters in the vector.
	*/
	void addProperty(std::string key, int value);

	/*! fn void addProperty(std::string key, double value)
	    \brief It adds a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as double.
		\details It adds a key-value couple from specified parameters in the vector.
	*/
	void addProperty(std::string key, double value);

	/*! fn void addProperty(std::string key, bool value)
	    \brief It adds a key-value couple.
	    \param key The key to search as std::string.
	    \param value the value as bool.
		\details It adds a key-value couple from specified parameters in the vector.
	*/
	void addProperty(std::string key, bool value);

	/*! fn int getIntProperty(std::string key, int index = 0)
	    \brief It returns the value for the key occurrence.
	    \param key The key to search as std::string
	    \param index The occurence of the key, default is zero, the first occurrence
		\details It returns the value for the key occurrence and the specified index. If index is not specified the first occurrence will be returned.
		\return The int property related to the specified key and index.
	*/
	int getIntProperty(std::string key, int index = 0);

	/*! fn long int getIntProperty(std::string key, int index = 0)
	    \brief It returns the value for the key occurrence.
	    \param key The key to search as std::string
	    \param index The occurence of the key, default is zero, the first occurrence
		\details It returns the value for the key occurrence and the specified index. If index is not specified the first occurrence will be returned.
		\return The long int property related to the specified key and index.
	*/
	long int getLongIntProperty(std::string key, int index = 0);

	/*! fn long long int getIntProperty(std::string key, int index = 0)
	    \brief It returns the value for the key occurrence.
	    \param key The key to search as std::string
	    \param index The occurence of the key, default is zero, the first occurrence
		\details It returns the value for the key occurrence and the specified index. If index is not specified the first occurrence will be returned.
		\return The long long int property related to the specified key and index.
	*/
	long long int getLongLongIntProperty(std::string key, int index = 0);

	/*! fn std::string getIntProperty(std::string key, int index = 0)
	    \brief It returns the value for the key occurrence.
	    \param key The key to search as std::string
	    \param index The occurence of the key, default is zero, the first occurrence
		\details It returns the value for the key occurrence and the specified index. If index is not specified the first occurrence will be returned.
		\return The std::string property related to the specified key and index.
	*/
	std::string getStringProperty(std::string key, int index = 0);

	/*! fn bool getIntProperty(std::string key, int index = 0)
	    \brief It returns the value for the key occurrence.
	    \param key The key to search as std::string
	    \param index The occurence of the key, default is zero, the first occurrence
		\details It returns the value for the key occurrence and the specified index. If index is not specified the first occurrence will be returned.
		\return The bool property related to the specified key and index.
	*/
	bool getBoolProperty(std::string key, int index = 0);

	/*! fn double getIntProperty(std::string key, int index = 0)
	    \brief It returns the value for the key occurrence.
	    \param key The key to search as std::string
	    \param index The occurence of the key, default is zero, the first occurrence
		\details It returns the value for the key occurrence and the specified index. If index is not specified the first occurrence will be returned.
		\return The double property related to the specified key and index.
	*/
	double getDoubleProperty(std::string key, int index = 0);

//	std::vector<xpProperty> toXpPropertyVector();
//	virtual std::vector<struct property> toSoapFormat();

	/*! fn char* toByteArray()
	    \brief It converts the xpPropertyList to bytes array.
		\details It converts the xpPropertyList to bytes array as char *.
		\return The xpPropertyList converted to byte array as char *.
	*/
	char *toByteArray(){

		//Return array
		int listSize = sizeof(uint32_t) + sizeof(uint32_t);
		char *retval = new char[listSize];

		//Num couples
        uint32_t send = xpToolSet::toStdEndianess((uint32_t)size());
		memcpy(retval + sizeof(uint32_t), &send, sizeof(uint32_t));

		int propertySize;
		for(int i = 0; i < (int)size(); i++) {
			propertySize = at(i).getPropertySize();
			listSize += propertySize;
			retval = (char*) realloc (retval, listSize);
			char *serialized_property = at(i).toByteArray();
			memcpy(retval + (listSize - propertySize), serialized_property, propertySize);
			delete serialized_property;
		}

        send = xpToolSet::toStdEndianess((uint32_t)listSize);
		memcpy(retval, &send, sizeof(uint32_t));
		return retval;
	}


private:
	std::string getProperty(std::string key, int index = 0);
	int checkPropertyExists(std::string key);
};

#endif /* XPPROPERTYLIST_H_ */
