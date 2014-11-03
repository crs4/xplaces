var fs = require('fs');
var FB = require('fb');
var storage = require('node-persist');
var util = require('util');
var path = require('path');
var randomstring = require('randomstring');
var swig = require('swig');
var http = require('http');
var arboreal = require('arboreal');

var Entities = require('html-entities').XmlEntities;

var xpMobileNode = require('xpMobileNode');
var xpTools = require('xpTools');

var request = require('request');

var XPDESKTOP = "xpDesktop";

var sinnova_contents = null;

var entities = new Entities();

var picture_url = "file:///code/install/sinnova/images/"
var http_server_url = "http://xpmobile.crs4.it:8080"
var hashtags = "#Sinnova2014 #Sinnova14 #SinnovaSocialWall #CRS4"
var hashtags_crs4 = "#Sinnova2014 #Sinnova14 #SinnovaSocialWall #CRS4"

function message_login(userid)
{
	return 'Sto usando Social Wall, la parete interattiva di Sinnova 2014. ' + hashtags_crs4 + ' -  http://nitweb.crs4.it/sinnova2014/upload/aziende.html?uid='+userid;
}

function message_preferred(company,url,userid)
{
	return 'Ho segnato ' + company + ' (' + url + ') tra i miei appunti. #Sinnova2014 #SinnovaSocialWall #CRS4. L\'elenco completo: http://nitweb.crs4.it/sinnova2014/upload/aziende.html?uid='+userid;
}

var send_action = true

storage.initSync();
FB.options({
           appId:          '239062392953403',
           appSecret:      'ffbe7c7ce16b9dfe31411a22883443ca',
           //redirectUri:   'http://156.148.70.68:8080/Sinnova.html',  
           });

var users;
var contents;    //user generated content
var tags;
var companies;
var interactions;

if(!storage.getItem('users')){
    users = {};
    storage.setItem('users', users);
}
else{
    users = storage.getItem('users');
}

if(!storage.getItem('contents')){
    contents = [];
    storage.setItem('contents', contents);
}
else{
	contents = storage.getItem('contents');
}



//set colors
function appendColor(id, color)
{
	colors[id] = {};
	colors[id].color = color;
};

var colors = {}
appendColor('0','#e21a1a');
appendColor('1','#00538a');
appendColor('2','#4fbbc5');
appendColor('3','#9ac61e');
appendColor('4','#d46817');



var templates = new Array();
loadTemplates();


var id = 0;
var name = "xpSinnova";
var type = "XP_SINNOVA";

var node = new xpMobileNode(id, name, type, {}, process.argv[2]);
node.init();

ignore_desktop_reset = false;

node.newAnnounce = announceListener;

var scaling_factor_sent = false;


function loginFB(destinationNode, fbcode, redirecturl)
{
	FB.napi('oauth/access_token', {
			client_id:      FB.options('appId'),
			client_secret:  FB.options('appSecret'),
			redirect_uri:   redirecturl,
			code:           fbcode
			}, function(err, result) {
			if(err){
					//send action error to client node xpmobilesinnova
					console.log("FB error 1:", err);
					//send action need new login
					var action = new Object();
					action['action_type'] = 0x01<<2;
					node.sendActionByProxy(destinationNode, action, node);
					return;
			}
					
			var accesstoken = result.access_token;
			
			FB.api('/me', {
					   fields:         'id, name, picture',
					   limit:          250,
					   access_token:   accesstoken
				   },
				   
				   function (result) {
					   if(!result || result.error) {
							console.log("FB error 2:", result.error);
							//send action need new login to client node xpmobilesinnova
							var action = new Object();
							action['action_type'] = 0x01<<2;
							node.sendActionByProxy(destinationNode, action, node);
							return;
					   }
				   
				   
					   //console.log("FB Query Result: ", result);
				   
				   
					   //store user info
					   if(users[result.id] == undefined)
					   {
					   		users[result.id] = {};
					   }
					   users[result.id].id = result.id;
					   users[result.id].name = result.name;
					   users[result.id].picture = result.picture.data.url;
					   users[result.id].access_token = accesstoken;
					   users[result.id].node = destinationNode;
					   users[result.id].current_session = node.descriptor["sender_session"];
					   storage.setItem('users',users);
					   console.log("Login, updated user info:", users);
					   
				   
					   var userid = result.id;
				   
					   sendActionLogin(userid, destinationNode);
					   
					   var body = message_login(userid);
					   postFacebook(body,accesstoken);
					
					   //send friends	
					   //getFBFriends(destinationNode, userid, accesstoken);
				   
					   //send bookmarks
					   sendUserContent(userid);
				   
			 		});
		}
	);
}

