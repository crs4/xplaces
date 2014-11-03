/*
 * xpNetwork.h
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPNETWORK_H_
#define XPNETWORK_H_

#include "xpDeviceImpl.h"
#include "xpDeviceProxy.h"
#include "xpEventListenerImpl.h"
#include "xpEventListenerProxy.h"
#include "xpAction.h"
#include "xpEvent.h"
#include "xpDescriptor.h"
#include "xpAnnounceItem.h"

#include <string>
#include "xpEventListener.h"
#include "xpApplication.h"

#include <time.h>
#include <pthread.h>

#include "udp/xpUDPService.h"
#include "xpInitParameters.h"

//!  xpNetwork is an interface to the xplaces network.
/*!
  An xpNetwork is an interface to the xplaces network that implements the methods to add a new node to the network, notify a new node, check if a node left the network and obtain references to them.
  xpNetwork is a singleton class, so it's used by calling the getInstance() static method.
  \code
  //Example: How to start network service.
  xpPropertyList properties;
     ...
     //Fill the sensorImpl properties
     ...
  MySensorImpl *sensorImpl = new MySensorImpl(new xpDeviceDescriptor("EXAMPLE_01", "example", properties));
  //Init the network services (bind the network and start the UDP service)
  xpNetwork::getInstance()->init(PORT_TO_BIND,"http://SERVER_IP:SERVER_PORT", ANNOUNCE_INTERVAL);
  //Bind the xpDeviceImpl to the network
  xpNetwork::getInstance()->publish(sensorImpl);
  \endcode
*/
class xpNetwork {
public:

	 int announceInterval;

	/*! fn static xpNetwork *getInstance()
	    \brief It returns the xpNetwork instance.
		\details It returns the pointer to the xpNetwork instance, by calling the constructor if the pointer is NULL.
		\return A pointer to the xpNetwork instance.
	*/
	static xpNetwork *getInstance();

    /*! ~xpNetwork()
	    \brief Destructor for the xpNetwork class.
	*/
	virtual ~xpNetwork();

	/*! fn void init(int port, std::string serverIpPort, int announceInterval)
	    \brief It initialises the xpNetwork services.
	    \param port The port used to listen from the network
	    \param serverIpPort The address of the server as std::string formatted as "http://ip:port"
	    \param announceInterval The time interval between two announces from the same xpNode
		\details It initialises the xpNetwork instance starting the service threads for announcing and UDP sending/receiving.
	*/
	void init(int port, std::string serverIpPort, int announceInterval);

	/*! fn xpInitParameters * parseInitParameters(int argc, char *argv[])
	    \brief It returns parsed parameters for initialization.
	    \param argc The number of parameters as int.
	    \param argv The parameters received as *char[].
	    \return The xpInitParameters pointer to the parsed parameters.
		\details It returns parsed parameters for xpNetwork and xPlaces nodes initialization. It aborts if the parameters doesn't match.
	*/
	xpInitParameters * parseInitParameters(int argc, char *argv[]);

	/*! fn xpInitParameters * initByParsing(int argc, char *argv[])
	    \brief It returns parsed parameters for initialization and it starts the network services.
	    \param argc The number of parameters as int.
	    \param argv The parameters received as *char[].
	    \return The xpInitParameters pointer to the parsed parameters.
		\details It returns parsed parameters for xpNetwork and xPlaces nodes initialization and it starts the network services. It aborts if the parameters doesn't match.
	*/
	xpInitParameters * initByParsing(int argc, char *argv[]);

	/*! fn xpDeviceProxy *lookupDevice(std::string name)
	    \brief It returns the xpDeviceProxy to the related device specified by the param.
	    \param name The device name to look up.
		\details It returns the xpDeviceProxy to the device specified by the param name otherwise NULL.
		\return The xpDeviceProxy pointer to the searched device if found, otherwise NULL.
	*/
	xpDeviceProxy *lookupDevice(std::string name);

	/*! fn xpDeviceProxy *lookupDevice(std::string name)
	    \brief It returns the xpDeviceProxy to the related device specified by the param without mutex control.
	    \param name The device name to look up.
		\details It returns the xpDeviceProxy to the device specified by the param name otherwise NULL without mutex control.
		\return The xpDeviceProxy pointer to the searched device if found, otherwise NULL.
	*/
	xpDeviceProxy *lookupDeviceNoBlocking(std::string name);

