/*
 * xpAnnounceItem.h
 *
 *  Created on: 7 May 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPANNOUNCEITEM_H_
#define XPANNOUNCEITEM_H_

#include <ctime>
#include "xpDescriptor.h"

//!  xpAnnounceItem is an item instanced when a new xpDescriptor is received.
/*!
  An xpAnnounceItem defines methods to check if a node is gone down and to extract session and timestamp.
*/
class xpAnnounceItem {
public:
	/*! xpAnnounceItem(xpDescriptor * descriptor)
	    \brief Constructor for the xpAnnounceItem class.
		\details It builds an xpAnnounceItem from an xpDescriptor.
		\param descriptor The source xpDescriptor
		\return An instanced xpAnnounceItem object
	*/
	xpAnnounceItem(xpDescriptor * descriptor);

    /*! ~xpAnnounceItem()
	    \brief Destructor for the xpAnnounceItem class.
	*/
	virtual ~xpAnnounceItem();

	/*! fn xpDescriptor *getDescriptor()
	    \brief It returns the xpDescriptor.
		\details It returns a pointer to the related xpDescriptor.
		\return The pointer to the xpDescriptor.
	*/
	xpDescriptor *getDescriptor();

	/*! fn void updateTimestamp()
	    \brief It updates the timestamp of the item.
		\details It updates the timestamp of the item.
	*/
	void updateTimestamp();

	/*! fn time_t getTimestamp()
	    \brief It returns the timestamp.
		\details It returns the timestamp as time_t.
		\return The timestamp as time_t.
	*/
	time_t getTimestamp();

	/*! fn std::string getSession()
	    \brief It returns the session.
		\details It returns the session as std::string.
		\return The session as std::string.
	*/
	std::string getSession();

	/*! fn bool isDead()
	    \brief It checks if a node is dead.
		\details It checks if the timestamp is so old that the node must be gone down. It adds the dead device to xpNetwork deathList
		\return True if the node is dead, false otherwise.
	*/
	bool isDead();

	/*! fn bool isAlive()
	    \brief It checks if a node is alive.
		\details It checks if the node is still alive.
		\return True if the node is alive, false otherwise.
	*/
	bool isAlive();
private:
	xpDescriptor *descriptor;
	time_t timestamp;
//	std::string session;

};

#endif /* XPANNOUNCEITEM_H_ */