function postFacebook(body,accesstoken)
{
	
	FB.api('me/feed', 'post', { 
		access_token: accesstoken, 
		message: body
    }, function (res) {
	  if(!res || res.error) {
		console.log(!res ? 'error occurred' : res.error);
		return;
	  }
	  console.log('Post Id: ' + res.id);
	});
}

function postFacebookPhoto(userid,accesstoken,filename,message)
{
	if (!fs.existsSync(filename))
	{
		console.log(escapedPath + ' does not exists!')
		return
	}
	
	var url = 'https://graph.facebook.com/'+userid+'/photos?access_token='+accesstoken;
	
	var r = request.post(url, function optionalCallback (err, httpResponse, body) {
		if (err) {
			return console.error('upload failed:', err);
		  }
		  console.log('Upload photo successful!  Server responded with:', body);
		});
		
	var form = r.form()
	form.append('message', message + ' ' + hashtags + ' - Messaggio inviato da Sinnova 2014 Social Wall');
	form.append('source', fs.createReadStream(filename))
	console.log('Upload ok photo successful!');
}

function getFBFriends(destinationNode, userid, accesstoken)
{
	FB.api('/me/friends', {
		  limit:          5000,
		  fields:         'id, name, picture',
		  access_token:   accesstoken
	  }, 
	  
	  function (result) {
		  if(!result || result.error) {
			  console.log("FB friends error:", result.error);
			  //send action need new login
			  var action = new Object();
			  action['action_type'] = 0x01<<2;
			  node.sendActionByProxy(destinationNode, action, node);
			  return ;
		  }
	  
		  //console.log("FB friends result:", result);
		  console.log("FB get friends");
		  var u = users[userid];
		  u.friends = result;
		  users[userid] = u;
		  storage.setItem('users', users);
	  
		  for(k in result.data){
	  
		  //only friends that has already used the application
		  //if(users[result.data[k].id] != undefined ){
			var action = new Object();
				action['action_type'] = 0x01<<4;
				//action['friend_id'] = users[result.data[k].id].id;
				//action['friend_name'] = users[result.data[k].id].name;
				//action['friend_picture'] = users[result.data[k].id].picture.data.url;
				action['friend_id'] = result.data[k].id;
				action['friend_name']  = result.data[k].name;
				action['friend_picture'] = result.data[k].picture.data.url;
				node.sendActionByProxy(destinationNode, action, node);
			//}
	  
		  }
	  
	});
}

function sendActionLogin(userid, destinationNode)
{
	//send action to client node xpmobilesinnova
	var action = new Object();
	action['action_type'] = 0x01<<1;
	
	//remove access token from variables directed to client
	var user_data = new Object;
	user_data.id = users[userid].id;
	user_data.name = users[userid].name;
	user_data.picture = users[userid].picture;
	
	action['user_data'] = JSON.stringify(user_data);
	node.sendActionByProxy(destinationNode, action, node);

	var qml = templates['user']({id: userid, picture: "http://graph.facebook.com/"+userid+"/picture?type=large"});

	//send action to DESKTOP with users qml
	var action = new Object();
	action['action_type'] = 0x01;
	action['NAME'] = userid;
	action['CONTENT'] = qml;
	action["QMLFUNCTION"] = "showHelpMessage";
	node.sendActionByName(XPDESKTOP, action, node);

}


