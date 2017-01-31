"use strict";

var WebSocketServer = require("ws").Server;
var Socket = require("./socket");
var Subscribable = require("../utils/subscribable");
var AbstractMap = require("../wContainer/abstractmap");
var String = require("../wType/string");
var Uint64 = require("../wType/uint64");

var Server = Subscribable.inherit({
    "className": "Server",
    "constructor": function(name) {
        Subscribable.fn.constructor.call(this);
        
        if (!name) {
            throw new Error("Can't construct a socket without a name");
        }
        
        this._lastId = new Uint64(0);
        this._name = name instanceof String ? name : new String(name);
        this._server;
        
        this._connections = new Server.ConnectionsMap(false);
        this._peers = new Server.PeersMap(false);
        this._listening = false;
        
        this._initProxy();
    },
    "destructor": function() {
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
            this._peers.clear();
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
    "_initProxy": function() {
        this._proxy = {
            onConnection: this._onConnection.bind(this),
            onReady: this._onReady.bind(this)
        };
    },
    "_onConnection": function(socket) {
        this._lastId["++"]();
        var wSocket = new Socket(this._name, socket, this._lastId);
        this._connections.insert(this._lastId.clone(), wSocket);
        
        wSocket.one("connected", Server.onSocketConnected, {srv: this, soc: wSocket});
        wSocket.one("disconnected", Server.onSocketDisconnected, {srv: this, soc: wSocket});
    },
    "_onReady": function() {
        this.trigger("ready");
    }
});
    Server.onSocketDisconnected = function() {
        var cItr = this.srv._connections.find(this.soc.getId());
        if (!cItr["=="](this.srv._connections.end())) {
            var pair = cItr["*"]();
            this.srv._connections.erase(cItr);
            pair.first.destructor();
        }
        var nItr = this.srv._peers.find(this.soc.getRemoteName());
        if (!nItr["=="](this.srv._peers.end())) {
            var nPair = nItr["*"]();
            var pIt = nPair.second.find(this.soc.getId());
            if (!pIt["=="](nPair.second.end())) {
                var pPair = pIt["*"]();
                nPair.second.erase(pIt);
                pPair.first.destructor();
            }
            if (nPair.second.size() === 0) {
                this.srv._peers.erase(nItr);
                nPair.destructor();
            }
        }
        setTimeout(Server.removeLater.bind(this), 1);
    };
    Server.onSocketConnected = function() {
        var name = this.soc.getRemoteName();
        
        var itr = this.srv._peers.find(name);
        var peer;
        if (itr["=="](this.srv._peers.end())) {
            peer = new Server.ConnectionsMap(false);
            this.srv._peers.insert(name.clone(), peer);
        } else {
            peer = itr["*"]().second;
        }
        peer.insert(this.soc.getId().clone(), this.soc);
        this.srv.trigger("newConnection", this.soc);
    };
    Server.ConnectionsMap = AbstractMap.template(Uint64, Socket);
    Server.PeersMap = AbstractMap.template(String, Server.ConnectionsMap);
    Server.removeLater = function() {
        this.soc.destructor();
    }

module.exports = Server;
