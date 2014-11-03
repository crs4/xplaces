import QtQuick 2.0

import qml.xpDesktop.items 1.0
import xpDesktop 1.0

Item {
	
	function enableUser()
	{
		applicationData.findItemByObjectName("<<parent>>").enableUser();
	}
	
	function disableUser()
	{
		applicationData.findItemByObjectName("<<parent>>").disableUser();
	}
	
}

