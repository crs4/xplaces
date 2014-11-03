/*
 * xpApplication.h
 *
 *  Created on: May 13, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPAPPLICATION_H_
#define XPAPPLICATION_H_

#include "xpNode.h"

//! xpApplication is a application node for the xplaces network.
/*!
  As xpNode extender an xpApplication must be instanced whit an xpDescriptor as container for network informations as ip, port, name and node type.
  \code
  //How to bind the network
  xpPropertyList properties;
      ...
      //Fill the properties
      ...
  xpApplication * myApp = new xpApplication(new xpDescriptor("APPLICATION_EXAMPLE", "APP_TYPE", properties));
      ...
      //Initialise the network
      ...
  //Allow application to bind the network
  xpNetwork::getInstance()->publish(myApp);
  \endcode
*/
class xpApplication: public xpNode {
public:
	/*! xpApplication(xpDescriptor * deviceRef)
	    \brief Constructor for the xpApplication class.
		\details It builds an xpApplication from an xpDescriptor.
		\param deviceRef The xpDescriptor containing the device properties and informations.
		\return An instanced xpApplication object
	*/
	xpApplication(xpDescriptor * deviceRef);

	/*! ~xpApplication()
	    \brief Destructor for the xpApplication class.
	*/
	virtual ~xpApplication();
};

#endif /* XPAPPLICATION_H_ */