	/*! fn xpDeviceProxy *lookupDevice(std::string name, int timeout)
	    \brief It returns the xpDeviceProxy to the related device specified by the param.
	    \param name The device name to look up.
	    \param timeout The max time interval until a device will be waited.
		\details It returns the xpDeviceProxy to the device specified by the param name otherwise NULL.
		\return The xpDeviceProxy pointer to the searched device if found, otherwise NULL.
	*/
	xpDeviceProxy *lookupDevice(std::string name, int timeout);

	/*! fn std::vector<xpDeviceProxy *> lookupDevices(std::string type)
	    \brief It returns a vector of xpDeviceProxy(s) by looking the specified type.
	    \param type The device type to look up.
		\details It returns a vector of xpDeviceProxy(s) pointers of the specified type if found.
		\return The vector of xpDeviceProxy(s) pointers of the specified type if found, otherwise NULL.
	*/
	std::vector<xpDeviceProxy *> lookupDevices(std::string type);

	/*! fn std::vector<xpDeviceProxy *> lookupDevices(std::string type, int timeout)
	    \brief It returns a vector of xpDeviceProxy(s) by looking the specified type.
	    \param type The device type to look up.
	    \param timeout The max time interval until the devices vector will be waited.
		\details It returns a vector of xpDeviceProxy(s) pointers of the specified type if found.
		\return The vector of xpDeviceProxy(s) pointers of the specified type if found, otherwise NULL.
	*/
	std::vector<xpDeviceProxy *> lookupDevices(std::string type, int timeout);

	/*! fn const std::string getIpPort()
	    \brief It returns the address of the node.
		\details It returns the address of the node as std::string formatted as "http://ip:port".
		\return The address of the node as std::string formatted as "http://ip:port".
	*/
	const std::string getIpPort();

    /*! fn bool sendAnnounce(xpDescriptor * descriptor, std::string destination)
        \brief It sends an announce message through the network.
        \param descriptor The xpDescriptor to send.
        \param destination The max time interval until a device will be waited.
        \param recipient_session The recipient session of the destination node.
        \details It forwards an xpDescriptor to the specified destination using UDP.
        \return It returns true if successful.
    */
    bool sendAnnounce(xpDescriptor *descriptor, std::string destination, std::string recipient_session);

	/*! fn bool sendAnnounce(xpDescriptor * descriptor, std::string destination)
	    \brief It sends an announce message through the network.
	    \param descriptor The xpDescriptor to send.
	    \param destination The max time interval until a device will be waited.
		\details It forwards an xpDescriptor to the specified destination using UDP.
		\return It returns true if successful.
	*/
    bool sendAnnounce(xpDescriptor *descriptor, std::string destination);

	/*! fn void addAnnounce(xpDescriptor * descriptor)
	    \brief It adds a new descriptor.
	    \param descriptor The xpDescriptor received.
		\details It adds a new descriptor to the devices list or applications list, used to forward announces cyclically, by discriminating sessions.
	*/
    void addAnnounce(xpDescriptor * descriptor);

	/*! fn void publish(xpDevice * device)
	    \brief Overload method used to add a new xpDevice to the network.
	    \param device The xpDevice to add.
		\details It allows the network access to a new xpDevice.
	*/
	void publish(xpDevice * device);

	/*! fn void publish(xpApplication * application)
	    \brief Overload method used to add a new xpApplication to the network.
	    \param application The xpApplication to add.
		\details It allows the network access to a new xpApplication.
	*/
    void publish(xpApplication * application);

    /*! fn void addToListenerList(xpEventListener * listener)
        \brief It adds a new xpEventListener to the network.
        \param listener The xpEventListener to add.
        \details It adds a new event listener to the class defined list. This new event listener will be checked when a new event is received.
    */
    bool addToListenerList(xpEventListener * listener);

    /*! fn void removeFromListenerList(xpEventListener * listener)
        \brief It removes an xpEventListener from the network.
        \param listener The xpEventListener to remove.
        \details It removes an event listener from the class defined list. This new event listener will be not checked when a new event is received.
    */
    void removeFromListenerList(xpEventListener * listener);

