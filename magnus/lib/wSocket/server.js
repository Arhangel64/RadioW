"use strict";

var WebSocketServer = require("ws").Server;
var Socket = require("./socket");
var Subscribable = require("../utils/subscribable");
var AbstractMap = require("../wContainer/abstractmap");
var AbstractSet = require("../wContainer/abstractset");
var String = require("../wType/string");
var Uint64 = require("../wType/uint64");

var Server = Subscribable.inherit({
    "className": "Server",
    "constructor": function(name) { 
        if (!name) {
            throw new Error("Can't construct a socket without a name");
        }
        Subscribable.fn.constructor.call(this);
        
        this._lastId = new Uint64(0);
        this._pool = new Server.Uint64Set(true);
        this._name = name instanceof String ? name : new String(name);
        this._server = undefined;
        this._connections = new Server.ConnectionsMap(true);
        this._listening = false;
        
        this._initProxy();
    },
    "destructor": function() {
        if (this._listening) {
            this._server.stop();
            delete this._server;
        }
        this._lastId.destructor();
        this._pool.destructor();
        this._name.destructor();
        this._connections.destructor();
        
        Subscribable.fn.destructor.call(this);
    },
    "listen": function(port) {
        if (!this._listening) {
            this._listening = true;
            this._server = new WebSocketServer({port: port}, this._proxy.onReady);
            this._server.on("connection", this._proxy.onConnection);
        }
    },
    "stop": function() {
        if (this._listening) {
            this._listening = false;
            this._server.stop();
            this._lastId = new Uint64(0);
            this._connections.clear();
            this._pool.clear();
            delete this._server;
        }
    },
    "getConnection": function(id) {
        var itr = this._connections.find(id);
        if (itr["=="](this._connections.end())) {
            throw new Error("Connection not found");
        }
        return itr["*"]().second;
    },
    "getConnectionsCount": function() {
        return this._connections.size();
    },
    "openConnection": function(addr, port) {
        var webSocket = new Subscribable();
        var wSocket = this._createSocket(webSocket);
        wSocket._socket.destructor();
        wSocket.open(addr, port);
    },
    "_createSocket": function(socket) {
        var connectionId;
        if (this._pool.size() === 0) {
            this._lastId["++"]()
            connectionId = this._lastId.clone();
        } else {
            var itr = this._pool.begin();
            connectionId = itr["*"]().clone();
            this._pool.erase(itr);
        }
        var wSocket = new Socket(this._name, socket, connectionId);
        this._connections.insert(connectionId, wSocket);
        
        wSocket.on("connected", this._onSocketConnected.bind(this, wSocket));
        wSocket.on("disconnected", this._onSocketDisconnected.bind(this, wSocket));
        wSocket.on("negotiationId", this._onSocketNegotiationId.bind(this, wSocket));
        
        return wSocket;
    },
    "_initProxy": function() {
        this._proxy = {
            onConnection: this._onConnection.bind(this),
            onReady: this._onReady.bind(this)
        };
    },
    "_onConnection": function(socket) {
        var wSocket = this._createSocket(socket);
        wSocket._setRemoteId();
    },
    "_onReady": function() {
        this.trigger("ready");
    },
    "_onSocketConnected": function(socket) {
        this.trigger("newConnection", socket);
    },
    "_onSocketDisconnected": function(socket) {
        var cItr = this._connections.find(socket.getId());
        setTimeout(this._connections.erase.bind(this._connections, cItr), 1);
    },
    "_onSocketNegotiationId": function(socket, id) {
        var oldId = socket.getId();
        if (id["=="](oldId)) {
            socket._setRemoteName();
        } else {
            var pItr = this._pool.lowerBound(id);
            var newId;
            if (pItr["=="](this._pool.end())) {
                this._lastId["++"]();
                newId = this._lastId.clone();
            } else {
                newId = pItr["*"]().clone();
                this._pool.erase(pItr);
            }
            var itr = this._connections.find(oldId);
            this._connections.erase(itr);
            this._pool.insert(oldId);
            socket._id = newId;
            this._connections.insert(newId.clone(), wSocket);
            socket._setRemoteId();
        }
    }
});

Server.ConnectionsMap = AbstractMap.template(Uint64, Socket);
Server.Uint64Set = AbstractSet.template(Uint64);

module.exports = Server;
