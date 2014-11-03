
var fs = require('fs');
var xpMobileNode = require('xpMobileNode');
var http = require('http');
var WebSocketClient = require('websocket').client;
var WebSocketServer = require('websocket').server;
var xpTools = require('xpTools');
qs = require('querystring');
var formidable = require('formidable');
var util = require('util');
var path = require('path');
var url = require('url');


var httpPort = xpTools.getConfigurationData(process.argv[2]).netconf.httpPort;


var xpTools = require('xpTools');
xpTools.getConfigurationData(process.argv[2]);



/*-------------------------- HTTP SERVER ------------------------------*/

var server = http.createServer(function (request, response) {

    console.log("Request: " + request.url);
    var rurl = url.parse(request.url, true, true);
    console.log("-----" + rurl.pathname);
    
    if (rurl.pathname == '/upload' && request.method.toLowerCase() == 'post') {
		   // parse a file upload
		   var form = new formidable.IncomingForm();
		   form.uploadDir  = __dirname + "/uploads/";
		   form.parse(request, function(err, fields, files) {
					  response.writeHead(200, {'content-type': 'text/plain'});
					  //response.write();
					  fs.renameSync(files.file.path, files.file.path + files.file.name);
					  files.file.path = files.file.path + files.file.name;
					  files.file.name = "uploads/" + path.basename(files.file.path);
					  //response.end(util.inspect({fields: fields, files: files}));
					  response.end(JSON.stringify(files));
					  });
		   
		   
		   }  
		   else{
			
			var pathname = rurl.pathname;
			
			if(rurl.pathname == '/')
				pathname = '/index.html';
			
			pathname = pathname.replace('%20',' ');

			fs.readFile(__dirname + pathname, function (err,data) {

				if (err) {
					response.writeHead(404);
					response.end(JSON.stringify(err));
					console.log("404 File Error: " + __dirname + pathname);
					return;
				}

				response.writeHead(200);
				response.end(data);
			});
     }

}).listen(httpPort);




/*-------------------------- PROXY CONNECTION ------------------------------*/

//proxy received messages from local nodes
function setProxyConnection(connection, xpNode)
{
		connection.isProxy = true;
		
		xpProxyConnection = connection;
        xpProxyNode = xpNode;
        
		xpProxyNode.newAnnounce = function(message, objRef) {
            //if(pRemoteNetworkTable[message["recipient_session"]])
            {
                //local announce received, reply it to proxy
                //1- store in local network table the session: local_session_node -> sender_ip_port
                //2- if node is dead, delete it from local table
                //3- send message as it is, without any changes
                
                //filter proxy announce
                if(message["sender_session"] == xpProxyNode.descriptor["sender_session"])
                	return;
                	
                //filter remote announce
                if(pRemoteNetworkTable[message["sender_session"]] != undefined)
                	return;
                
                	
                xpTools.xpLog(false, "ProxyReceiveFromLocalNodes: proxy received announce from local node " + message["device_name"] + " " + message["sender_session"]);
                xpTools.xpLog(false, "ProxyReceiveFromLocalNodes: proxy sends to " + message["recipient_ip_port"]);


                pLocalNetworkTable[message["sender_session"]] = new Object();
                pLocalNetworkTable[message["sender_session"]]["ip_port"] = message["sender_ip_port"];
                pLocalNetworkTable[message["sender_session"]]["device_name"] = message["device_name"];

                if(message["status"] == "Dead")
                    delete pLocalNetworkTable[message["sender_session"]];
                else
                    xpProxyConnection.send(JSON.stringify(message));
            }
        }

        xpProxyNode.dispatchEvent = function(message, objRef) {
            //if(pRemoteNetworkTable[message["recipient_session"]])
            {
                //console.log('<----- xpEvent to wsProxy *********************************************' + message["sender_ip_port"] + ' to ' + message["recipient_ip_port"]);
                xpProxyConnection.send(JSON.stringify(message));
            }
        }

        xpProxyNode.dispatchAction = function(message, objRef) {
            //if(pRemoteNetworkTable[message["recipient_session"]])
            {
                //console.log('<----- xpAction to wsProxy ********************************************' + message["sender_ip_port"] + ' to ' + message["recipient_ip_port"]);
                xpProxyConnection.send(JSON.stringify(message));
            }
        }

        xpProxyNode.addListener = function(message, objRef) {
            //if(pRemoteNetworkTable[message["recipient_session"]])
            {
                //console.log('<----- addListener to wsProxy ********************************************' + message["sender_ip_port"] + ' to ' + message["recipient_ip_port"]);
                xpProxyConnection.send(JSON.stringify(message));
            }
        }

        xpProxyNode.removeListener = function(message, objRef) {
            //if(pRemoteNetworkTable[message["recipient_session"]])
            {
                //console.log('<----- addListener to wsProxy ********************************************' + message["sender_ip_port"] + ' to ' + message["recipient_ip_port"]);
                xpProxyConnection.send(JSON.stringify(message));
            }
        }
        
}