function sendUserContent(userid)
{
	if(users[userid].folder != undefined){
		for (i in users[userid].folder){
			var action = new Object();
	   		action['action_type'] = 0x01<<5;   //send user folder to client
	   		action['content'] = JSON.stringify(users[userid].folder[i]);
	   		node.sendActionByProxy(users[userid].node, action, node);
		}
	}
}


function logout(userid)
{
	var action = new Object();
	action["NAME"] = userid;
	action["action_type"] = 0x01 << 11;
	node.sendActionByName(XPDESKTOP, action, node);
	users[userid].current_session = ''
}






node.dispatchAction = function(action){
	console.log("Action Received: ", action);
	
	var destinationNode = {};
    destinationNode ["sender_ip_port"]  = action ["sender_ip_port"];
	destinationNode ["sender_session"]  = action ["sender_session"];

    if(action['action_type'] == 0x01){ //login
        
        var fbcode = action['fbcode'];
        var redirecturl = action['redirecturl'];
        
        console.log("------" + redirecturl);
        
		loginFB(destinationNode, fbcode, redirecturl);
            
	}
	else if(action['action_type'] == 0x01 << 3){ 
		
		var userid = action['fbuserid'];
		
		if(users[userid] == undefined)
		{
			var action = new Object();
			action['action_type'] = 0x01<<2;
			node.sendActionByProxy(destinationNode, action, node);
			console.log("Error relogin: ", userid);
		}
		else
		{
			sendActionLogin(userid, destinationNode);
			
			FB.api('oauth/access_token', {
				client_id:      FB.options('appId'),
				client_secret:  FB.options('appSecret'),
				grant_type: 'fb_exchange_token',
				fb_exchange_token: users[userid].access_token
			}, function (res) {
				if(!res || res.error)
				{
					console.log("Error relogin FB: ", !res ? 'error occurred' : res.error);
					return;
    			}
    			
    			//store user info
				users[userid].access_token = res.access_token;
				users[userid].node = destinationNode;
				users[userid].current_session = node.descriptor["sender_session"];
				storage.setItem('users',users);
				console.log("re-Login, updated user info:", users);
								
				//send friends
				//getFBFriends(destinationNode, userid, res.access_token);
		   
				//send bookmarks
				sendUserContent(userid);

			}
			);
		}  
    }
    
    
    //Upload, store url (image) and message, and send action to desktop
    else if(action['action_type'] == 0x01 << 5){ 
        
        
        
        var content = new Object();
        content.userid = action['fbuid'];
        content.url = action['url'];
        content.message = action['message'];
        
        contents.push(content);
        storage.setItem("contents", contents);
        
        var title = 'Messaggio inviato da: '+users[content.userid].name;
        
        entities = new Entities();
        var title = entities.encodeNonUTF(title);
        var message = entities.encodeNonUTF(content.message);
        
        var actionToSend = new Object();
        actionToSend["action_type"] = 0x01 << 0;
        var qml = templates['content']({parent: 'diariocoverflow', message: message, title: title, picture: content.url});
        //TODO accenti
        
        actionToSend["CONTENT"] = qml ;
        actionToSend["NAME"] = randomstring.generate();  //widget name
        actionToSend["PARENT"] = 'FUNCTION' ;  //widget name
        actionToSend["QMLFUNCTION"] = "appendToListModel";
        node.sendActionByName(XPDESKTOP,actionToSend, node);
        
        var filename = content.url.replace(http_server_url,__dirname).replace('xpApplicationServer','xplacesMobileTest');
        
        postFacebookPhoto(content.userid,users[content.userid].access_token,filename,content.message)
        
    }
    
    //Disable user desktop
    else if(action['action_type'] == 0x01 << 6 || action['action_type'] == 0x01 << 7)
    { 
    	var actionToSend = new Object();
        actionToSend["action_type"] = 0x01 << 0;
        
        var qml = templates['user_actions']({parent: action['userid']});
        //TODO accenti
        
        actionToSend["CONTENT"] = qml ;
        actionToSend["NAME"] = randomstring.generate();  //widget name
        actionToSend["PARENT"] = 'FUNCTION' ;  
        
        if(action['action_type'] == 0x01 << 6)
        	actionToSend["QMLFUNCTION"] = "disableUser()";
        else
        	actionToSend["QMLFUNCTION"] = "enableUser()";
        node.sendActionByName(XPDESKTOP,actionToSend, node);
    }
    
    else if(action['action_type'] == 0x01 << 11){ 
    	userid = action["NAME"];
    	users[userid].current_session = ''
    }
};


