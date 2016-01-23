"use strict";
(function socket_js() {
    var moduleName = "lib/wSocket/socket"
    
    var defineArray = [];
    defineArray.push("lib/utils/subscribable");
    defineArray.push("lib/wType/event");
    defineArray.push("lib/wType/bytearray");
    defineArray.push("lib/wType/string");
    defineArray.push("lib/wType/vocabulary");
    defineArray.push("lib/wType/uint64");
    defineArray.push("lib/wType/address");
    
    define(moduleName, defineArray, function socket_module() {
        var Subscribable = require("lib/utils/subscribable");
        var Event = require("lib/wType/event");
        var ByteArray = require("lib/wType/bytearray");
        var String = require("lib/wType/string");
        var Vocabulary = require("lib/wType/vocabulary");
        var Uint64 = require("lib/wType/uint64");
        var Address = require("lib/wType/address");
        
        var Socket = Subscribable.inherit({
            "className": "Socket",
            "constructor": function(name) {
                Subscribable.fn.constructor.call(this);
                
                this._state = DISCONNECTED;
                
                if (!name) {
                    throw new Error("Can't construct a socket without a name");
                }
                
                this._name = name instanceof String ? name : new String(name);
                this._remoteName = new String();
                this._id = new Uint64(0);
                this._socket;
                
                this.on("connected", this.onConnected);
                this.on("disconnected", this.onDisconnected);
                this.on("__msg__", this.on__msg__);
            },
            "destructor": function() {
                this.close();
                if (this._state === DISCONNECTING) {
                    this.on("disconnected", function() {
                        Subscribable.fn.destructor.call(this);
                    })
                } else {
                    Subscribable.fn.destructor.call(this);
                }
            },
            "close": function() {
                if ((this._state !== DISCONNECTED) && (this._state !== DISCONNECTING)) {
                    this._state = DISCONNECTING;
                    this._socket.close();
                }
            },
            "open": function(addr, port) {
                var that = this;
                
                if (this._state === DISCONNECTED) {
                    this._state = CONNECTING;
                    this._socket = new WebSocket("ws://"+ addr + ":" + port);
                    this._socket.binaryType = "arraybuffer";
                    
                    this._socket.onopen = function() {
                        that.trigger("__cnt__");
                    }
                    this._socket.onclose = function(ev) {
                        that.trigger("disconnected", ev);
                    }
                    this._socket.onerror = function(err) {
                        that.trigger("error", err);
                    }
                    this._socket.onmessage = function(mes) {
                        var raw = new ByteArray(new Uint8Array(mes.data));
                        
                        var ev = raw[">>"]();
                        
                        if (!(ev instanceof Event)) {
                            throw new Error("Wrong format of websocket data");
                        }
                        that.trigger("__msg__", ev);
                    }
                }
                
                this._uncyclic.push(function() {
                    that = undefined;
                });
            },
            "send": function(ev) {
                var ba = new ByteArray();
                ba["<<"](ev);
                
                this._socket.send(ba.toArrayBuffer());
            },
            "onConnected": function() {
                this._state = CONNECTED;
            },
            "onDisconnected": function(ev) {
                this._id.destructor();
                this._id = new Uint64(0);
                
                this._remoteName.destructor();
                this._remoteName = new String();
                
                console.log(ev);
            },
            "on__msg__": function(ev) {
                if (ev.isSystem()) {
                    var cmd = ev._data.at("command").toString();
                    
                    switch(cmd) {
                        case "setId":
                            this._setId(ev._data.at("id"));
                            this._setRemoteName();
                            break;
                        case "setName":
                            this._setName(ev._data.at("name"));
                            this.trigger("connected");
                            break;
                        default:
                            throw new Error("Unknown system command: " + cmd);
                    }
                } else {
                    trigger("message", ev);
                }
                ev.destructor();
            },
            "_setId": function(id) {
                if ((this._state === CONNECTING) && (this._id.valueOf() === 0)) {
                    this._id.destructor();
                    this._id = id.clone();
                } else {
                    throw new Error("An attempt to set id in unexpected time");
                }
            },
            "_setName": function(name) {
                 if ((this._state === CONNECTING) && (this._id.valueOf() !== 0)) {
                    this._remoteName.destructor();
                    this._remoteName = name.clone();
                } else {
                    throw new Error("An attempt to set name in unexpected time");
                }
            },
            "_setRemoteName": function() {
                var vc = new Vocabulary();
                vc.insert("command", new String("setName"));
                vc.insert("name", this._name.clone());
                
                var ev = new Event(new Address(), vc, true);
                ev.setSenderId(this._id.clone());
                this.send(ev);
                
                ev.destructor();
            }
        });
        
        return Socket;
    });
    
    var DISCONNECTED = 111;
    var DISCONNECTING = 110;
    var CONNECTING = 101;
    var CONNECTED = 100;
})();