//proxy received messages from remote proxy
function proxyForwardMessage(message)
{
            //printHash(message);
            //console.log(data);
            
            message["sender_ip_port"] = xpProxyNode.descriptor["sender_ip_port"];

			//if remote sender_session is unknown, forward announce to server
            if(pRemoteNetworkTable[message["sender_session"]] == undefined)
            {
            	pRemoteNetworkTable[message["sender_session"]] = new Object;
                pRemoteNetworkTable[message["sender_session"]]["ip_port"] = message["sender_ip_port"];
                pRemoteNetworkTable[message["sender_session"]]["device_name"] = message["device_name"];

                if(message["message_type"] == 1)
                {
                	xpTools.xpLog(false, "ProxyReceiveFromRemoteProxy: proxy send message " + message["device_name"] + " (" + message["sender_ip_port"] + ") to server node received from: " + message["device_name"]);
                
                    xpProxyNode.forwardsAnnounce(message,xpProxyNode);
                }
            }

            else
            {
            //message received from other proxy, reply it from local node
            //1- lookup the ip_port from local table, accessing the table with recipient_session key
            //2- replace the sender_ip_port with proxy sender_ip_port
            //3- send message to local node
            
            if(pLocalNetworkTable[message["recipient_session"]] != undefined)
            {
            	xpTools.xpLog(false, "ProxyReceiveFromRemoteProxy: proxy send message " + message["device_name"] + " to local node: " + pLocalNetworkTable[message["recipient_session"]]["device_name"] + " (" + pLocalNetworkTable[message["recipient_session"]]["ip_port"] + ")");
                
                message["recipient_ip_port"] = pLocalNetworkTable[message["recipient_session"]]["ip_port"];
                message["sender_ip_port"] = xpProxyNode.descriptor["sender_ip_port"];

                var recipientAddress = message["recipient_ip_port"];
                recipientAddress = recipientAddress.substr(7,recipientAddress.lenght);
                var recipientIP = recipientAddress.split(":")[0];
                var recipientPort = recipientAddress.split(":")[1];
                var datagram = xpTools.toByteArray(message);

                var errorStr = "[xpProxyServer forwarding announce] Unable to send datagram\n";

                xpProxyNode.send_datagram(datagram, recipientPort, recipientIP, errorStr, "", xpProxyNode);
            }

        }
}



/*-------------------------- SERVER ------------------------------*/


wsServer = new WebSocketServer({
                                   httpServer: server,
                                   autoAcceptConnections: false
                               });


function originIsAllowed(origin) {
    // put logic here to detect whether the specified origin is allowed.
    return true;
}





var ids = new Object();
var id = 0;

var serverNode = null;

var xpProxyConnection = 0;
var xpProxyNode = 0;

name = xpTools.getAppEnviroment().proxy.name;
type = "XP_PROXY";
        
if(xpTools.localNodeIsServer() || xpTools.getAppEnviroment().proxy.forward)
{
	serverNode = new xpMobileNode(id, name, type, serverNode, process.argv[2]);
    serverNode.init();
    id ++;
}

var pRemoteNetworkTable = new Object();
var pLocalNetworkTable = new Object();


