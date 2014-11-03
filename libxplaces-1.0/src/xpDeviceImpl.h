/*
 * xpDevice.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPDEVICEIMPL_H_
#define XPDEVICEIMPL_H_

#include "xpEventListener.h"
#include "xpAction.h"
#include "xpDeviceState.h"
#include "xpDevice.h"
#include "xpDescriptor.h"


#include <pthread.h>

#include <vector>

class xpAnnounceItem;

//!  xpDeviceImpl is a node able to send events and receive actions.
/*!
  xpDeviceImpl inherits from xpDevice. As xpNode must be instanced by defining a xpDescriptor, containing his network informations.
  As xpDevice derived class must implements the pure virtual methods of the superclass.
  \code
  //How extend xpDeviceImpl

  #include "xpDeviceImpl.h"
  class MyDeviceImpl : public xpDeviceImpl {
  public:
	MyDeviceImpl(xpDeviceDescriptor *descriptor);
	virtual ~MyDeviceImpl();
	virtual void actionReceived(xpAction *) {
		//Handler for action received...
	};
  }
  \endcode

  \code
  //How to bind the network

  //Initialise an xpDeviceImpl with name, type and properties
  MyDeviceImpl *deviceImpl = new MyDeviceImpl(new xpDeviceDescriptor("EXAMPLE_01", "example", properties));
  //Init the network services (bind the network and start the UDP service)
  xpNetwork::getInstance()->init(PORT_TO_BIND,"http://SERVER_IP:SERVER_PORT", ANNOUNCE_INTERVAL);
  //Bind the xpDeviceImpl to the network
  xpNetwork::getInstance()->publish(deviceImpl);
  \endcode
*/
class xpDeviceImpl : public xpDevice {
public:
	/*! xpDeviceImpl(xpDescriptor * deviceRef, bool checkListenerMode = false)
	    \brief Constructor for the xpDeviceImpl class.
	    \param deviceRef It contains the fundamental informations for this kind of node.
	    \param checkListenerMode If true the device will check for dead listeners.
		\details As defined by the superclass, xpDeviceImpl must be instanced with an xpDescriptor, that contains the fundamental informations for this kind of node.
		\return A instanced xpDeviceImpl object
	*/
    xpDeviceImpl(xpDescriptor * deviceRef);

	/*! ~xpDeviceImpl()
	    \brief Destructor for the xpDeviceImpl class.
	*/
	virtual ~xpDeviceImpl();

	/*! \fn bool addEventListener(xpEventListener *, long int event_mask)
	    \brief This method implements the event listener registration.
	    \param eventListener The event listener to add.
	    \param event_mask The event type as long int mask.
		\details Implemented from the pure virtual definition in the superclass, this method implements the actual registration of an xpEventListener, by adding it in std::vector<xpEventListener*> eventListeners. That vector will be checked during events sending in notifyListeners(xpEvent *event) method. The registration is for the event type specified as method parameter.
		\return The method returns true if successful.
	*/
	virtual bool addEventListener(xpEventListener * eventListener, long int event_mask);

	/*! \fn bool removeEventListener(xpEventListener *)
	    \brief Ths method implements the event listener removing.
	    \param eventListener The event listener to remove.
		\details Implemented from the pure virtual definition in the superclass, this method implements the actual removing of an event listener from the std::vector<xpEventListener*> eventListeners. The removing is inclusive for all event type registration.
		\return The method returns true if successful.
	*/
	virtual bool removeEventListener(xpEventListener * eventListener);

    /*! \fn bool removeEventListener(std::string *)
        \brief Ths method implements the session string of the event listener to remove.
        \param sessionName The the session string of the event listener to remove.
        \details Implemented from the pure virtual definition in the superclass, this method implements the actual removing of an event listener from the std::vector<xpEventListener*> eventListeners. The removing is inclusive for all event type registration.
        \return The method returns true if successful.
    */
    virtual bool removeEventListener(std::string sessionName);

	/*! \fn bool sendAction(xpAction *)
	    \brief This method implements the actions receiving.
	    \param action The received xpAction from an xpDeviceProxy.
		\details This method, inherited as to define from the superclass, implements an action received handler. The actual handler, actionReceived(xpAction *), has to be defined by the class extender.
		\return The method returns true if successful.
	*/
	virtual bool sendAction(xpAction * action);

	/*! \fn void actionReceived(xpAction *)
	    \brief Pure virtual method for actions receiving.
	    \param action The received xpAction from an xpDeviceProxy.
		\details This pure virtual method defines that a class extender has to implements an action received handler.
	*/
	virtual void actionReceived(xpAction * action) = 0;

	/*! \fn void notifyListeners(xpEvent *event)
	    \brief This method implements the event sending.
		\param event The event to be forwarded.
		\details By checking the std::vector<xpEventListener*> eventListeners, this method implements the event forwarding, passed as parameter, to the registered event listeners.
	*/
	void notifyListeners(xpEvent *event);

	/*! \fn pthread_mutex_t getListenersMutex()
	    \brief This method returns the event listeners mutex for external access at the related vector.
		\details This method returns the event listeners pthread_mutex_t for external access at the related vector std::vector<xpEventListener*> eventListeners, defined in the superclass.
		\returns The vector listeners mutex as pthread_mutex_t.
	*/
	pthread_mutex_t getListenersMutex() {
		return listenersMutex;
	}

    void checkDeadListeners(xpAnnounceItem * deadNode);

private:
	pthread_mutex_t listenersMutex;

	static void checkListeners(void * reference);
};

#endif /* XPDEVICEIMPL_H_ */
