/*
 * xplaces.h
 *
 *  Created on: May 6, 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

/*! \mainpage
 *
 * \section intro What is xPlaces?
 * eXtensible Places is an open source, multi-platform framework designed to enable interactive applications for several environments. It is primarily concerned with the human aspects of ubiquitous computing, applied to enhance the possible interactions between people and the technologies blended with an ambient. Its main focus is on augmenting and extending everyday life and working activities, implementing services and end-user applications for home automation/entertainment, haptic interfaces and interactive surfaces.
 * \section more Let's live interactive!
 * The purpose of this framework is the design and the development of interactive living/working environment, provided with integrated information and communication technology to best satisfy people needs, thanks to an active and no-invasive synchronized orchestration. The main aim is to give to the “world around us” the role of a medium able to hide technological complexity implicit in everyday objects and to make easier people activities encouraging interaction with services and applications. xPlaces framework not only allows the development of specific ready-to-use products, but it also sets up a technological platform comprising device, network and computer programme standardized components allowing for a simple creation of services adapted to different needs and environments spanning office, clinic, factory, museum, metropolitan area, and the global environment. Interaction with them becomes implicit and natural according to people preferences. The eXtended Places could easily and naturally integrate devices ranging from tiny sensors and actuators to hand-held information appliances, able to configure, install, diagnose, maintain, and improve themselves. Such technology enables data collection and processing in a variety of situations, for applications, which include environmental monitoring, context-aware personal assistants, home automation and entertainment, interactive showcase and so on.
 *
 * \section install Installation
 * \subsection step1 Step 1: Download the framework
 * Download the sorces package ".tar.gz" right here \htmlonly <a href="">sources</a>\endhtmlonly
 * \subsection step2 Step 2: Compile the library
 * Extract the sources and call the contained makefile by executing \code make libxplaces \endcode or \code make all \endcode
 * \subsection step3 Step 3: Add the library path.
 * Once compiled, add the path to LD_LIBRARY_PATH and you're ready to use xplaces.
 */

#ifndef XPLACES_H_
#define XPLACES_H_

typedef char XP_DEVICE_NAME[32];
typedef char XP_DEVICE_TYPE[32];

/* SENSORS */
static const XP_DEVICE_TYPE XP_CAMERA = "CAMERA";
static const XP_DEVICE_TYPE XP_RFID = "RFID";
static const XP_DEVICE_TYPE XP_THERMOMETER = "TEMP";
static const XP_DEVICE_TYPE XP_ACCELEROMETER = "ACCELEROMETER";
static const XP_DEVICE_TYPE XP_DIAMOND_TOUCH = "DIAMOND_TOUCH";
static const XP_DEVICE_TYPE XP_FIDUCIAL = "FIDUCIAL";
static const XP_DEVICE_TYPE XP_JACOTABLE = "JACOTABLE";
static const XP_DEVICE_TYPE XP_TFRAME = "TFRAME";
static const XP_DEVICE_TYPE XP_WIDGET = "WIDGET";
static const XP_DEVICE_TYPE XP_GESTURE = "GESTURE";
static const XP_DEVICE_TYPE XP_DEPTH_CAMERA = "DEPTH_CAMERA";

/* ACTUATORS */
static const XP_DEVICE_TYPE XP_X10 = "X10";
static const XP_DEVICE_TYPE XP_VIDEO = "VIDEO";

/* DEVICES */
static const XP_DEVICE_TYPE XP_DESKTOP = "XP_DESKTOP";
static const XP_DEVICE_TYPE XP_DUMMY = "XP_DUMMY";


/* ONLY FOR TESTING */
static const XP_DEVICE_TYPE XP_TEST_SENSOR = "XP_TEST_SENSOR";
static const XP_DEVICE_TYPE XP_TEST_ACTUATOR = "XP_TEST_ACTUATOR";
static const XP_DEVICE_TYPE XP_TEST_APPLICATION = "XP_TEST_APP";

/* MESSAGE TYPES */
static const int XP_ANNOUNCE = 1;
static const int XP_EVENT = 2;
static const int XP_ACTION = 3;
static const int XP_ADD_LISTENER = 4;
static const int XP_REMOVE_LISTENER = 5;

/* Specific Types */
#define XP_ALIVE 0x01 << 31



#endif /* XPLACES_H_ */
