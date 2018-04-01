"use strict";

var WebSocket = require("ws");
var Subscribable = require("../utils/subscribable");
var Event = require("../wType/event");
var ByteArray = require("../wType/bytearray");
var String = require("../wType/string");
var Vocabulary = require("../wType/vocabulary");
var Uint64 = require("../wType/uint64");
var Address = require("../wType/address");
var factory = require("../wType/factory");

var Socket = Subscribable.inherit({
    "className": "Socket",
    "constructor": function(name, socket, id) {
        if (!name) {
            throw new Error("Can't construct a socket without a name");
        }
        Subscribable.fn.constructor.call(this);
        
        this._state = DISCONNECTED;
        this._dState = SIZE;
        this._name = name instanceof String ? name : new String(name);
        this._remoteName = new String();
        this._id = new Uint64(0);
        this._serverCreated = false;
        this._helperBuffer = new ByteArray(4);
        
        this._initProxy();
        if (socket) {
            this._serverCreated = true;
            this._socket = socket;
            this._id.destructor();
            this._id = id.clone();
            
            this._socket.on("close", this._proxy.onClose);
            this._socket.on("error", this._proxy.onError);
            this._socket.on("message", this._proxy.onMessage);
        }
    },
    "destructor": function() {
        this.close();
        if (this._state === DISCONNECTING) {
            var onclose = function() {
                Subscribable.fn.destructor.call(this);
            }
            this.on("disconnected", onclose.bind(this));
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
    "getId": function() {
        return this._id;
    },
    "getRemoteName": function() {
        return this._remoteName;
    },
    "_initProxy": function() {
        this._proxy = {
            onClose: this._onClose.bind(this),
            onError: this._onError.bind(this),
            onMessage: this._onMessage.bind(this)
        };
    },
    "isOpened": function() {
        return this._state !== undefined && this._state === CONNECTED;
    },
    "_onClose": function(ev) {
        this._state = DISCONNECTED;
        this.trigger("disconnected", ev, this);
    },
    "_onError": function(err) {
        this.trigger("error", err);
    },
    "_onEvent": function(ev) {
        if (ev.isSystem()) {
            var cmd = ev._data.at("command").toString();
            
            switch(cmd) {
                case "setId":
                    if (this._serverCreated) {
                        if (this._state === CONNECTING) {
                            this.trigger("negotiationId", ev._data.at("id"));
                        } else {
                            throw new Error("An attempt to set id in unexpected time");
                        }
                    } else {
                        this._setId(ev._data.at("id"));
                        this._setRemoteName();
                    }
                    break;
                case "setName":
                    this._setName(ev._data.at("name"));
                    if (!ev._data.at("yourName")["=="](this._name)) {
                        this._setRemoteName();
                    }
                    this._state = CONNECTED;
                    this.trigger("connected");
                    break;
                default:
                    throw new Error("Unknown system command: " + cmd);
            }
        } else {
            this.trigger("message", ev);
        }
        ev.destructor();
    },
    "_onMessage": function(msg) {
        var raw = new Uint8Array(msg);
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
    "open": function(addr, port) {
        if (this._state === DISCONNECTED) {
            this._state = CONNECTING;
            this._remoteName.destructor();
            this._remoteName = new String();
            this._socket = new WebSocket("ws://"+ addr + ":" + port);
            
            this._socket.on("close", this._proxy.onClose);
            this._socket.on("error", this._proxy.onError);
            this._socket.on("message", this._proxy.onMessage);
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
    },
    "_setRemoteId": function() {
        if (this._state === DISCONNECTED) {
            this._state = CONNECTING;
        }
        var vc = new Vocabulary();
        vc.insert("command", new String("setId"));
        vc.insert("id", this._id.clone());
        
        var ev = new Event(new Address(), vc, true);
        ev.setSenderId(this._id.clone());
        this.send(ev);
        
        ev.destructor();
    }
});

var DISCONNECTED = 111;
var DISCONNECTING = 110;
var CONNECTING = 101;
var CONNECTED = 100;

var SIZE = 1
var BODY = 10;

module.exports = Socket;