node.dispatchEvent = function(event){
    
    console.log("xpEvent:", util.inspect(event));
    if(event['__event_type'] == 0x01<<0 ){  // open content
        var source = event['source'];
        
        updateScaleClick(source);
    }
    else if(event['__event_type'] == 0x01<<1 ){  // close content
        var source = event['source'];
        
    }
    else if(event['__event_type'] == 0x01<<2 ){  // drop content
        var source = event['source']; //id azienda
        var target = event['target']; //userid
        
        //remove "company" from source string
        source = source.replace("company", "");
        console.log('Drop Event', source + " has been dropped in " + target); 
        
        var dropped_company = null;
        
        //search for dropped company
        dropped_company = companies[source];
        
        userid = target

		
 
        if(dropped_company && users[target] != undefined){
            if(users[target].folder === undefined){
                users[target].folder = new Array();
            }
            
            var folder = users[target].folder
            var alreadyDropped = false
            
            for(i in folder)
            {
            	if(folder[i].id == dropped_company.id)
            	{
            		alreadyDropped = true
            		break
            	}
            }
            
            if(!alreadyDropped)
            {
            
				var content = new Object();
				content.userid = target;
				content.id = dropped_company.id; //folder name
				content.picture = dropped_company.logo
				content.url = 'http://www.sinnovasardegna.it/aziende/'+dropped_company.id+'.html'
				content.name = dropped_company.nome;
			
				users[target].folder.push(content);
				storage.setItem('users', users);
			
				console.log(content)
			
			
				//send action to user device
				var action = new Object();
				action['NAME'] = target;
				action['action_type'] = 0x01<<5;   //update user folder
				action['content'] = JSON.stringify(content);

				node.sendActionByProxy(users[target].node, action, node);
			
			
				var body = message_preferred(content.name,content.url,target);
				postFacebook(body,users[target].access_token);
			
				updateScaleDrop(source);
			
				//send action to DESKTOP to show message
				var action = new Object();
				action['action_type'] = 25;
				action['NAME'] = userid;
				action["QMLFUNCTION"] = "showCompanyMessage";
				node.sendActionByName(XPDESKTOP, action, node);
				
				console.log('showCompanyMessage') 
			
				//upload contents to server
				upload(target);
			}
			else
			{
				//send action to DESKTOP to show message
				var action = new Object();
				action['action_type'] = 25;
				action['NAME'] = userid;
				action["QMLFUNCTION"] = "showDropErrorMessage";
				node.sendActionByName(XPDESKTOP, action, node);
				
				console.log('showDropErrorMessage') 
			}
        }
    }
    else if(event['__event_type'] == 0x01<<4 ){  // reset
    	send_action = true
		if(!ignore_desktop_reset)
			resetDesktop();
    }
   
    
};

//----------- Scaling

var max_scale = 1.5;
var min_scale = 1;
var weight_click = 1;
var weight_drop = 10;

var dataTree = new arboreal();

if(!storage.getItem('scalefactors')){
    var scalefactors = {};
    storage.setItem('scalefactors', scalefactors);
}
else{
	scalefactors = storage.getItem('scalefactors');
}



function sendScaleFactorsToDesktop()
{
	for(id in scalefactors)
	{
		var scalef = scalefactors[id].scalef;
		if(scalef == 0 || scalef == undefined)
			scalef = 1;
			
		setTimeout(sendScaleAction, 4000 + 6000 * Math.random(), id, scalef)
	}
	
	scaling_factor_sent = true;
	ignore_desktop_reset = false;
}






