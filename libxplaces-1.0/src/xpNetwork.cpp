/*
 * xpNetwork.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#include "xpNetwork.h"

#ifdef WIN32
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include "XGetopt.h"
#else
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/param.h>
#include <time.h>
#endif


#include <stdio.h>
    // only needed for definition of MAXHOSTNAMELEN for part 1

#include <sstream>
#include <iostream>

using namespace std;

#include <pthread.h>    // use Pthreads
#include "xpToolSet.h"


#define BACKLOG (100)
#define TIMEOUT (24*60*60) /* timeout after 24hrs of inactivity */



void xpNetwork::insertNodeInDeathList(unsigned int index) {

    xpAnnounceItem * deadItem = announcesList[index];

    for(int i=0; i < (int) devicesList.size(); i++) {
        xpDeviceImpl * device = (xpDeviceImpl *)(devicesList[i]);
        if(device)
            device->checkDeadListeners(deadItem);
    }

    deathList.push_back(deadItem);

    fprintf(stderr, "%s with session %s in deathlist: %d \n", deathList.back()->getDescriptor()->getName().c_str(), deathList.back()->getDescriptor()->getSenderSession().c_str(), deathList.size());

}

xpAnnounceItem * xpNetwork::searchAnnounceInDeath(xpDescriptor *descriptor)
{
    for (int i = deathList.size() - 1; i >= 0; i--)
    {
        /*fprintf(stderr, "Dead Announce: %s  New descriptor: %s \n", deathList[i]->getDescriptor()->getSenderIpPort().c_str(), descriptor->getSenderIpPort().c_str());
        fprintf(stderr, "Dead Announce: %s  New descriptor: %s \n", deathList[i]->getDescriptor()->getName().c_str(), descriptor->getName().c_str());
        fprintf(stderr, "Dead Announce: %s  New descriptor: %s \n", deathList[i]->getDescriptor()->getType().c_str(), descriptor->getType().c_str());*/


        if(deathList[i]->getDescriptor()->getSenderIpPort().compare(descriptor->getSenderIpPort()) == 0 &&
                deathList[i]->getDescriptor()->getName().compare(descriptor->getName()) == 0 &&
                deathList[i]->getDescriptor()->getType().compare(descriptor->getType()) == 0)
        {
            fprintf(stderr, "Renewed item! %s\n", deathList[i]->getDescriptor()->getSenderName().c_str());
            fprintf(stderr, "Old: %s    New: %s\n", deathList[i]->getDescriptor()->getSenderSession().c_str(), descriptor->getSenderSession().c_str());

            xpAnnounceItem * renewedItem = deathList[i];

            renewedItem->getDescriptor()->setSenderSession(descriptor->getSenderSession());

            renewedItem->updateTimestamp();

            deathList.erase(deathList.begin() + i);

            announcesList.push_back(renewedItem);

            return renewedItem;
        }
    }

    return NULL;
}

xpAnnounceItem * xpNetwork::searchAnnounceInExistent(xpDescriptor *descriptor)
{
    for (int i = getInstance()->announcesList.size() - 1; i >= 0; i--)
    {
        /*fprintf(stderr, "Announce: %s  New descriptor: %s \n", announcesList[i]->getDescriptor()->getSenderIpPort().c_str(), descriptor->getSenderIpPort().c_str());
        fprintf(stderr, "Announce: %s  New descriptor: %s \n", announcesList[i]->getDescriptor()->getName().c_str(), descriptor->getName().c_str());
        fprintf(stderr, "Announce: %s  New descriptor: %s \n", announcesList[i]->getDescriptor()->getType().c_str(), descriptor->getType().c_str());*/


        if(announcesList[i]->getDescriptor()->getSenderIpPort().compare(descriptor->getSenderIpPort()) == 0 &&
                announcesList[i]->getDescriptor()->getName().compare(descriptor->getName()) == 0 &&
                announcesList[i]->getDescriptor()->getType().compare(descriptor->getType()) == 0)
        {
            fprintf(stderr, "Item already in AnnounceList, updating tables! %s\n", announcesList[i]->getDescriptor()->getName().c_str());
            fprintf(stderr, "Old: %s    New: %s\n", announcesList[i]->getDescriptor()->getSenderSession().c_str(), descriptor->getSenderSession().c_str());

            xpAnnounceItem * renewedItem = announcesList[i];

            renewedItem->updateTimestamp();

            renewedItem->getDescriptor()->setSenderSession(descriptor->getSenderSession());


            return renewedItem;
        }
    }

    return NULL;
}

