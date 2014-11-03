
/*
 * xpDeviceProxy.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPDEVICEPROXY_H_
#define XPDEVICEPROXY_H_

#include "xpEventListener.h"
#include "xpAction.h"
#include "xpDeviceState.h"
#include "xpDevice.h"
#include "xpDescriptor.h"

//!  xpDeviceProxy is a proxy to a node able to send events and receive actions.
/*!
  xpDeviceProxy is a proxy to an xplaces network node able to send events and receive actions. An xpDeviceProxy is an actions forwarder and is instances by the xpDescriptor of an xpDeviceImpl.
  As xpDevice derived class must implements the pure virtual methods of the superclass.
*/
class xpDeviceProxy : public xpDevice{
public:
	/*! xpDeviceProxy(xpDescriptor * deviceRef)
	    \brief Constructor for the xpDeviceProxy class.
	    \param deviceRef It contains the fundamental informations of the related node.
		\details As defined by the superclass, xpDeviceProxy must be instanced with an xpDescriptor, obtained from a related xpDeviceImpl, that contains the fundamental informations of the node.
		\return A instanced xpDeviceProxy object
	*/
	xpDeviceProxy(xpDescriptor * deviceRef);

	/*! ~xpDeviceProxy()
	    \brief Destructor for the xpDeviceProxy class.
	*/
	virtual ~xpDeviceProxy();

	/*! \fn bool addEventListener(xpEventListener * eventListener, long int event_mask)
	    \brief Event listener registration request forwarding.
		\param eventListener The event listener to add.
	    \param event_mask The event type as long int mask.
		\details xpDeviceProxy implements that method as event listener registration request forwarding to the related xpDeviceImpl that implements the actual registration.
		\return The method returns true if successful.
	*/
	virtual bool addEventListener(xpEventListener *, long int event_mask);

	/*! \fn bool removeEventListener(xpEventListener * eventListener)
	    \brief Pure virtual method for event listener removing.
	    \param eventListener The event listener to remove.
		\details This method is implemented by xpDeviceProxy and xpDeviceImpl. xpDeviceProxy implements that method as event listener removing request forwarding to the related xpDeviceImpl that implements the actual removing.
		\return The method must return true if successful.
	*/
	virtual bool removeEventListener(xpEventListener *);

    /*! \fn bool removeEventListener(std::string *)
        \brief Ths method implements the session string of the event listener to remove.
        \param sessionName The the session string of the event listener to remove.
        \details Implemented from the pure virtual definition in the superclass, this method implements the actual removing of an event listener from the std::vector<xpEventListener*> eventListeners. The removing is inclusive for all event type registration.
        \return The method returns true if successful.
    */
    virtual bool removeEventListener(std::string sessionName);

	/*! \fn bool sendAction(xpAction * action)
	    \brief This method implements the actions forwarding.
	    \param action The xpAction to be forwarded to an xpDeviceImpl.
		\details xpDeviceProxy implements that method as actions forwarding to the related xpDeviceImpl that could implement an handler.
		\return The method returns true if successful.
	*/
	virtual bool sendAction(xpAction *);


};

#endif /* XPDEVICEPROXY_H_ */