function updateScaleClick(nodeid)
{
	scalefactors[nodeid]['click']++;
	var treeItem = dataTree.find(nodeid);
	var parentItem = treeItem.parent;
	scalefactors[parentItem.id]['tot_children_click']++;
	
	updateScalingTraversal(treeItem.parent);
	
	storage.setItem('scalefactors', scalefactors);
}


function updateScaleDrop(nodeid)
{
	scalefactors[nodeid]['drop']++;
	var treeItem = dataTree.find(nodeid);
	var parentItem = treeItem.parent;
	scalefactors[parentItem.id]['tot_children_drop']++;
	
	updateScalingTraversal(treeItem.parent);
	
	storage.setItem('scalefactors', scalefactors);
}


function weightFunction(f)
{
	console.log("item",f);
	return f['click'] + f['drop'] * 10 + f['tot_children_click']/10 + f['tot_children_drop'];
}

function updateScalingTraversal(treeItem)
{

	console.log("updateScalingTraversal " + treeItem.id);
	
	if(treeItem.children.length > 0)
	{
		var weight = weightFunction(scalefactors[treeItem.children[0].id]);
		
		var min = weight;
		var max = weight;
	
		//get min and max
		for (var i in treeItem.children)
		{
			var child = treeItem.children[i];
			var childWeight = weightFunction(scalefactors[child.id]);
			
			scalefactors[child.id].weight = childWeight;
			
			if(childWeight < min)
				min = childWeight;
			if(childWeight > max)
				max = childWeight;
		}
	
		//update children node and send actions
		for (var i in treeItem.children)
		{
			var child = treeItem.children[i];

			var childWeight = scalefactors[child.id].weight;
			var scalef = childWeight == min ? min_scale
				: (childWeight / max) * (max_scale - min_scale) + min_scale;
				
			scalefactors[child.id].scalef = scalef;
			
			console.log(min + " " + max + " " + childWeight + " " + max_scale + " " + min_scale);
			
			sendScaleAction(child.id, scalef);
		}
	}
	
	if(treeItem.id != 'tree')
		updateScalingTraversal(treeItem.parent)
	
}

function sendScaleAction(nodeid, scalef)
{
	if(nodeid.indexOf('slide') >= 0)
		return;
		
	var action = new Object();
	action["NAME"] = nodeid;
	action["FACTOR"] = scalef;
	action["WIDTH"] = 100 * scalef;
	action["HEIGHT"] = 100 * scalef;
	//action["action_type"] = 0x01 << 10;
	action["action_type"] = 0x01 << 15;
	node.sendActionByName(XPDESKTOP, action, node);
		
	console.log("sendScaleAction:" + nodeid + " " + scalef + " " + 100 * scalef);
}

function createScaleDataItem()
{
	var f = {};
	f['click'] = 0;
	f['drop'] = 0;
	f['scalef'] = 1;
	f['weight'] = 0;
	f['tot_children_click'] = 0;
	f['tot_children_drop'] = 0;
	return f;
}


//----------- Load templates form Qml dir


function loadTemplates(){

    swig.setDefaults({ varControls: ['<<', '>>'], cache: false});
    fs.readdir(__dirname + "/Qml/", function(err, files){
               
         files.forEach(function(filename){
           templates[path.basename(filename, ".qml")] = swig.compileFile(__dirname + "/Qml/" + filename);
           console.log("Load template: " + filename);
        });
     });
    
    
}

function validString(string)
{
	if(string != undefined && string!= "" && string.match(/[a-zA-Z]/))
	{
		if(string.length > 500)
		{
			var croppedString = string.substring(0,500);
			var lastDotIndex = croppedString.lastIndexOf('.')
			croppedString = croppedString.substring(0,lastDotIndex+1)
			return croppedString;
		}
		return string
	}
	else return ''
}