void *xpNetwork::announce_service()
{

    while(true)	{

        getInstance()->lock();
            for (int i = getInstance()->announcesList.size() - 1; i >= 0; i--) {
                if (getInstance()->announcesList[i]->isDead()) {

                    /*for(int j=0; j< (int) getInstance()->devicesList.size(); j++) {
                        getInstance()->devicesList[j]->removeEventListener(getInstance()->announcesList[i]->getDescriptor()->getSenderSession());
                    }*/

                    getInstance()->insertNodeInDeathList(i);

                    getInstance()->announcesList.erase(getInstance()->announcesList.begin() + i);

                } else {
//					fprintf(stderr, "%s is alive... go on!\n", getInstance()->announcesList[i]->getDescriptor()->getName().c_str());
                }
            }

            for(int i=0; i< (int) getInstance()->devicesList.size(); i++) {
                if (getInstance()->serverIpPort.compare(getInstance()->getIpPort()) != 0) {
                    xpDescriptor * descriptor = getInstance()->devicesList[i]->getDescriptor();
                    getInstance()->sendAnnounce(descriptor, getInstance()->serverIpPort, getInstance()->serverSession);
                }
                for(unsigned int j = 0 ; j < getInstance()->announcesList.size(); j++)	{
                    xpDescriptor * descriptor = getInstance()->announcesList[j]->getDescriptor();
                    if(descriptor->getSenderSession() == getInstance()->serverSession)
                        continue;
                    getInstance()->sendAnnounce(getInstance()->devicesList[i]->getDescriptor(), descriptor->getSenderIpPort(), descriptor->getSenderSession());
                }
            }

            for(int i=0; i< (int) getInstance()->applicationsList.size(); i++) {
                if (getInstance()->serverIpPort.compare(getInstance()->getIpPort()) != 0) {
                    xpDescriptor * descriptor = getInstance()->applicationsList[i]->getDescriptor();
                    getInstance()->sendAnnounce(descriptor, getInstance()->serverIpPort, getInstance()->serverSession);
                }

                for(unsigned int j = 0 ; j < getInstance()->announcesList.size(); j++)	{
                    xpDescriptor * descriptor = getInstance()->announcesList[j]->getDescriptor();
                    if(descriptor->getSenderSession() == getInstance()->serverSession)
                        continue;
                    getInstance()->sendAnnounce(getInstance()->applicationsList[i]->getDescriptor(), descriptor->getSenderIpPort(), descriptor->getSenderSession());
                }
            }
        getInstance()->unlock();

        time_t announceTime = time(NULL);

#ifdef WIN32
        Sleep(xpNetwork::getInstance()->announceInterval *1000);
#else
        sleep(xpNetwork::getInstance()->announceInterval);
#endif

        if(difftime(time(NULL), announceTime) > xpNetwork::getInstance()->announceInterval)
            std::cout << "Error, wait failed! The thread took a pause of seconds: " << difftime(time(NULL), announceTime)  << " announceInterval: " << xpNetwork::getInstance()->announceInterval << std::endl;
    }

    return NULL;
}

/*
 * xpNetwork::addAnnounce obtain a new announce from the server.
 * It must check if a node is offline, comparing its timestamp and session field.
 * If the specific xpNetwork instance is a server, it resends the announce list to the network.
 */