wsServer.on('request', function(request) {

    var connection = null;
    var node = null;
    
    
    if (!originIsAllowed(request.origin)) {
        // Make sure we only accept requests from an allowed origin
        request.reject();
        console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
        return;
    }
    
    connection = request.accept('echo-protocol', request.origin);
    console.log((new Date()) + ' Connection accepted.');
    

    connection.on('message', function(data) {

        var message = JSON.parse(data["utf8Data"]);
        //printHash(message);
        //console.log(data);

        if(connection.isProxy)
        {
			proxyForwardMessage(message);
            return;
        }
        
        if(message["message_type"] == 1) {

            connection.sendername = name;
            connection.senderaddress = request.origin;
            
            if(connection.isProxy == undefined && message["type"] == "XP_PROXY")
            	setProxyConnection(connection, serverNode);


            if(!connection.isProxy)
            {

				var name = message["name"];
            	var type = message["type"];
            
                if(ids[name] == undefined){
                    ids[name] = 1;
                }
                else {
                    ids[name] = ids[name] + 1;
                }

                name = name + "_" + ids[name];
                
                node = new xpMobileNode(id, name, type, node, process.argv[2]);
            	node.init();
            	id ++;

                node.newAnnounce = function(xpAnnounce, objRef) {
                    connection.send(JSON.stringify(xpAnnounce));
                }

                node.dispatchEvent = function(xpEvent, objRef) {
                    connection.send(JSON.stringify(xpEvent));
                }

                node.dispatchAction = function(xpAction, objRef) {
                    connection.send(JSON.stringify(xpAction));
                }
            }
        }
        
        if(message["message_type"] == 2) {
            
            node.notifyListeners(message);

        }
        
        if(message["message_type"] == 3) {

            if(message["device_name"] != "")
            {
                node.sendActionByName(message["device_name"], message, node);
            }
            else
            {
                var destination = new Object();
                destination["sender_ip_port"] = message["sender_ip_port"];
                destination["sender_session"] = "NULL";
                node.sendActionByProxy(destination, message, node);
                printHash(message);
            }
        }
        
        if(message["message_type"] == 4) {
            
            node.listenDevice(message["device_name"], message["__event_type"], node);
        }
           
        if (message.type === 'utf8') {
            console.log('Received Message: ' + message.utf8Data);
            
        }
        else if (message.type === 'binary') {
            console.log('Received Binary Message of ' + message.binaryData.length + ' bytes');
            
        }
    });
    connection.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
        if(node) node.delete();

        delete ids[connection.name];
        
    });
});



function printHash(hashTable) {
    for(key in hashTable){
        console.log(key+":"+ hashTable[key]+"\n");
    }
}

                
                


/*-------------------------- CLIENT ------------------------------*/

var client = 0;

if(xpTools.getAppEnviroment().proxy.forward)
{
	wsServerAddress = xpTools.getAppEnviroment().proxy.proxyIPPort;
	console.log("Server: " + wsServerAddress);
	
    client = new WebSocketClient();

    client.wsServerAddress = wsServerAddress;

    client.on('connectFailed', function(error) {
        console.log('wsProxyServer - Connect Error: ' + error.toString());
        setTimeout(proxyClientConnect, 5000);
    });

    client.on('connect', function(connection) {
        console.log('WebSocket client connected: ' + this.wsServerAddress);

		setProxyConnection(connection, serverNode);
        
        this.publish("xpProxy", "XP_PROXY");

        connection.wsServerAddress = this.wsServerAddress;

        connection.on('error', function(error) {
            console.log("wsProxyServer - Connection Error: " + error.toString());
        });
        connection.on('close', function() {
            console.log('wsProxyServer - Connection Closed');
            setTimeout(proxyClientConnect, 5000);
        });
        connection.on('message', function(data) {
        	var message = JSON.parse(data["utf8Data"]);
        	proxyForwardMessage(message);
        });

    });


    client.publish = function(name, type){
        var message = new Object();
        message["message_type"] = 1;
        message["name"] = name;
        message["type"] = type;
        xpProxyConnection.send(JSON.stringify(message));
    }

    proxyClientConnect();
}

function proxyClientConnect()
{
    if(client.wsServerAddress)
    {
        client.connect(client.wsServerAddress, 'echo-protocol');
    }
}

