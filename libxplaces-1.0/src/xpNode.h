/*
 * xpNode.h
 *
 *  Created on: May 7, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPNODE_H_
#define XPNODE_H_

#include "xpDescriptor.h"

//! A template for an xplaces network endpoint.
/*!
  An xpNode is every object in the xplaces network that has an xpDescriptor and could send announce messages through the network using UDP.
  It's a superclass for xpDevice and xpApplication and it defines that every extended class must have an xpDescriptor as container for network informations as ip, port, name and node type.
*/
class xpNode {
public:
	/*! xpNode(xpDescriptor * deviceRef)
	    \brief Constructor for the xpNode class.
		\details It builds an xpNode from an xpDescriptor.
		\param deviceRef The xpDescriptor containing the device properties and informations.
		\return An instanced xpNode object
	*/
	xpNode(xpDescriptor * deviceRef);

	/*! ~xpNode()
	    \brief Destructor for the xpNode class.
	*/
	virtual ~xpNode();

	/*! xpDescriptor *getDescriptor()
	    \brief It returns the related xpDescriptor.
		\details It returns the related xpDescriptor reference containing the device informations and properties.
		\return A pointer to the related xpDescriptor reference
	*/
	xpDescriptor *getDescriptor() {
		return descriptor;
	} ;

	/*! virtual void publishDone()
	    \brief It contains the post-publish instructions to do.
		\details  It contains the post-publish instructions to do, it has to be implemented but it's not pure virtual.
	*/
	virtual void publishDone() {

	}

private:
	xpDescriptor * descriptor;

};

#endif /* XPNODE_H_ */