void xpNetwork::addAnnounce(xpDescriptor *deviceDescriptor)
{
    if (deviceDescriptor->getSenderIpPort().compare(getIpPort()) == 0) {
        std::cerr << "ghost received - discarded: " << deviceDescriptor->getSenderSession() << std::endl;
        delete deviceDescriptor;
        return;
    }

    lock();

    for(int i = 0 ; i < (int)announcesList.size(); i++) {

        if(announcesList[i]->getDescriptor()->getSenderSession().compare(deviceDescriptor->getSenderSession()) == 0) {
            announcesList[i]->updateTimestamp();

            time_t time = announcesList[i]->getTimestamp();
            //std::cerr << deviceDescriptor->getName() << " " << deviceDescriptor->getSenderSession() << ": renewal " << gmtime(&time)->tm_min << ":" << gmtime(&time)->tm_sec << std::endl;

            if(deviceDescriptor->getSenderIpPort() == serverIpPort)
                serverSession = deviceDescriptor->getSenderSession();

            unlock();
            delete deviceDescriptor;
            return;
        }
    }

    xpAnnounceItem * renewedItem = searchAnnounceInDeath(deviceDescriptor);
    if(!renewedItem)
    {
        renewedItem = searchAnnounceInExistent(deviceDescriptor);
    }

    unlock();

    if(!renewedItem)
    {
        xpAnnounceItem * newAnnounce = new xpAnnounceItem(deviceDescriptor);

        lock();
        std::cerr << deviceDescriptor->getName() << " on ip: " << deviceDescriptor->getSenderIpPort() << " " << deviceDescriptor->getSenderSession() << ": new item" << std::endl;
        announcesList.push_back(newAnnounce);
        unlock();
    }
    else
    {
        for(int i=0; i < (int) listenersList.size(); i++) {
            xpEventListenerImpl * listener = (xpEventListenerImpl *)(listenersList[i]);
            if(listener)
                listener->checkDevices(renewedItem);
        }
    }

    lock();

    if (serverIpPort.compare(getIpPort()) == 0) {
        for(unsigned int i = 0 ; i < getInstance()->announcesList.size(); i++)	{
            xpDescriptor * announceToReply = getInstance()->announcesList[i]->getDescriptor();
            getInstance()->sendAnnounce(deviceDescriptor, announceToReply->getSenderIpPort(),announceToReply->getSenderSession());
        }
    }

    unlock();
}

bool xpNetwork::sendAnnounce(xpDescriptor * descriptor, std::string destination) {

    // send my descriptor to network
    descriptor->setRecipientIpPort(destination);
    if(destination.compare(descriptor->getSenderIpPort()) != 0) {
        if(getDeliveryService()->announce(*descriptor) == 0)
            return true;
        else
            return false;
    } else {
        return false;
    }
}

bool xpNetwork::sendAnnounce(xpDescriptor * descriptor, std::string destination, std::string recipient_session) {

    // send my descriptor to network
    descriptor->setRecipientSession(recipient_session);
    descriptor->setRecipientIpPort(destination);
    if(destination.compare(descriptor->getSenderIpPort()) != 0) {
        if(getDeliveryService()->announce(*descriptor) == 0)
            return true;
        else
            return false;
    } else {
        return false;
    }
}

void xpNetwork::publish(xpDevice * device) {
    device->getDescriptor()->setSenderIpPort(xpNetwork::getInstance()->getIpPort());
    device->getDescriptor()->setSenderSession(xpToolSet::randomString(SESSION_STRING));

    std::cerr << device->getDescriptor()->getName() << " session key: " << device->getDescriptor()->getSenderSession() << std::endl;

    lock();
        devicesList.push_back(device);
    unlock();

    device->publishDone();
}

