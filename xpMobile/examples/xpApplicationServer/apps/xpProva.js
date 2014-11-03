var fs = require('fs');

xpProva.appName = "xpProva";
xpProva.type = "XP_EXAMPLE";
xpProva.count = 0;
xpProva.json = 'var object8 = { type: "XP_APPLICATIONSERVER", events: { 1: {name: "Number", mask: 1, keys: ["number"]}, }, actions: { 1: {name: "Action", mask: 1, keys: ["content"]}, } }';

function xpProva(node){
    
    xpProva.node = node;
    node.init();
    setInterval(xpProva.callback, 1000);
    
    xpProva.node.listenDevice("....", 0x01<<0, xpProva.node);
    
    
    xpProva.node.dispatchAction = function(action){
        
        console.log(util.inspect(action));
        
    }
    
    
    
    xpProva.node.dispatchEvent = function(xpEvent, objRef) {
        if(xpEvent['recipient_session'] == objRef.descriptor['sender_session']) {
            xpTools.xpLog(false, "Event Received\n");
            //xpTools.printHash(xpEvent);
        }
    }
    
    
}


xpProva.callback = function (){
    var event = new Object();
    event['__event_type'] = 0x1 << 0;
    event['number'] = xpProva.count;
    xpProva.node.notifyListeners(event);
    console.log(xpProva.count);
    xpProva.count+=2;
    
}




