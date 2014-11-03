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
        ListElement {type:"content"; text: "Bellissimo Sinnova 2014 l'anno prossimo miglioreremo ancora! Fantastico salone e bellissime aziende! Grande innovazione!"; title:"Inviato da <b>Samuel Iacolina</b>"; source: "http://dovemibutto.files.wordpress.com/2013/07/img_0343.jpg"} 
        ListElement {type:"content"; text: "Bellissimo Sinnova 2014 l'anno prossimo miglioreremo ancora! Fantastico salone e bellissime aziende! Grande innovazione!"; title:"Inviato da <b>Samuel Iacolina</b>"; source: "http://dovemibutto.files.wordpress.com/2013/07/dsc2961.jpg"} 
        ListElement {type:"content"; text: "Bellissimo Sinnova 2014 l'anno prossimo miglioreremo ancora! Fantastico salone e bellissime aziende! Grande innovazione!"; title:"Inviato da <b>Samuel Iacolina</b>"; source: "http://i1.ytimg.com/vi/Tse42aLa9wA/maxresdefault.jpg"} 
        ListElement {type:"content"; text: "Bellissimo Sinnova 2014 l'anno prossimo miglioreremo ancora! Fantastico salone e bellissime aziende! Grande innovazione!"; title:"Inviato da <b>Samuel Iacolina</b>"; source: "http://1.bp.blogspot.com/-VhTyqRs5y4c/UtGb5yiwYMI/AAAAAAAALQk/gEh1DxEjTxk/s1600/IMG_8635.JPG"} 
        ListElement {type:"content"; text: "Bellissimo Sinnova 2014 l'anno prossimo miglioreremo ancora! Fantastico salone e bellissime aziende! Grande innovazione!"; title:"Inviato da <b>Samuel Iacolina</b>"; source: "http://1.bp.blogspot.com/-1m96LsVRUUE/UpJ5-CrotJI/AAAAAAAABHw/OrlWR31b9Y0/s1600/IMG_0736.JPG"} 
    }
    
    function appendToParentNode() {
		var parentItem = applicationData.findItemByObjectName("<<parent>>");
        parentItem.setChild(slides);
    }
    
    path: "circular"
    
    coverFlowWidth: 600
    coverFlowHeight: 600
    
    circularShader: false
    
    itemWidth: 550
    itemHeight: 350
    
    itemColor: parent.color
    itemBorderWidth: 3
    itemBorderColor: "white"
    itemInnerBorderWidth: 40
    
    backButtonOffsetCenter: itemHeight/2
    
}