void xpNetwork::publish(xpApplication *application) {
    application->getDescriptor()->setSenderIpPort(xpNetwork::getInstance()->getIpPort());
    application->getDescriptor()->setSenderSession(xpToolSet::randomString(SESSION_STRING));

    std::cerr << application->getDescriptor()->getName() << " session key: " << application->getDescriptor()->getSenderSession() << std::endl;

    lock();
        applicationsList.push_back(application);
    unlock();

    application->publishDone();
}


xpNetwork* xpNetwork::instance = NULL;

xpNetwork* xpNetwork::getInstance()
{
    if (instance == NULL)
    {
            instance = new xpNetwork();
    }

    return instance;
}


xpNetwork::xpNetwork() {
//	mutex = PTHREAD_MUTEX_INITIALIZER;

    setlocale (LC_NUMERIC,"en_US.UTF-8"); //Set the float numerical format from [INTEGER].[DECIMAL] to [INTEGER],[DECIMAL]

    pthread_mutex_init(&mutex, NULL);
}

xpNetwork::~xpNetwork()
{

}

void xpNetwork::lock() {
    pthread_mutex_lock(&mutex);
}

void xpNetwork::unlock() {
    pthread_mutex_unlock(&mutex);
}



bool xpNetwork::addToListenerList(xpEventListener *eventListener) {
    lock();
        bool listenerExists = false;
        for(int i=0; i < (int) listenersList.size(); i++) {
            /*fprintf(stderr, "Listener: %s %s %d\n", eventListener->getSenderSession().c_str(), listenersList[i]->getSenderSession().c_str(), i);
            fprintf(stderr, "Listener: %s %s %d\n", eventListener->getRecipientSession().c_str(),listenersList[i]->getRecipientSession().c_str(), i);
            fprintf(stderr, "Listener: %d %d %d\n", eventListener->getEventTypeMask(),listenersList[i]->getEventTypeMask(), i);*/

            if(eventListener->getSenderSession().compare(listenersList[i]->getSenderSession()) == 0
                    && eventListener->getRecipientSession().compare(listenersList[i]->getRecipientSession()) == 0
                    && eventListener->getEventTypeMask() == listenersList[i]->getEventTypeMask())
            {
                listenerExists = true;
                break;
            }
        }
        if(!listenerExists) {
            listenersList.push_back(eventListener);
            fprintf(stderr, "Listener session added as: %s\n", eventListener->getSenderSession().c_str());
        }
    unlock();
    return !listenerExists;
}

void xpNetwork::removeFromListenerList(xpEventListener *eventListener) {
    lock();
        for (int i = getInstance()->listenersList.size() - 1; i >= 0; i--) {
            if (getInstance()->listenersList[i]->getSenderSession().compare(eventListener->getSenderSession()) == 0
                    && eventListener->getRecipientSession().compare(listenersList[i]->getRecipientSession()) == 0
                    && eventListener->getEventTypeMask() == listenersList[i]->getEventTypeMask())
            {
                getInstance()->listenersList.erase(getInstance()->listenersList.begin() + (i));
                fprintf(stderr, "Listener session removed from list!!!\n");
                unlock();
                return;
            }
        }
    unlock();
}

void xpNetwork::init(int port, std::string serverIpPort, int announceInterval) {

    this->port = port;
    this->serverIpPort = serverIpPort;
    xpToolSet::testAndBlockAddress(serverIpPort);
    this->announceInterval = announceInterval;

    pthread_create(new pthread_t(), NULL, (void*(*)(void*))xpNetwork::announce_service,NULL);
    udpService = new xpUDPService(getIpPort());

}

