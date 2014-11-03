import QtQuick 2.0
import xpDesktop 1.0
import QtMultimedia 5.0

import qml.xpDesktop.items 1.0

//<<id>>
//<<name>>
//<<picture>>

GraphNode
{
    id: tag
    objectName:"<<id>>"

    //Component.onDestruction: console.log("Letter Component destroyed")


    radius: width*0.5
    width: diagonal; height: diagonal

    property real diagonal: 100
    color: "<<color>>"
    border.color: "white"
    border.width: 3

    property string text: applicationData.decodeEntities("<<name>>")

    Text
    {
        id: label
        font.pixelSize: 13//parent.width/text.length*1.4;
        color: "#ffffff"; styleColor: "#222222"; style: Text.Raised
        wrapMode: Text.WordWrap
        width: parent.width - 10
        horizontalAlignment: Text.AlignHCenter 
        anchors.centerIn: parent
        text: parent.text
    }
    
    function appendToParentNode() {
        applicationData.findItemByObjectName("<<parent>>").append(tag);
    }
}