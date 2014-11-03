/*
 * xpDeviceDescriptor.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPDEVICEDESCRIPTOR_H_
#define XPDEVICEDESCRIPTOR_H_

#include <string>
#include <vector>
#include "xpPropertyList.h"
#include "xpMessage.h"

//! xpDescriptor is a container for network informations.
/*!
  An xpDescriptor has to be instanced by every node in the xplaces network that has to connect.
  Inheriting from the xpMessage superclass the methods to obtain address and session, an xpDescriptor must be instanced by specifying the name, the type and the properties of the node.
*/
class xpDescriptor : public xpMessage {
public:
	/*! xpDescriptor(std::string device_name, std::string device_type, xpPropertyList properties)
	    \brief Constructor for the xpDescriptor class.
		\details It instances an xpDescriptor from the specified parameters. This constructor is used when instancing a new node.
		\param device_name The name of the node as std:string.
		\param device_type The type of the node as std::string.
		\param properties The properties list of the node as xpPropertyList.
		\return An instanced xpDescriptor object
	*/
    xpDescriptor(std::string device_name, std::string device_type, xpPropertyList properties);

	/*! xpDescriptor(xpPropertyList properties)
	    \brief Constructor for the xpDescriptor class.
		\details It rebuilds an xpDescriptor from an xpPropertyList.
		\param properties The source xpPropertyList
		\return An instanced xpDescriptor object
	*/
    xpDescriptor(xpPropertyList properties);

    /*! ~xpDescriptor()
	    \brief Destructor for the xpDescriptor class.
	*/
    virtual ~xpDescriptor();

	/*! fn std::string getName()
	    \brief It returns the device name.
		\details It returns the device name as std::string by obtaining it from the properties list.
		\return The device name.
	*/
    std::string getName()
    {
        return getStringProperty("device_name");
    }

	/*! fn std::string getType()
	    \brief It returns the device type.
		\details It returns the device type as std::string by obtaining it from the properties list.
		\return The device type.
	*/
    std::string getType()
    {
    	return getStringProperty("device_type");
    }

	/*! fn std::string setName(std::string value)
	    \brief It sets the device name.
	    \param value The device name value to set.
		\details It sets the device name in the properties list.
	*/
    void setName(std::string value)
    {
        setProperty("device_name", value);
    }

	/*! fn std::string setType(std::string value)
	    \brief It sets the device type.
	    \param value The device type value to set.
		\details It sets the device type in the properties list.
	*/
    void setType(std::string value)
    {
    	setProperty("device_type", value);
    }

//    static std::string randomString(int length, bool letters, bool numbers, bool symbols);

//    virtual std::vector<struct property> toSoapFormat();
};

#endif /* XPDEVICEDESCRIPTOR_H_ */