xpInitParameters * xpNetwork::parseInitParameters(int argc, char *argv[]) {
    char *pValue = NULL;
    char *sValue = NULL;
    char *iValue = NULL;
    char *nValue = NULL;
    char *tValue = NULL;
    char *dValue = NULL;
    char *lValue = NULL;
    int c;

    bool nFlag = false;
    bool sFlag = false;
    bool pFlag = false;
    bool iFlag = false;
    bool testFlag = false;

    while ((c = getopt(argc, argv, "p:s:i:n:t:d:l:x")) != -1) {
        switch (c)
        {
        case 'p':
            pValue = optarg;
            pFlag = true;
            break;
        case 's':
            sValue = optarg;
            sFlag = true;
            break;
        case 'i':
            iValue = optarg;
            iFlag = true;
            break;
        case 'n':
            nValue = optarg;
            nFlag = true;
            break;
        case 'l':
            lValue = optarg;
            break;
        case 't':
            tValue = optarg;
            break;
        case 'd':
            dValue = optarg;
            break;
        case 'x':
            testFlag = true;
            break;
        case '?':
#ifndef WIN32
            if (optopt == 'p' || optopt == 's' || optopt == 'i' || optopt == 'n' || optopt == 'l' || optopt == 't' || optopt == 'd')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint(optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,"Unknown option character `\\x%x'.\n",	optopt);
#else
            fprintf(stderr, "Error starting... Usage:\n\texecutable -s IP_SERVER:PORT_SERVER -i INTERVAL_IN_SECONDS -p SERVER_PORT -n NODE_NAME [-x] [-l] [-t]\nWhere:\n");
            fprintf(stderr, "\t-s Server ip and port, colon separated\n");
            fprintf(stderr, "\t-i Announce interval, in seconds\n");
            fprintf(stderr, "\t-n Node name\n");
            fprintf(stderr, "\t-p Node port\n");
            fprintf(stderr, "\t-x Test mode [OPTIONAL]\n");
            fprintf(stderr, "\t-l Network interface [OPTIONAL]\n");
            fprintf(stderr, "\t-d USB Port [OPTIONAL]\n");
            fprintf(stderr, "\t-t Another optional parameter [OPTIONAL]\n");
#endif //NotWIN32
            abort();
        default:
            fprintf(stderr,"default\n");
            abort();
        }
    }
    fprintf(stderr,"%s, %s, %d, %s, %s, %s\n", pValue, sValue, atoi(iValue), nValue, tValue, dValue);

    if(!nFlag || !pFlag || !sFlag || !iFlag) {
        fprintf(stderr,"Parameter missing, aborting...\n");
        fprintf(stderr,"nodePort %d\n", pFlag);
        fprintf(stderr,"serverIpPort %d\n", sFlag);
        fprintf(stderr,"intervalAnnounce %d\n", iFlag);
        fprintf(stderr,"nodeName %d\n", nFlag);
        abort();
    }

    if(!tValue)
        tValue = (char*)"";
    if(!dValue)
        dValue = (char*)"";
    if(!lValue)
        lValue = (char*)"";
    netInterface = std::string(lValue);

    return new xpInitParameters(atoi(pValue), std::string(sValue), atoi(iValue), std::string(nValue), std::string(tValue), std::string(dValue), testFlag);
}

xpInitParameters * xpNetwork::initByParsing(int argc, char *argv[]) {
    xpInitParameters * initParameters = parseInitParameters(argc, argv);
    init(initParameters->nodePort, initParameters->serverIpPort, initParameters->announceInterval);
    return initParameters;
}

xpDeviceProxy *xpNetwork::lookupDevice(std::string name, int timeout) {
    time_t begin = time(NULL);
    xpDeviceProxy *proxy = NULL;

    do {
        proxy = lookupDevice(name);
        if (proxy != NULL) {
            return proxy;
        } else {
#ifndef WIN32
            sleep(2);
#else
            Sleep(2);
#endif
        }
    } while (difftime(time(NULL), begin) < timeout);

    return NULL;
}

std::vector<xpDeviceProxy *> xpNetwork::lookupDevices(std::string type, int timeout) {
    time_t begin = time(NULL);
    std::vector<xpDeviceProxy *> retval;

    do {
        retval = lookupDevices(type);
        if (retval.size() > 0) {
            return retval;
        } else {
#ifndef WIN32
            sleep(2);
#else
            Sleep(2);
#endif
        }
    } while (difftime(time(NULL), begin) < timeout);

    return retval;
}