function sendDemoSlide(treeCompany)
{
	var company_id = treeCompany.id;
	
	var slide_id = treeCompany.id + "_slide";
	
	var company = getCompany(company_id)
	
	var pillola1 = {};
	pillola1.text = ''
	pillola1.exist = false
	var pillola2 = {};
	pillola2.text = ''
	pillola2.exist = false
	var pillola3 = {};
	pillola3.text = ''
	pillola3.exist = false
	var youtube = {}
	youtube.text = ''
	youtube.exists = false
	
	var p = validString(company.pillole['A chi ci ispiriamo'])
	if(p!='')
	{
		pillola1.text = entities.encodeNonUTF(p);
		pillola1.exist = true
	}
	var p = validString(company.pillole['Chi ne beneficia'])
	if(p!='')
	{
		pillola2.text = entities.encodeNonUTF(p);
		pillola2.exist = true
	}
	p = validString(company.pillole['Il nostro modo di fare innovazione']);
	if(p!='')
	{
		pillola3.text = entities.encodeNonUTF(p);
		pillola3.exist = true
	}
	
	
	if(company.videoYoutube != undefined)
	{
		youtube.text = company.videoYoutube;
		youtube.text = youtube.text.replace('www.youtube.com/watch?v=','')
		youtube.text = youtube.text.replace('youtu.be/','');
		youtube.text = youtube.text.replace('&feature=player_detailpage','');
		youtube.text = youtube.text.replace('http://','');
		youtube.text = youtube.text.replace('https://','');
		
		if(company_id == '17280')
			youtube.text = 'https://www.youtube.com/watch?v=iPC9lGP-rys'
			
		youtube.exist = true
	}
	
	if(pillola1.text != '' || pillola2.text != '' || pillola3.text != '' || youtube.text != '')
	{
		var qml = templates['slides']({id: slide_id, youtube: youtube, pillola1: pillola1, pillola2: pillola2, pillola3: pillola3, parent: /*"company"+*/company_id});
		
		var action = new Object();
		action["NAME"] = slide_id;
		action["CONTENT"] = qml;
		action["PARENT"] = /*"company"+*/company_id;
		action["QMLFUNCTION"] = "appendToParentNode";
		action["action_type"] = 0x01;
		node.sendActionByName(XPDESKTOP, action, node);
		
		var treeSlide = treeCompany.appendChild(null,slide_id).children[0];
		
		console.log(slide_id);
	}
	else
	{
		console.log(slide_id + ' not present');
	}
	
	if(!scaling_factor_sent)
	{
		setTimeout(sendScaleFactorsToDesktop, 8000);
		scaling_factor_sent = true
	}
}


function sendCompanies(treeTag)
{
	var count = 0;
	
	for(i in companies){
	
		if(companies[i].tag != treeTag.id)
			continue;
			
		var comp = companies[i];
		
		console.log("company" + comp.id)
		
		var name = comp.nome.replace(/ [s].?[r].?[l].?/gi,'');
		name = name.replace(/ [s].?[p].?[a].?/gi,'');
		
		var name = entities.encodeNonUTF(name);
		
		var qml = templates['company']({id: comp.id, name: name, picture: comp.logo, parent: /*"tag"+*/comp.tag});
		
		var action = new Object();
		action["NAME"] = /*"company" +*/ comp.id;
        action["CONTENT"] = qml;
        action["PARENT"] = /*"tag"+*/comp.tag;
        action["QMLFUNCTION"] = "appendToParentNode";
        action["action_type"] = 0x01;
        node.sendActionByName(XPDESKTOP, action, node);
        

		
		var treeCompany = treeTag.appendChild(null,comp.id).children[count++];
		
		if(scalefactors[comp.id] == undefined)
		{
			scalefactors[comp.id] = createScaleDataItem(); 	
		}
		
		setTimeout(sendDemoSlide, 8000 + 4000 * Math.random(), treeCompany);
	}
}


