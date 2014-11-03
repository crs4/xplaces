
var http = require('http');
var xpTools = require('xpTools');
var xpNode = require('xpNode');

var node = null;

var name = "xpDummyNodeJS";
var type = "XP_DUMMY";

//usage: node xplacesSerial.js 4550 192.168.1.100 192.168.1.100:4000
//xpNode(id, name, type, client, basep, localip, serveripport)
node = new xpNode(name, type, node, process.argv[2], process.argv[3], "http://"+process.argv[4]);
node.init();

var sec = 0;


function sendEvent()
{
	var message = new Object();
    message["__event_type"] = 256;
    message["dummyvalue"] = sec++;
	node.notifyListeners(message);
}

setInterval(sendEvent, 1000);