std::vector<xpDeviceProxy *> xpNetwork::lookupDevices(std::string type)
{
    int i;
    std::vector<xpDeviceProxy *> retval;
    xpDescriptor *temp;

    lock();

    for(i=0 ; i < (int)announcesList.size() ; i++)
    {
        if(announcesList[i]->getDescriptor()->getType().compare(type) == 0)
        {
            temp = announcesList[i]->getDescriptor();
            retval.push_back(new xpDeviceProxy(temp));
        }
    }

    unlock();

    fprintf(stderr, "Lookup type %s: %d devices found\n", type.c_str(), (int)retval.size());

    return retval;
}

xpDeviceProxy *xpNetwork::lookupDeviceNoBlocking(std::string name)
{
    int i;

    xpDescriptor *temp;

    for(i=0 ; i < (int)announcesList.size() ; i++)
    {
        if(announcesList[i]->getDescriptor()->getName().compare(name) == 0)
        {
            temp = announcesList[i]->getDescriptor();
            return new xpDeviceProxy(temp);
        }
    }

    return NULL;
}

xpDeviceProxy *xpNetwork::lookupDevice(std::string name)
{
    lock();
    xpDeviceProxy *temp = lookupDeviceNoBlocking(name);
    unlock();

    if(temp != NULL)
        fprintf(stderr, "Lookup %s: Device found\n", temp->getDescriptor()->getName().c_str());

    return temp;
}

const std::string xpNetwork::getIpPort()
{
    std::ostringstream ip_port;
#ifdef WIN32
    WSADATA WinsockData;
        if (WSAStartup(MAKEWORD(2, 2), &WinsockData) != 0) {
            cerr << "Failed to find Winsock 2.2!" << endl;
            return "ERRORE";
        }
        SOCKET sd = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
        if (sd == SOCKET_ERROR) {
            cerr << "Failed to get a socket. Error " << WSAGetLastError() <<
                endl; return "ERRORE";
        }

        INTERFACE_INFO InterfaceList[20];
        unsigned long nBytesReturned;
        if (WSAIoctl(sd, SIO_GET_INTERFACE_LIST, 0, 0, &InterfaceList,
                sizeof(InterfaceList), &nBytesReturned, 0, 0) == SOCKET_ERROR) {
            cerr << "Failed calling WSAIoctl: error " << WSAGetLastError() <<
                    endl;
            return "ERRORE";
        }

        int nNumInterfaces = nBytesReturned / sizeof(INTERFACE_INFO);

        for (int i = 0; i < nNumInterfaces; ++i) {


            sockaddr_in *pAddress;
            u_long nFlags = InterfaceList[i].iiFlags;

            if (!(nFlags & IFF_LOOPBACK))
            {
                pAddress = (sockaddr_in *) & (InterfaceList[i].iiAddress);
                ip_port << "http://" << inet_ntoa(pAddress->sin_addr) << ":" << this->port;
            }

        }
        WSACleanup();

#else
    struct ifaddrs *ifa = NULL, *ifp = NULL;
    char ip[ NI_MAXHOST ];


    if (getifaddrs (&ifp) < 0)
    {
        perror ("getifaddrs");
        return "";
    }

    for (ifa = ifp; ifa; ifa = ifa->ifa_next)
    {
        socklen_t salen;
        if(netInterface.empty())
        {
            std::string lo = "lo";
            if (ifa->ifa_addr->sa_family == AF_INET && lo.compare(ifa->ifa_name) != 0)
            {
                salen = sizeof (struct sockaddr_in);
                getnameinfo (ifa->ifa_addr, salen, ip, sizeof (ip), NULL, 0, NI_NUMERICHOST) ;
            }
        }
        else
        {
            if (ifa->ifa_addr->sa_family == AF_INET && netInterface.compare(ifa->ifa_name) == 0)
            {
                salen = sizeof (struct sockaddr_in);
                getnameinfo (ifa->ifa_addr, salen, ip, sizeof (ip), NULL, 0, NI_NUMERICHOST) ;
            }
        }
    }
    freeifaddrs (ifp);
    ip_port << "http://" << ip << ":" << this->port;

#endif

    xpToolSet::testAndBlockAddress(ip_port.str());

    return ip_port.str();
}


