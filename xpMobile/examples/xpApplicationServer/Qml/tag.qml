import QtQuick 2.0
import xpDesktop 1.0
import QtMultimedia 5.0

import qml.xpDesktop.items 1.0
import xpDesktop 1.0

//<<id>>
//<<name>>
//<<picture>>

GraphNode
{
    id: tag
    objectName:"<<id>>"

	color: "<<color>>"
	border.color: "white"
    radius: width*0.5
    width: diagonal; height: diagonal
    
    property string text: applicationData.decodeEntities("<<name>>")
    
    property real diagonal: 100
    
    antialiasing: true

    Image
    {
    	id: tagimage
    	source: '<<picture>>'
    	anchors.fill: parent
    }
    
    
	CircleEffect {
		id: circleLayer

		anchors.fill: tagimage
		sourceItem: tagimage

		enabled: true
	}
	
	Rectangle
	{
		width: parent.width + 4
		height: parent.height + 4
		radius: width/2
		border.color: "white"
		border.width: 2
		color: "transparent"
		antialiasing: true
		anchors.centerIn: parent
	}
	
	function appendToParentNode() {
        applicationData.findItemByObjectName("<<parent>>").append(tag);
    }
    
    onFakeStateChanged:
    {
    	tagimage.source = '<<picture>>'
    	console.log('onFakeStateChanged');
    }
}