	/*! fn int getAnnounceInterval() const
	    \brief It returns the announce interval.
		\details It returns the announce interval as cont int.
		\return The announce interval in seconds as const int.
	*/
    int getAnnounceInterval() const
    {
        return announceInterval;
    }

	/*! fn std::vector<xpAnnounceItem*> getAnnouncesList()
	    \brief It returns the announce list.
		\details It returns the announce list as std::vector<xpAnnounceItem*>.
		\return The announce list as std::vector<xpAnnounceItem*>.
	*/
    std::vector<xpAnnounceItem*> getAnnouncesList() {
    		return announcesList;
    }

	/*! std::list<xpAnnounceItem*> getDeathList()
	    \brief It returns the death list.
		\details It returns the dead devices list as std::list<xpAnnounceItem*>.
		\return The death list as std::list<xpAnnounceItem*>.
	*/
    std::vector<xpAnnounceItem *> getDeathList() {
    		return deathList;
    }

    /*! fn void renewItem()
        \brief It renews a dead Item.
        \details It renews the dead device with the specified session.
        \param session The session of dead device to renew.
    */
    xpAnnounceItem * searchAnnounceInDeath(xpDescriptor *deviceDescripto);

	/*! void insertNodeInDeathList(unsigned int index)
	    \brief It inserts a node in the death list.
		\details It inserts an xpAnnounceItem, specified by the param, in the death list.
		\param index The index of the announce item as unsigned int.
	*/
    void insertNodeInDeathList(unsigned int index);

	/*! xpAnnounceItem * getDeadNode(std::string session)
	    \brief It returns a dead node searching by session.
		\details It returns a dead node searching by session as xpAnnounceItem *.
		\return The dead node as xpAnnounceItem *.
	*/
    xpAnnounceItem * getDeadNode(std::string session);

	/*! fn xpDevice *searchDeviceByName(std::string name)
	    \brief It returns the searched xpDevice.
		\details It returns a pointer to the searched device specified by the name.
		\param name The name to look up as std::string.
		\return The pointer to the searched xpDevice if found, otherwise NULL.
	*/
    xpDevice *searchDeviceByName(std::string name);

	/*! fn xpDevice * searchDeviceBySession(std::string session)
	    \brief It returns the searched xpDevice.
		\param session The session to look up as std::string.
		\details It returns a pointer to the searched device specified by the session.
		\return The pointer to the searched xpDevice if found, otherwise NULL.
	*/
    xpDevice* searchDeviceBySession(std::string session);

	/*! fn xpUDPService *getDeliveryService()
	    \brief It returns the UDP Service.
		\details It returns the pointer to the class defined UDP service if xpNetwork is initialised.
		\return The pointer to the class defined UDP service.
	*/
    xpUDPService *getDeliveryService();

	/*! fn void dispatcher(xpPropertyList properties)
	    \brief It dispatchs a received properties list.
		\param properties The xpPropertyList to dispatch at destination
		\details It dispatchs a received properties list to the destination matching type and session.
	*/
    void dispatcher(xpPropertyList properties);

    xpAnnounceItem * searchAnnounceInExistent(xpDescriptor *deviceDescriptor);


private:
    int port;
    std::string serverIpPort;
    std::string serverSession;
    xpUDPService *udpService;
    std::string netInterface;

//    pthread_mutex_t announcesListMutex;
//    pthread_mutex_t applicationsListMutex;
//    pthread_mutex_t devicesListMutex;
//    pthread_mutex_t listenersListMutex;

    pthread_mutex_t mutex;
    void lock();
    void unlock();

    static void *announce_service();


    std::vector<xpDevice*> getDevicesList() const
    {
        return devicesList;
    }

    ;
    std::vector<xpEventListener*> getListenersList() const
    {
        return listenersList;
    }

    ;

    xpNetwork();
    std::vector<xpAnnounceItem*> announcesList;
    std::vector<xpDevice*> devicesList;
    std::vector<xpEventListener*> listenersList;
    std::vector<xpApplication*> applicationsList;
    std::vector<xpAnnounceItem *> deathList;
    std::string server_address;
    static xpNetwork *instance;
    void dispatch(xpEvent event);
    void dispatch(xpAction action);
    void dispatch(xpEventListenerProxy *eventListener);
    void dispatchRemove(xpEventListenerProxy *eventListener);

	friend class xpDevice;

};

#endif /* XPNETWORK_H_ */