xpDevice* xpNetwork::searchDeviceByName(std::string name) {
    lock();

    for(int i=0; i < (int) devicesList.size(); i++) {
        if(name.compare(devicesList[i]->getDescriptor()->getName()) == 0) {
            unlock();
            return devicesList[i];
        }
    }

    unlock();
    return NULL;
}


xpDevice* xpNetwork::searchDeviceBySession(std::string session) {
    lock();
    for(int i=0; i < (int) devicesList.size(); i++) {
        if(session.compare(devicesList[i]->getDescriptor()->getSenderSession()) == 0) {
            unlock();
            return devicesList[i];
        }
    }
    unlock();
    return NULL;
}

void xpNetwork::dispatcher(xpPropertyList properties) {

    xpMessage * message = (xpMessage *) &properties;

    switch(message->getMessageType())
    {
        case (XP_EVENT):
            dispatch(xpEvent(*message));
        break;

        case (XP_ACTION):
            dispatch(xpAction(*message));
        break;

        case(XP_ADD_LISTENER):
            dispatch(new xpEventListenerProxy(properties));
        break;

        case(XP_REMOVE_LISTENER):
            dispatchRemove(new xpEventListenerProxy(properties));
        break;

        case(XP_ANNOUNCE):
            addAnnounce(new xpDescriptor(properties));
        break;
    }

}


void xpNetwork::dispatch(xpEvent event){
    lock();
//  std::cerr << "EVENT from " << event.getSenderIpPort() << " to " << event.getRecipientIpPort() << " " << event.getType() << std::endl;
//	std::cerr << "scanning " << listenersList.size() << " event listeners\n";
    for(int i=0; i < (int) listenersList.size(); i++) {
        // If the listener is listening to the session...
        if( ((xpEventListenerImpl *) listenersList[i])->checkDeviceExists(event.getSenderSession())) {
            // ...check if the event type match
            if(listenersList[i]->getEventTypeMask() & event.getType()) {
                listenersList[i]->newEvent(&event);
            }
            else if(event.getType() & XP_ALIVE)
                listenersList[i]->newEvent(&event);
        }
    }
    unlock();
}


void xpNetwork::dispatch(xpAction action){
    //TODO: L'azione è reinoltrata in funzione della chiave di sessione, implementare un meccanismo di bitmask??
    xpDevice * node = searchDeviceBySession(action.getRecipientSession());
    if (node != NULL) {
        node->sendAction(&action);
    } else {
        std::cerr << "no such node results published: " << action.getRecipientSession() << std::endl;
    }
}


void xpNetwork::dispatch(xpEventListenerProxy *eventListener) {

//	xpToolSet::debugOutput(*eventListener);

    xpDevice * device = searchDeviceBySession(eventListener->getRecipientSession());
    if(device)
        device->addEventListener(eventListener, eventListener->getEventTypeMask());
}


void xpNetwork::dispatchRemove(xpEventListenerProxy *eventListener) {

//	xpToolSet::debugOutput(*eventListener);

    xpDevice * device = searchDeviceBySession(eventListener->getRecipientSession());
    if(device)
        device->removeEventListener(eventListener);
}


xpUDPService *xpNetwork::getDeliveryService() {
    return udpService;
}

xpAnnounceItem * xpNetwork::getDeadNode(std::string session) {
    lock();

    for(int i=0 ; i < (int)deathList.size() ; i++)
    {
        if(deathList[i]->getSession().compare(session) == 0)
        {
            unlock();
            return deathList[i];
        }
    }

    unlock();
    return NULL;
}
