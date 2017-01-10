"use strict";

var Subscribable = require("../lib/utils/subscribable");
var Handler = require("../lib/wDispatcher/handler");
var String = require("../lib/wType/string");
var Address = require("../lib/wType/address");
var Vocabulary = require("../lib/wType/vocabulary");
var Socket = require("../lib/wSocket/socket");

var Connector = Subscribable.inherit({
    "className": "Connector",
    "constructor": function(pnk, dp, srv, cmds) {
        Subscribable.fn.constructor.call(this);
        
        this._parentNodeKey = new String(pnk);
        this._dispatcher = dp;
        this._server = srv;
        this._commands = cmds;
        this._occ = 0;
        
        this._nodes = global.Object.create(null);
        this._pending = [];
        
        this._server.on("newConnection", this._onNewConnection, this);
    },
    "destructor": function() {
        this._server.off("newConnection", this._onNewConnection, this);
        this._parentNodeKey.destructor();
        
        for (var key in this._nodes) {
            var node = this._nodes[key];
            if (node.outgoing && (node.pending || node.connected)) {
                node.socket.destructor();
            }
            node.destructor();
            delete this._nodes[key];
        }
        
        Subscribable.fn.destructor.call(this);
    },
    "addNode": function(key) {
        var old = this._nodes[key];
        if (old) {
            throw new Error("An attempt to add already existing node in connector");
        }
        
        var node = new Node();
        var cn = new Address(["connect" + key]);
        var dn = new Address(["disconnect" + key]);
        var commandsAddr = this._commands.getAddress();
        var connect = new Handler(commandsAddr["+"](cn), node, node._h_connect);
        var disconnect = new Handler(commandsAddr["+"](dn), node, node._h_disconnect);
        node.on("connect", this.connectTo, this);
        node.on("disconnect", this.disonnectNode, this);
        this._commands.addCommand("connect" + key, connect, new Vocabulary());
        this._commands.addCommand("disconnect" + key, disconnect, new Vocabulary());
        this._commands.enableCommand("connect" + key, true);
        this._nodes[key] = node;
    },
    "connectTo": function(name, address, port) {
        var node = this._nodes[name];
        if (!node) {
            throw new Error("An attempt to access non existing node in connector");
        }
        if (!node.connected) {
            var socket = new Socket(this._parentNodeKey.clone());
            socket.on("connected", this._onOutgoingSocketConnected.bind(this, node));
            socket.on("error", this._onOutgoingSocketError.bind(this, node));
            node.outgoing = true;
            node.connected = false;
            node.pending = true;
            node.socket = socket;
            this._pending.push[node];
            this._commands.enableCommand("connect" + name, false);
            this._commands.enableCommand("disconnect" + name, false);
            socket.open(address, port);
        } else {
            //TODO;
        }
    },
    "disconnectNode": function(name) {
        var node = this._nodes[name]
        if (!node) {
            throw new Error("An attempt to access non existing node in connector");
        }
        
        if (node.connected) {
            this._commands.enableCommand("connect" + name, false);
            this._commands.enableCommand("disconnect" + name, false);
            node.socket.close();
        } else {
            //TODO;
        }
    },
    "getConnectionsCount": function() {
        return this._server.getConnectionsCount() + this._occ;
    },
    "_onNewConnection": function(socket) {
        this.trigger("serviceMessage", "New connection, id: " + socket.getId().toString(), 0);
        socket.on("message", this._dispatcher.pass, this._dispatcher);
        socket.on("disconnected", this._onSocketDisconnected.bind(this, socket));
        this.trigger("connectionsCountChange", this.getConnectionsCount());
        
        var node = this._nodes[socket.getRemoteName().toString()];
        if (node) {
            if (!node.connected) {
                node.connected = true;
                this._commands.enableCommand("disconnect" + node.name.toString(), true);
            } else {
                //TODO;
            }
        }
    },
    "_onOutgoingSocketConnected": function(node) {
        var index = this._pending.indexOf(node);
        if (index !== -1) {
            node.pending = false;
            this._pending.splice(index, 1);
        } else {
            //TODO;
        }
        ++this._occ;
        this._onNewConnection(node.socket);
    },
    "_onOutgoingSocketError": function() {
        var index = this._pending.indexOf(node);
        if (index !== -1) {
            node.pending = false;
            this._pending.splice(index, 1);
        } else {
            //TODO;
        }
        node.socket.destructor();
        node.socket = undefined;
        this._commands.enableCommand("connect" + node.name.toString(), true);
    },
    "_onSocketDisconnected": function(socket) {
        this.trigger("serviceMessage", "Connection closed, id: " + socket.getId().toString(), 0);
        
        var key = socket.getRemoteName().toString();
        var node = this._nodes[key];
        if (node) {
            if (node.connected) {
                if (node.outgoing) {
                    socket.destructor();
                }
                node.connected = false;
                node.pending = false;
                node.outgoing = false;
                node.socket = undefined;
                
                this._commands.enableCommand("connect" + node.name.toString(), true);
                --this._occ;
            } else {
                throw new Error("Something went wrong in connector");;
            }
        } else {
            
        }
        
        this.trigger("connectionsCountChange", this.getConnectionsCount());
    },
    "sendTo": function(key, event) {
        var node = this._nodes[key];
        if (!node) {
            throw new Error("An attempt to access non existing node in connector");
        }
        if (node.connected) {
            node.socket.send(event);
        } else {
            //TODO
        }
    }
});

var Node = Subscribable.inherit({
    "className": "ConnectorNode",
    "constructor": function(name) {
        Subscribable.fn.constructor.call(this);
        
        this.name = new String(name);
        this.socket = undefined;
        this.connected = false;
        this.outgoing = false;
        this.pending = false;
        
    },
    "destructor": function() {
        this.name.destructor();
        
        Subscribable.fn.destructor.call(this);
    },
    "_h_connect": function(ev) {
        var vc = ev.getData();
        
        this.trigger("connect", vc.at("address"), vc.at("port"));
    },
    "_h_disconnect": function(ev) {
        this.trigger("disconnect", this.name);
    }
});

module.exports = Connector;
