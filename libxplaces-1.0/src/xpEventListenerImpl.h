/*
 * xpEventListenerImpl.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPEVENTLISTENERIMPL_H_
#define XPEVENTLISTENERIMPL_H_

#include <string>
#include "xpEvent.h"
#include "xpDescriptor.h"
#include "xpEventListener.h"
#include <list>
#include <pthread.h>
#include "xpNode.h"

class xpAnnounceItem;

//!  xpEventListenerImpl is an actual implementation of an xpEventListener that brings informations concerning event listeners.
/*!
  An xpEventListenerImpl is a event listener implementation that receive events from the network, by obtaining them from related proxyes that forward events from xpDeviceImpl where the listener is registered.
  The class has to implements the pure virtual methods of the superclass.
  \code
  //How to extend xpEventListenerImpl
  #include "xpEventListenerImpl.h"
  class MyEventListener : public xpEventListenerImpl {
  public:
	MyEventListener();
	virtual ~MyEventListener();

	virtual void eventReceived(xpEvent * event) {
		//xpEvent received handler code
	}
  \endcode

  \code
  //How to use a listener to receive events
    ...
    //Init the network services
    ...
  //Retrieve the proxy to an xpDevice
  xpDeviceProxy *proxy = xpNetwork::getInstance()->lookupDevice("DEVICE_NAME", 3600);
  //Instance a new listener
  MyEventListener *listener = new MyEventListener();
  if (proxy != NULL)
	  //Register the listener through the proxy for the specified event type
	  proxy->addEventListener(listener, LONG_INT_TYPE_MASK);
  \endcode
*/
class xpEventListenerImpl : public xpEventListener {
public:
	/*! xpEventListenerImpl()
	    \brief Constructor for the xpEventListenerImpl class.
	    \param hotSwapMode If true the listener will try to reconnect to xpDevice(s) if the devices gone down. False is the default value.
		\details Base constructor for the xpEventListenerImpl class, the event will be instanced without properties or other informations.
		\return An empty xpEventListenerImpl object
	*/
    xpEventListenerImpl(xpNode * node);

    /*! ~xpEventListenerImpl()
        \brief Destructor for the xpEventListenerImpl class.
    */
    virtual ~xpEventListenerImpl();

	/*! \fn bool newEvent(xpEvent * event)
	    \brief Method for the event receiving.
		\param event The received event as xpEvent.
		\details This method implements the event receiving by calling the method eventReceived(xpEvent * event).
		\return The method returns true if successful.
	*/
	virtual bool newEvent(xpEvent * event);

	/*! \fn void eventReceived(xpEvent * event)
	    \brief Pure virtual method for the event receiving.
		\param event The received event as xpEvent.
		\details This method must implement an handler for an event received.
	*/
	virtual void eventReceived(xpEvent * event)=0;

	/*! \fn void addSessionToList(std::string session)
	    \brief It adds a session to the listener list.
		\param session The session to add as std::string.
		\details This method adds a listener to the class defined listeners list.
	*/
    void addDeviceToListen(xpDescriptor * device);

	/*! \fn void addSessionToListNoBlocking(std::string session)
	    \brief It adds a session to the listener list without mutex control.
		\param session The session to add as std::string.
		\details This method adds a listener to the class defined listeners list without mutex control.
	*/
    void addDeviceToListenNoBlocking(xpDescriptor * device);

	/*! \fn void removeSessionFromList(std::string session)
	    \brief It removes a session from the listener list.
		\param session The session to remove as std::string.
		\details This method removes a listener from the class defined listeners list.
	*/
    void removeDeviceFromList(std::string session);

	/*! \fn void removeSessionFromListNoBlocking(std::string session)
	    \brief It removes a session from the listener list without mutex control.
		\param session The session to remove as std::string.
		\details This method removes a listener from the class defined listeners list without mutex control.
	*/
    void removeDeviceFromListNoBlocking(std::string session);


	/*! \fn bool checkSessionExists(std::string sessionToCheck)
	    \brief It checks if a session is registered.
		\param sessionToCheck The session to check as std::string.
		\details It checks if a session is registered in the class defined listeners list.
		\return It returns true if the session is registered, false otherwise.
	*/
    xpDescriptor * checkDeviceExists(std::string sessionToCheck);

    xpDescriptor * checkDeviceExistsNoBlocking(std::string sessionToCheck);

	/*! \fn getSessionListSize() const
	    \brief It returns the sessions list size.
		\details It returns the number of registered sessions in the class defined list.
		\return The sessions list size as const int.
	*/
    int getDeviceListSize() const {
        return deviceList.size();
	}

	/*! \fn std::list<std::string> getSessionList() const
	    \brief It returns the sessions list.
		\details It returns the registered sessions in the class defined list.
		\return The sessions list as const std::list<std::string>.
	*/
    std::list<xpDescriptor *> getDeviceList() const {
        return deviceList;
	}

    virtual void connectionReEstablished();

    void checkDevices(xpAnnounceItem * deadNode);

protected:
    std::list<xpDescriptor *> deviceList; /*!< std::list<std::string> containing the registered sessions. */

private:
	pthread_mutex_t sessionsMutex;

	static void checkDevices(void * reference);

    xpNode * node;

};

#endif /* XPEVENTLISTENERIMPL_H_ */