function sendSubTags(treeTag)
{
	var count = 0;
	
	for(i in tags)
    {    	
    	if(treeTag.id != tags[i].tag)
			continue;
			
		var name = entities.encodeNonUTF(tags[i].nome);
    
    	var qml = templates['tag']({id: tags[i].id, name: name, picture: picture_url + tags[i].id + '.png', color: colors[treeTag.id].color, parent: treeTag.id});
		var action = new Object();
		action["NAME"] = tags[i].id;
		action["CONTENT"] = qml;
		action["PARENT"] = treeTag.id;
		action["QMLFUNCTION"] = "appendToParentNode";
		action["action_type"] = 0x01;
		node.sendActionByName(XPDESKTOP, action, node);
		
		var treeSubTag = treeTag.appendChild(null,tags[i].id).children[count++];
		
		if(scalefactors[tags[i].id] == undefined)
		{
			scalefactors[tags[i].id] = createScaleDataItem(); 	
		}
		
		setTimeout(sendCompanies, 3000 + 8000 * Math.random(), treeSubTag);
    }
}

function sendAllContentToDesktop()
{
	//create new tree for scaling
	dataTree = new arboreal();
	var treeRoot = dataTree.appendChild(null,'tree').children[0];
    
    for(i in tags)
    {
    	if(tags[i].tag != undefined)
			continue;
			
		var name = entities.encodeNonUTF(tags[i].nome);
			
		var qml = templates['tag']({id: tags[i].id, name: name, picture: picture_url + tags[i].id + '.png', color: colors[tags[i].id].color});
		var action = new Object();
		action["NAME"] = tags[i].id;
		action["CONTENT"] = qml;
		action["action_type"] = 0x01;
		node.sendActionByName(XPDESKTOP, action, node);

	
		var treeTag = treeRoot.appendChild(null,tags[i].id).children[i];
	
		if(scalefactors[tags[i].id] == undefined)
		{
			scalefactors[tags[i].id] = createScaleDataItem(); 	
		}
		
		if(tags[i].haveSubTags)
    		setTimeout(sendSubTags, 2000 + 1000 * Math.random(),treeTag);
    	else
			setTimeout(sendCompanies, 5000 + 8000 * Math.random(),treeTag);
    }
    
    sendAlreadyLoggedUsersToDesktop();
}


function resetDesktop(cb)
{
	ignore_desktop_reset = true;
	scaling_factor_sent = false;
	
	var action = new Object();
    action["action_type"] = 0x01 << 13; //clear desktop
    node.sendActionByName(XPDESKTOP, action, node);
    
    //populate dekstop
	setTimeout(sendAllContentToDesktop, 1000);
	
	setTimeout(createDiario, 500);
}

function createDiario()
{
	var qml = templates['tag']({id: 'diariosinnova', name: '', parent: 'diariosinnova', picture: picture_url + "diario.png", color: '#04a88a'});
	
	var action = new Object();
	action["NAME"] = 'diariosinnova';
	action["CONTENT"] = qml;
	action["action_type"] = 0x01;
	node.sendActionByName(XPDESKTOP, action, node);
	
	setTimeout(createDiarioCoverflow, 500);
}

function createDiarioCoverflow()
{
	var qml = templates['diario']({id: 'diariocoverflow', name: "Slides", parent: 'diariosinnova'});

	var action = new Object();
	action["NAME"] = 'diariocoverflow';
	action["CONTENT"] = qml;
	action["PARENT"] = 'diariosinnova';
	action["QMLFUNCTION"] = "appendToParentNode";
	action["action_type"] = 0x01;
	node.sendActionByName(XPDESKTOP, action, node);
}

function announceListener(announce)
{
	//check if desktop is new or re-new
	if(announce['device_name'] == XPDESKTOP && announce['status'] == "New")
	{
		send_action = false
		
		readSinnovaJSONcontentsFromFile();
		//readSinnovaJSONcontents()
		
		//do not reset
		//populate dekstop
		setTimeout(sendAllContentToDesktop, 1000);
	
		setTimeout(createDiario, 500);
		node.listenDevice(XPDESKTOP, 0xff, node);   //listen all event from Desktop	
	}
	
	//check if a user is dead
	if(announce['status'] == "Dead" && announce['device_name'].indexOf('xpSinnovaMobile') >= 0)
	{
		for(userid in users)
		{
			if(users[userid].node['sender_session'] == announce['sender_session'])
				setTimeout(logout, 1000, userid);
		}
	}
	
}

