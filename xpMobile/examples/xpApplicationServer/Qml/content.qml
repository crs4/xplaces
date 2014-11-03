import QtQuick 2.0

import qml.xpDesktop.items 1.0
import xpDesktop 1.0

Item {
	
	function appendToListModel()
	{
		applicationData.findItemByObjectName("<<parent>>").listModel.append({"type":"content","text": "<<message>>", "title":"<<title>>", "source": "<<picture>>"});
	}
	
}

