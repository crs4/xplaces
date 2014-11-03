 var properties = new Array();
                            var object = {
                                type: "XP_ARDUINO",
                                events: {
                                    256 : {name: "Analog Pin", mask: 256, keys: ["A0", "A1", "A2", "A3", "A4", "A5"  ], keyoptions: {
                                        A0: {title: "Analog Pin 0", type: "number", min: 0, max: 1024},
                                        A1: {title: "Analog Pin 1", type: "number", min: 0, max: 1024},
                                        A2: {title: "Analog Pin 2", type: "number", min: 0, max: 1024},
                                        A3: {title: "Analog Pin 3", type: "number", min: 0, max: 1024},
                                        A4: {title: "Analog Pin 4", type: "number", min: 0, max: 1024},
                                        A5: {title: "Analog Pin 5", type: "number", min: 0, max: 1024}
                                        }
                                    }
                                    
                                },
                                actions: {
                                    1: {name: "Configuration", mask: 1, keys: ["LT", "A0", "A1", "A2", "A3", "A4", "A5"], keyoptions: {
                                        LT: {title: "Delay Time",   type: "range" , min: 10 , max: 1000, step: 10},
                                        A0: {title: "Analog Pin 0", type: "range", min: 0 , max: 1},
                                        A1: {title: "Analog Pin 1", type: "range", min: 0 , max: 1},
                                        A2: {title: "Analog Pin 2", type: "range", min: 0 , max: 1},
                                        A3: {title: "Analog Pin 3", type: "range", min: 0 , max: 1},
                                        A4: {title: "Analog Pin 4", type: "range", min: 0 , max: 1},
                                        A5: {title: "Analog Pin 5", type: "range", min: 0 , max: 1}
                                 
                                        }
                                    
                                    },
                                    2: {name: "Digital Pin Settings", mask: 2, keys: ["D2", "D3", "D12", "D13"], keyoptions: {
                                        
                                        D2:  {title: "Digital Pin 2",  type: "range", min: 0, max: 1},
                                        D3:  {title: "Digital Pin 3",  type: "range", min: 0, max: 1},
                                        D4:  {title: "Digital Pin 4",  type: "range", min: 0, max: 1},
                                        D5:  {title: "Digital Pin 5",  type: "range", min: 0, max: 1},
                                        D6:  {title: "Digital Pin 6",  type: "range", min: 0, max: 1},
                                        D7:  {title: "Digital Pin 7",  type: "range", min: 0, max: 1},
                                        D8:  {title: "Digital Pin 8",  type: "range", min: 0, max: 1},
                                        D9:  {title: "Digital Pin 9",  type: "range", min: 0, max: 1},
                                        D10: {title: "Digital Pin 10", type: "range", min: 0, max: 1},
                                        D11: {title: "Digital Pin 11", type: "range", min: 0, max: 1},
                                        D12: {title: "Digital Pin 12", type: "range", min: 0, max: 1},
                                        D13: {title: "Digital Pin 13", type: "range", min: 0, max: 1},
                                        
                                        }
                                    
                                    },
                                    
                                }
                                
                            };
                        
                        
                        var object2 = {
                            type: "XP_ORIENTATION",
                            events: {
                                8: {name: "Orientation", mask: 8, keys: ["X", "Y", "Z" ] , chart: 'bubble'  }
                            },
                            actions: {
                                1: {name: "Desktop", mask: 1, keys: ["URL", "NAME"], dropTarget: true},
                                2: {name: "Desktop2", mask: 2, keys: ["URL", "NAME"], dropTarget: true}
                                
                            }
                            
                        }
                        
                        var object3 = {
                            type: "XP_DESKTOP",
                            events: {
                                
                            },
                            actions: {
                            
                                1: {name: "Add Content", mask: 1, keys: ["NAME", "CONTENT", "QMLCOMPONENT", "QMLFUNCTION", "PARENT"], dropTarget: true},
                                2: {name: "Play Video", mask: 2, keys: ["NAME"]},
                                4: {name: "Pause Video", mask: 4, keys: ["NAME"]},
                                8: {name: "Stop Video", mask: 8, keys: ["NAME"]},
                                16: {name: "Seek Video", mask: 16, keys: ["NAME", "SEEK"], keyoptions:{
                                NAME:{ title: "Name"},
                                SEEK:{ title: "Seek", type: "range", min: 0 , max: 1, step:0.1 }
                                        }},
                                2048: {name: "Remove Content", mask: 2048, keys: ["NAME", "QMLCOMPONENT", "QMLFUNCTION"]}
                                
                                
                            }
                            
                        }


			var object4 = {
                            type: "CAMERA",
                            events: {
                                2: {name: "Camera Shadow", mask: 2, keys: ["number_of_shadows", "x0", "x1", "midpoint"] },
                            },
                            actions: {
                                32: {name: "Camera Config", mask: 32, keys: ["frozen", "width", "height", "horizonP1/x", "horizonP1/y", "horizonP2/x", "horizonP2/y"],
                                                        keyoptions: {
                                                        frozen: {title: "Frozen", type: "range", min: 0 , max: 1},
                                                        width:  {title: "Width", type :"range"},
                                                        height:  {title: "Height", type :"range"},
                                                        "horizonP1/x": {title: "Horizon P1 X", type: "range", min: 0 , max: 600 },
                                                        "horizonP1/y": {title: "Horizon P1 Y", type: "range", min: 0 , max: 600 },
                                                        "horizonP2/x": {title: "Horizon P2 X", type: "range", min: 0 , max: 600 },
                                                        "horizonP2/y": {title: "Horizon P2 Y", type: "range", min: 0 , max: 600 },
                                                        },
                                                        state: true 

                            },
                                
                                
                            }
                            
                        }


			var object5 = {
                            type: "XP_FINGER_MANAGER",
                            events: {
                                4096: {name: "Trackable Event", mask: 4096, keys: ["number_of_trackable", "cx", "cy", "state"] },
                            },
                            actions: {
                                
                                
                            }
                            
                        }

                        
            var object6 = {
                            type: "XP_DUMMY",
                            events: {
                                256: {name: "dummyvalue", mask: 256, keys: ["dummyvalue" ] }
                            },
                            actions: {
                                1: {name: "Test Action", mask: 1, keys: ["NAME", "CONTENT"], dropTarget: true}
                            }

                        }



                    var object7 = {
                        type: "XP_DESKTOPMOBILE",
                        events: {
                            
                        },
                        actions: {
                            1: {name: "Add Content", mask: 1, keys: ["NAME", "CONTENT"], dropTarget: true},
                            2: {name: "Play Video", mask: 2, keys: ["NAME"]},
                            4: {name: "Pause Video", mask: 4, keys: ["NAME"]},
                            8: {name: "Stop Video", mask: 8, keys: ["NAME"]},
                            16: {name: "Seek Video", mask: 16, keys: ["NAME", "SEEK"], keyoptions:{
                            NAME:{ title: "Name"},
                            SEEK:{ title: "Seek", type: "range", min: 0 , max: 1, step:0.1 }
                            }
                                
                                
                                
                            }
                            
                        }
                            
                        }


var object8 = {
type: "XP_APPLICATIONSERVER",
events: {
    1: {name: "Number", mask: 1, keys: ["number"]},
},
actions: {
    1: {name: "Action", mask: 1, keys: ["content"]},
    }
        
        
        
    }
    




                        properties["XP_ARDUINO"] = object;
                        properties["XP_ORIENTATION"] = object2;
                        properties["XP_DESKTOP"] = object3;
                        properties["CAMERA"] = object4;
                        properties["XP_FINGER_MANAGER"] = object5;
                        properties["XP_DUMMY"] = object6;
                        properties["XP_DESKTOPMOBILE"] = object7;
                        properties["XP_APPLICATIONSERVER"] = object8;