function sendAlreadyLoggedUsersToDesktop()
{
	for(i in users)
	{
		console.log("sendAlreadyLoggedUsersToDesktop: " + users[i].current_session + "    " + node.descriptor["sender_session"]);
		if(users[i].current_session == node.descriptor["sender_session"])
		{
			var qml = templates['user']({id: users[i].id, picture: "http://graph.facebook.com/"+users[i].id+"/picture?type=large"});

			//send action to DESKTOP with users qml
			var action = new Object();
			action['action_type'] = 0x01;
			action['NAME'] = users[i].id;
			action['CONTENT'] = qml;
			node.sendActionByName(XPDESKTOP, action, node);
		}
	}
}

function getSinnovaContents()
{
	tags = {};
	for( i in sinnova_contents.tags)
	{
		tags[sinnova_contents.tags[i].id] = sinnova_contents.tags[i];
		tags[sinnova_contents.tags[i].id].haveSubTags = false;
	}
	
	console.log('tags', tags)
	for( key in tags)
	{
		console.log('parent of ' + key,tags[tags[key].tag])
		if(tags[key].tag != undefined)
		{
			tags[tags[key].tag].haveSubTags = true;
		}
	}
	
	companies = {}
	for( i in sinnova_contents.aziende)
	{
		companies[sinnova_contents.aziende[i].id] = sinnova_contents.aziende[i];
	}
}

function readSinnovaJSONcontentsFromFile()
{
	fs.readFile(__dirname + "/data/aziende.json.all.inc", function (err,data) {

		if (err) {
			console.log("error", err);
			return;
		}
		
		sinnova_contents = JSON.parse(data);
		getSinnovaContents();
		loadTemplates();
	});
}

function readSinnovaJSONcontents()
{
	var url = 'http://www.sinnovasardegna.it/aziende.json.inc';

	http.get(url, function(res) {
		var body = '';

		res.on('data', function(chunk) {
			body += chunk;
		});

		res.on('end', function() {
			sinnova_contents = JSON.parse(body);
			getSinnovaContents();
			loadTemplates();
			
		});
	}).on('error', function(e) {
		  console.log("Got error: ", e);
	});
}

function getCompany(id)
{
	for(i in companies)
	{
		if(companies[i].id == id)
			return companies[i];
	}
}



function upload(userid)
{
	var folder = users[userid].folder;
	var content = [];
	
	for(i in folder)
	{
		content.push(getCompany(folder[i].id));
	}
	
	
	
	swig.renderFile(__dirname+'/html/template_sinnova.json',{'folder':content, 'name': users[userid].name}, function (err, json)
	{
		if (err) {
			throw err;
		}
		
		swig.renderFile(__dirname+'/html/template_sinnova.html',{'folder':content}, function (err, html)
		{
	  	  	if (err) {
				throw err;
			}
			
			var r = request.post('http://nitweb.crs4.it/sinnova2014/upload_sinnova.php', function optionalCallback (err, httpResponse, body) {
			//var r = request.post('http://localhost:8888/upload_file.php', function optionalCallback (err, httpResponse, body) {
			if (err) {
				return console.error('upload failed:', err);
			  }
			  console.log('Upload successful!  Server responded with:', body);
			});
		
			var form = r.form()
			form.append('html_filename', userid+'.html');
			form.append('json_filename', userid+'.json');
			form.append('html_content', html);
			form.append('json_content', json);
			form.append('auth_key', 'asfawqf7qeyfq8fqwe7qe8qq7wf8q8q');
		
			console.log('http://nitweb.crs4.it/sinnova2014/upload/sinnovasardegna.html?uid='+userid);
			console.log('http://nitweb.crs4.it/sinnova2014/upload/'+userid+'.html');
		});
	});
}