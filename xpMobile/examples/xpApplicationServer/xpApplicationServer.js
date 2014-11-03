var fs = require('fs');
var vm = require('vm');
var swig  = require('swig');



var xpMobileNode = require('xpMobileNode');
var xpTools = require('xpTools');
var util = require('util');
var path = require('path');

var dirname = "./apps/";
var id = 0;
var name = "xpApplicationServer";
var type = "XP_APPLICATIONSERVER";


var locals = { 
            now: function () { return new Date(); },
            test: function () { return false },
                }
                
swig.setDefaults({ 
                cache: false,
                autoescape: false,
                locals: locals           
 });


var apps = new Object();
var serverNode = new xpMobileNode(id, name, type, serverNode, process.argv[2]);
serverNode.init();
id++;

setInterval(callback, 5000);
fs.readdir(dirname, readdir);

function readdir(err, files){
    
    files.forEach(function(filename){
                  if(path.extname(filename) == '.js') {
                    fs.readFile(dirname + filename,  function(err, data){
                              
                              if (err) throw err;
                              var appName = path.basename(filename, '.js');
                              var sandbox = {require:require, setInterval: setInterval, console: console, util: util};
                           
                              vm.runInNewContext(data, sandbox);
                              var node = new xpMobileNode(id, sandbox[appName].appName, sandbox[appName].type, node, process.argv[2]);
                              sandbox[appName](node);
                                
                              apps[appName] = node;
                              var event = new Array();
                                event['__event_type'] = 0x01<<0;
                                event['status'] = 'STARTED';
                                event['app_name'] = appName;
                                serverNode.notifyListeners(event);
                                
                              id++;
 
                    });
                  }
                  
                  else if(path.extname(filename) == '.json') {
                    fs.readFile(dirname + filename,  function(err, data){
                              
                              if (err) throw err;
                                var appName = path.basename(filename, '.json');
                                var sandbox = {require:require, setInterval: setInterval, console: console, util: util};
                                var ctx = vm.createContext(sandbox);
                                vm.runInContext(data, ctx);
                                
                                var properties = ctx[appName].properties;
                                var node = new xpMobileNode(id, ctx[appName].name, ctx[appName].type, ctx, process.argv[2]);
                                apps[appName] = node;
                                id++;
                                node.device_properties = JSON.stringify(properties);
                                node.init();
                                                                
                                for(i = 0; i < ctx[appName].listeners.length; i++){
                                
                                    var ev = ctx[appName].listeners[i];
                                    node.listenDevice(ev.deviceName, ev.mask, node);
                                    console.log("-----", ev.deviceName);
                                    ctx[appName].devices[ev.deviceName] = new Object();
                                    ctx[appName].devices[ev.deviceName].events = new Array();
                                    ctx[appName].devices[ev.deviceName].events[ev.mask] = new Object();
                                }
                                //console.log(properties);
                                
                                
                                
                                node.dispatchAction = ctx[appName].actionReceived;
                                node.dispatchEvent = ctx[appName].eventReceived;
                                setTimeout(ctx[appName].init, 1, {node: node, ctx: ctx[appName]});
                                setInterval(ctx[appName].start, 1000, {node: node, ctx: ctx[appName]});
                                var event = new Array();
                                event['__event_type'] = 0x01<<0;
                                event['status'] = 'STARTED';
                                event['app_name'] = appName;
                                serverNode.notifyListeners(event);
                                
                    });

                  }
                  
        });
}



fs.watch("./apps", function (event, filename) {
         console.log('event is: ' + event);
         if (filename) {
         console.log('filename provided: ' + filename);
         } else {
         console.log('filename not provided');
         }
         });




function callback(){
    var i;
    var appsKeys = Object.keys(apps);
    for(i = 0; i < appsKeys.length; i++) {
        var event = new Object();
        event['__event_type'] = 0x01<<0;
        event['status'] = 'RUNNING';
        event['app_name'] = appsKeys[i];
        serverNode.notifyListeners(event);
    }
    
    
}


serverNode.dispatchAction = function(action){
	console.log("Action Received: ", action);
    
    if(action['action_type'] == 0x01<<0) {
    console.log(action);
       action.ruledata = JSON.parse(action.ruledata);
       swig.renderFile(__dirname + "/apptemplates/template.json", action, function (err, output) {
          if (err) {
            throw err;
          }
          else{
                fs.writeFile("./apps/" + action['rulename'] + ".json", output, function(err) {
                    if(err) {
                    console.log(err);
                } else {
                 
                 console.log("The file was saved!");
                 }
                 });
    
            }

        });
       
 }       

}





var toString = Object.prototype.toString;

function dump_object(obj) {
    var buff, prop;
    buff = [];
    for (prop in obj) {
        buff.push(dump_to_string(prop) + ': ' + dump_to_string(obj[prop]))
    }
    return '{' + buff.join(', ') + '}';
}

function dump_array(arr) {
    var buff, i, len;
    buff = [];
    for (i=0, len=arr.length; i<len; i++) {
        buff.push(dump_to_string(arr[i]));
    }
    return '[' + buff.join(', ') + ']';
}

function dump_to_string(obj) {
    if (toString.call(obj) == '[object Function]') {
        return obj.toString();
    } else if (toString.call(obj) == '[object Array]') {
        return dump_array(obj);
    } else if (toString.call(obj) == '[object String]') {
        return '"' + obj.replace('"', '\\"') + '"';
    } else if (obj === Object(obj)) {
        return dump_object(obj);
    }
    return obj.toString();
}
