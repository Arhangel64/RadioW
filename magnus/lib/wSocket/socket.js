"use strict";

var WebSocket = require("ws");
var Subscribable = require("../utils/subscribable");
var Event = require("../wType/event");
var ByteArray = require("../wType/bytearray");
var String = require("../wType/string");
var Vocabulary = require("../wType/vocabulary");
var Uint64 = require("../wType/uint64");
var Address = require("../wType/address");

var Socket = Subscribable.inherit({
    "className": "Socket",
    "constructor": function(name, socket, id) {
        Subscribable.fn.constructor.call(this);
        
        this._state = DISCONNECTED;
        
        if (!name) {
            throw new Error("Can't construct a socket without a name");
        }
        
        this._initProxy();
        
        this._name = name instanceof String ? name : new String(name);
        this._remoteName = new String();
        this._id = new Uint64(0);
        this._serverCreated = false;
        
        this.on("connected", this.onConnected);
        this.on("disconnected", this.onDisconnected);
        
        if (socket) {
            this._serverCreated = true;
            this._state = CONNECTING;
            this._socket = socket;
            this._id.destructor();
            this._id = id.clone();
            
            this._socket.on("close", this._proxy.onClose);
            this._socket.on("error", this._proxy.onError);
            this._socket.on("message", this._proxy.onMessage);
            
            this._setRemoteId();
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
            onOpen: this._onOpen.bind(this),
            onClose: this._onClose.bind(this),
            onError: this._onError.bind(this),
            onMessage: this._onMessage.bind(this)
        };
    },
    "isOpened": function() {
        return this._state !== undefined && this._state === CONNECTED;
    },
    "_onClose": function(ev) {
        this.trigger("disconnected", ev, this);
    },
    "_onError": function(err) {
        this.trigger("error", err);
    },
    "_onMessage": function(msg) {
        var raw = new ByteArray(new Uint8Array(msg));
        var ev = raw[">>"]();
                
        if (!(ev instanceof Event)) {
            throw new Error("Wrong format of websocket data");
        }
        
        if (ev.isSystem()) {
            var cmd = ev._data.at("command").toString();
            
            switch(cmd) {
                case "setId":
                    this._setId(ev._data.at("id"));
                    this._setRemoteName();
                    break;
                case "setName":
                    this._setName(ev._data.at("name"));
                    if (this._serverCreated) {
                        this._setRemoteName();
                    }
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
    "_onOpen": function() {
        this.trigger("__cnt__");
    },
    "open": function(addr, port) {
        var that = this;
        
        if (this._state === DISCONNECTED) {
            this._state = CONNECTING;
            this._socket = new WebSocket("ws://"+ addr + ":" + port);
            //this._socket.binaryType = "arraybuffer";
            
            this._socket.on("open", this._proxy.onOpen);
            this._socket.on("close", this._proxy.onClose);
            this._socket.on("error", this._proxy.onError);
            this._socket.on("message", this._proxy.onMessage);
        }
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
        this._state = DISCONNECTED;
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
    },
    "_setRemoteId": function() {
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

module.exports = Socket;
