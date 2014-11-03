/*
 * xpInitParameters.h
 *
 *  Created on: 28 Jul 2010
 *      Author: Gian Maria Simbula
 *      Email: simbula@sardegnaricerche.it
 */

#ifndef XPINITPARAMETERS_H_
#define XPINITPARAMETERS_H_

#include <string>

//!  xpInitParameters is a container for parsed xPlaces Init parameters.
/*!
  An xpInitParameters contains init parameters for node and xpNetwork initialization, obtained by parsing command-line parameters.
  The correct syntax for the method is:
  \code
  [PATH_TO_EXECUTABLE]/[EXECUTABLE_NAME] [OPTIONS]
  \endcode
  The accepted options are:
  \code
  -p [INTEGER] The port used by the node
  -s [ip:port] The address of the server
  -i [INTEGER] The announce interval in seconds
  -n [STRING] The node name
  -t [STRING] OPTIONAL: The node type
  -d [STRING] OPTIONAL: The path to the device
  -x [BOOL] OPTIONAL: Testing mode value, no arguments needed
  \endcode
*/
class xpInitParameters {
public:
	/*! xpInitParameters(int nodePort, std::string serverIpPort, int announceInterval, std::string nodeName, std::string nodeType, std::string devicePath, bool testMode)
	    \brief Constructor for the InitParameters class.
	    \param nodePort The port used by the node as int.
	    \param serverIpPort The address of the server as ip-port address, formatted as "ip:port".
	    \param announceInterval The announce interval in seconds as int.
	    \param nodeName The node name as std::string.
	    \param nodeType The node type as std::string.
	    \param devicePath The path to the plugged device as std::string.
	    \param testMode If true, this parameter enables testing mode as bool.
		\details Explicit constructor for the InitParameters class, it contains the typical init parameters.
		\return A InitParameters xpAction object
	*/
	xpInitParameters(int nodePort, std::string serverIpPort, int announceInterval, std::string nodeName, std::string nodeType, std::string devicePath, bool testMode = false);
	virtual ~xpInitParameters();

	int nodePort; /*!< The port used by the node as int. */
	std::string serverIpPort; /*!< The address of the server as ip-port address, formatted as "http://ip:port". */
	int announceInterval; /*!< The announce interval in seconds as int. */
	std::string nodeName; /*!< The node name as std::string. */
	std::string nodeType; /*!< The node type as std::string. */
	std::string devicePath; /*!< The path to the plugged device as std::string. */
	bool testMode; /*!< If true, this parameter enables testing mode as bool. */
};

#endif /* XPINITPARAMETERS_H_ */

