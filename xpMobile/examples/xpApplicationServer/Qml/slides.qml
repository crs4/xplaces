import QtQuick 2.0
import xpDesktop 1.0
import QtMultimedia 5.0

import qml.xpDesktop.items 1.0

SinnovaCoverflow
{
	id: slides
    objectName: "<<id>>"

    listModel: demoModel
	
    ListModel {
        id: demoModel
        {% if pillola1.exist %}
        ListElement { type: "text"; source:""; title: "A chi ci ispiriamo"; text: "<<pillola1.text>>"}
        {% endif %}
    	{% if pillola2.exist %}
        ListElement { type: "text"; source:""; title: "Chi ne beneficia"; text: "<<pillola2.text>>"}
        {% endif %}
    	{% if pillola3.exist %}
        ListElement { type: "text"; source:""; title: "Il nostro modo di fare innovazione"; text: "<<pillola3.text>>"}
        {% endif %}
        {% if youtube.exist %}
        ListElement { type: "youtube"; source: "<<youtube.text>>"; title:""; text: ""}
        {% endif %}
    }
    
    function appendToParentNode() {
		var parentItem = applicationData.findItemByObjectName("<<parent>>");
        parentItem.setChild(slides);
    }
    
    path: "circular"
    
    coverFlowWidth: 500
    coverFlowHeight: 500
    
    itemWidth: 350
    itemHeight: 350
    
    itemColor: parent.color
    itemBorderWidth: 3
    itemBorderColor: "white"
    itemInnerBorderWidth: 40

    backButtonOffsetCenter: 120
}

