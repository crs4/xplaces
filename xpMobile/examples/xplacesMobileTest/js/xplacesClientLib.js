

function xpNetwork(){
            var remoteAddress = 'ws://' + location.host;
            var connection = new WebSocket(remoteAddress, 'echo-protocol');
            this.connection = connection;
            var eventListener = null;
            var announceListener = null;
            var actionListener = null;
            //console.log(connection);

            this.connection.onopen = function() {
                console.log('WebSocket client connected');
                setTimeout(xpReady, 10);
                
            };
                        
            this.connection.onclose = function() {
                console.log('echo-protocol Connection Closed');
            };
                       
            this.connection.onmessage =  function(data) {
              
                var message = JSON.parse(data.data);
                //console.log(message);
                
                
                    if(message["message_type"] == 2 ){
                        if(eventListener) eventListener(message);
                    }
                        
                    else if(message["message_type"] == "1" ){
                        if(announceListener) announceListener(message);
                    }      
                    
                    else if(message["message_type"] == 3 ){
                        if(actionListener) actionListener(message);
                    }    
                    
                };
                         
                           
            
            this.publish = function(name, type){
                var message = new Object();
                message["message_type"] = 1;
                message["name"] = name;
                message["type"] = type;
                this.connection.send(JSON.stringify(message));
            }
            
            this.addAnnounceListener = function(l){
                announceListener = l;
            }
            
            this.addActionListener = function(l){
                actionListener = l;
            }
            
            this.addEventListener = function(deviceName, eventMask, l){
                //console.log("AddListener");
                var message = new Object();
                message["message_type"] = 4;
                message["device_name"] = deviceName;
                message["__event_type"]  = eventMask;
                
                eventListener = l;
                this.connection.send(JSON.stringify(message));            
            }
            
            this.sendEvent = function(event){
                event["message_type"] = 2;
                this.connection.send(JSON.stringify(event));                
            }
            
            
            this.sendAction = function(destName, action){
                action["message_type"] = 3;
                action["device_name"] = destName;
                this.connection.send(JSON.stringify(action));
            }
            
        }

function startDeviceOrientation(network, cbfunction){
    if (window.DeviceOrientationEvent) {
        
        window.addEventListener('deviceorientation', function(event) {
                                var tiltLR = event.gamma;
                                // beta is the front-to-back tilt in degrees, where front is positive
                                var tiltFB = event.beta;
                                // alpha is the compass direction the device is facing in degrees
                                var dir = event.alpha
                                var message = new Object();
                                message["__event_type"] = 8;
                                message["X"] = tiltLR;
                                message["Y"] = tiltFB;
                                message["Z"] = dir;
                                
                                if(cbfunction) cbfunction(tiltLR, tiltFB, dir);
                                if(network) network.sendEvent(message);
                                }, true);
    }
}


        //END LIBRARY