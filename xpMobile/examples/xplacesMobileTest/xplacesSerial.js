
var http = require('http');
var xpTools = require('xpTools');
var xpNode = require('xpNode');
var serialPort = require("serialport");
var SerialPort = serialPort.SerialPort

var serialMap = {};

//var id = 0;
//var name = "xpSerial";
//var type = "XP_DUMMY";

//usage: node xplacesSerial.js 4550 192.168.1.100 192.168.1.100:4000
//xpNode(id, name, type, client, basep, localip, serveripport)
//node = new xpNode(id, name, type, node, process.argv[2], process.argv[3], "http://"+process.argv[4]);
//node.init();

var portid = 0;


function serialPortWriteLog(err, results) {
	console.log('err ' + err);
	console.log('results ' + results);
}


function checkUsbPort()
{
	serialPort.list(function (err, ports) {
	
	  var port = null;	
	  
	  var serialNumber = '';
	
	  var found = ports.some(function(p, index, array) {
		//console.log(serialMap);
		//console.log(p);
		
		if(p.manufacturer.indexOf("Arduino") > -1 || p.manufacturer.indexOf("FTDI") > -1)
		{
			port = p;
			
			if(serialMap[p.serialNumber] == undefined)
			{
				serialNumber = p.serialNumber;
				return true;	
			}
			
		}
		
		return false;
	  });
	  
	  if(found)
	  {
	    var serialPath = port.comName.replace("cu","tty");
	 	var sp = new SerialPort(serialPath, {
  			baudrate: 9600,
  			parser: serialPort.parsers.readline("\n")
		});
		
		serialMap[serialNumber] = {};
		serialMap[serialNumber].serial = sp;
		serialMap[serialNumber].node = null;
			
		sp.on('data', function (data) { serialPortRead(data,serialNumber) });
		sp.on('close', function () { deleteNode(serialNumber); });
		sp.on('error', function () { deleteNode(serialNumber); });
	  }
	  
	  setTimeout(checkUsbPort, 1000);
	  
	  
	});
}


checkUsbPort();

function deleteNode(serialNumber)
{
    if( serialMap[serialNumber].node != null)
    {
    	serialMap[serialNumber].node.close();
		delete serialMap[serialNumber];
	}
}


function serialPortRead(data,serialNumber)
{
	console.log('data received: ' + data);
		
	var properties = new Object();
	
	var propertiesString = data.split(";");
	propertiesString.forEach(function(element)
	{
		var key = element.split("=")[0];
		var value = element.split("=")[1];
		if(key != "\r")
			properties[key] = value;
	});
	
	if(properties["message_type"] == 1)
	{
		if(serialMap[serialNumber].node == null)
			setArduinoNode(properties["device_name"], properties["device_type"],serialNumber)

	}
	else if(properties["message_type"] == 2)
	{
		console.log(properties);
		if(serialMap[serialNumber].node)
			serialMap[serialNumber].node.notifyListeners(properties);
	}
}

function setArduinoNode(name, type, serialNumber)
{    
	var node = new xpNode(name+'_'+portid, type, node, parseInt(process.argv[2])+portid, process.argv[3], "http://"+process.argv[4]);
	node.init();
	portid++;
	
	node.dispatchAction = function(message, objRef) {
		
		
		var buffer = "#";
		
		for(key in message)
		{
			buffer += key+"="+message[key]+";";
		}
		
		buffer += "@\n";
		
		console.log("Action Received: \n" + message);
		console.log("Parsed Action: " + buffer);
		
        serialMap[serialNumber].serial.write(buffer, serialPortWriteLog);
    }
    
    serialMap[serialNumber].node = node;
}