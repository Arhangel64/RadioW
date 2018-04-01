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
    defineArray.push("lib/wType/factory");
    
    define(moduleName, defineArray, function socket_module() {
        var Subscribable = require("lib/utils/subscribable");
        var Event = require("lib/wType/event");
        var ByteArray = require("lib/wType/bytearray");
        var String = require("lib/wType/string");
        var Vocabulary = require("lib/wType/vocabulary");
        var Uint64 = require("lib/wType/uint64");
        var Address = require("lib/wType/address");
        var factory = require("lib/wType/factory");
        
        var Socket = Subscribable.inherit({
            "className": "Socket",
            "constructor": function(name) {
                if (!name) {
                    throw new Error("Can't construct a socket without a name");
                }
                Subscribable.fn.constructor.call(this);
                
                this._state = DISCONNECTED;
                this._dState = SIZE;
                this._name = name instanceof String ? name : new String(name);
                this._remoteName = new String();
                this._id = new Uint64(0);
                this._socket = undefined;
                this._helperBuffer = new ByteArray(4);
                
                this._initProxy();
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
            "_emitEvent": function(ev) {
                this.trigger("message", ev);
                ev.destructor();
            },
            "getId": function() {
                return this._id;
            },
            "isOpened": function() {
                return this._state === CONNECTED;
            },
            "open": function(addr, port) {
                if (this._state === DISCONNECTED) {
                    this._state = CONNECTING;
                    this._socket = new WebSocket("ws://"+ addr + ":" + port);
                    this._socket.binaryType = "arraybuffer";
                    
                    this._socket.onclose = this._proxy.onSocketClose;
                    this._socket.onerror = this._proxy.onSocketError;
                    this._socket.onmessage = this._proxy.onSocketMessage
                }
            },
            "send": function(ev) {
                var size = ev.size();
                var ba = new ByteArray(size + 5);
                ba.push32(size);
                ba.push8(ev.getType());
                ev.serialize(ba);
                
                this._socket.send(ba.data().buffer);
            },
            "_initProxy": function() {
                this._proxy = {
                    onSocketClose: this._onSocketClose.bind(this),
                    onSocketError: this._onSocketError.bind(this),
                    onSocketMessage: this._onSocketMessage.bind(this)
                }
            },
            "_onEvent": function(ev) {
                if (ev.isSystem()) {
                    var cmd = ev._data.at("command").toString();
                    
                    switch(cmd) {
                        case "setId":
                            this._setId(ev._data.at("id"));
                            this._setRemoteName();
                            break;
                        case "setName":
                            this._setName(ev._data.at("name"));
                            this._state = CONNECTED;
                            this.trigger("connected");
                            break;
                        default:
                            throw new Error("Unknown system command: " + cmd);
                    }
                    ev.destructor();
                } else {
                    setTimeout(this._emitEvent.bind(this, ev), 5);      //TODO event queue
                }
            },
            "_onSocketClose": function(ev) {
                this._state = DISCONNECTED;
                
                this._id.destructor();
                this._id = new Uint64(0);
                
                this._remoteName.destructor();
                this._remoteName = new String();
                
                console.log(ev);
                this.trigger("disconnected", ev);
            },
            "_onSocketError": function(err) {
                this.trigger("error", err);
            },
            "_onSocketMessage": function(mes) {
                var raw = new Uint8Array(mes.data);
                var i = 0;
                
                while (i < raw.length) {
                    switch (this._dState) {
                        case SIZE:
                            i = this._helperBuffer.fill(raw, raw.length, i);
                            
                            if (this._helperBuffer.filled()) {
                                var size = this._helperBuffer.pop32();
                                this._helperBuffer.destructor();
                                this._helperBuffer = new ByteArray(size + 1);
                                this._dState = BODY;
                            }
                            break;
                        case BODY:
                            i = this._helperBuffer.fill(raw, raw.length, i);
                            
                            if (this._helperBuffer.filled()) {
                                var ev = factory(this._helperBuffer);
                                this._onEvent(ev);
                                this._helperBuffer.destructor();
                                this._helperBuffer = new ByteArray(4);
                                this._dState = SIZE;
                            }
                            break;
                    }
                }
            },
            "_setId": function(id) {
                if (this._state === CONNECTING) {
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
                vc.insert("yourName", this._remoteName.clone());
                
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
    
    var SIZE = 11;
    var BODY = 10;
})();
