var xpMobileNode = require('xpMobileNode');


var node1, node2;


var id = 0;
var name = "xpSinnova";
var type = "XP_SINNOVA";

var node1 = new xpMobileNode(id, name, type, {}, process.argv[2]);
node1.init();

id = 1;
name = "xpSinnova2";
type = "XP_SINNOVA";
var node2 = new xpMobileNode(id, name, type, {}, process.argv[2]);
node2.init();


var action = new Object();
var value = "";
for(i=0; i<4092; i++) value = value + "1";

action["pippo"] = value;
node2.sendActionByName("xpSinnova", action, node2);

node1.dispatchAction = function(action){

    console.log(action);
}

