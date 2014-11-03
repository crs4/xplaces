
/*
 * xpDevice.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPDEVICE_H_
#define XPDEVICE_H_

#include "xpEventListener.h"
#include "xpAction.h"
#include "xpDeviceState.h"
#include <list>
#include "xpNode.h"

//!  xpDevice is a template for every node in the xplaces network able to send events and receive actions.
/*!
  xpDevice is a superclass for the nodes able to send events and receive actions. This kind of node are implemented as xpDeviceImpl, typically events source for the network, and xpDeviceProxy, his input forwarder for the received actions.
  The classes that extend xpDevice must have an xpDescriptor, as defined by the superclass xpNode, and implement his pure virtual methods.
*/
class xpDevice :public xpNode {
public:
	/*! xpDevice(xpDescriptor * deviceRef)
	    \brief Constructor for the xpDevice class.
	    \param deviceRef It contains the fundamental informations for this kind of node.
		\details This constructor defines that every class that extends xpDevice must have an xpDescriptor, that contains the fundamental informations for this kind of node.
		\return A instanced xpDevice object
	*/
	xpDevice(xpDescriptor * deviceRef);

	/*! ~xpDevice()
	    \brief Destructor for the xpDevice class.
	*/
	virtual ~xpDevice();

	/*! \fn bool addEventListener(xpEventListener * eventListener, long int event_mask)
	    \brief Pure virtual method for event listener registration.
		\param eventListener The event listener to add.
	    \param event_mask The event type as long int mask.
		\details This method is implemented by xpDeviceProxy and xpDeviceImpl. xpDeviceProxy implements that method as event listener registration request forwarding to the related xpDeviceImpl that implements the actual registration.
		\return The method must return true if successful.
	*/
	virtual bool addEventListener(xpEventListener * eventListener, long int event_mask) = 0;

	/*! \fn bool removeEventListener(xpEventListener * eventListener)
	    \brief Pure virtual method for event listener removing.
	    \param eventListener The event listener to remove.
		\details This method is implemented by xpDeviceProxy and xpDeviceImpl. xpDeviceProxy implements that method as event listener removing request forwarding to the related xpDeviceImpl that implements the actual removing.
		\return The method must return true if successful.
	*/
	virtual bool removeEventListener(xpEventListener * eventListener) = 0;

    /*! \fn bool removeEventListener(std::string *)
        \brief Ths method implements the session string of the event listener to remove.
        \param sessionName The the session string of the event listener to remove.
        \details Implemented from the pure virtual definition in the superclass, this method implements the actual removing of an event listener from the std::vector<xpEventListener*> eventListeners. The removing is inclusive for all event type registration.
        \return The method returns true if successful.
    */
    virtual bool removeEventListener(std::string sessionName) = 0;

	/*! \fn bool sendAction(xpAction * action)
	    \brief Pure virtual method for actions forwarding and receiving.
	    \param action The xpAction to be forwarded if this method is implemented by an xpDeviceProxy or the received action if this method is implemented by a xpDeviceImpl.
		\details This method is implemented by xpDeviceProxy and xpDeviceImpl. xpDeviceProxy implements that method as actions forwarding to the related xpDeviceImpl that could implement an handler.
		\return The method must return true if successful.
	*/
    virtual bool sendAction(xpAction * action) = 0;

	std::vector<xpEventListener*> eventListeners; /*!< The std::vector contains the eventListener for the device. It's used by xpDeviceImpl during event forwarding. */
};

#endif /* XPDEVICE_H_ */
