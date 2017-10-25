"use strict";

var Subscribable = require("../lib/utils/subscribable");
var Handler = require("../lib/wDispatcher/handler");
var String = require("../lib/wType/string");
var Address = require("../lib/wType/address");
var Uint64 = require("../lib/wType/uint64");
var Object = require("../lib/wType/object");
var Vocabulary = require("../lib/wType/vocabulary");
var Socket = require("../lib/wSocket/socket");

var Connector = Subscribable.inherit({
    "className": "Connector",
    "constructor": function(dp, srv, cmds) {
        Subscribable.fn.constructor.call(this);
        
        this._dispatcher = dp;
        this._server = srv;
        this._commands = cmds;
        this._nodes = global.Object.create(null);
        this._ignoredNodes = global.Object.create(null);
        
        this._server.on("newConnection", this._onNewConnection, this);
        this._server.on("closedConnection", this._onClosedConnection, this);
        
        var cn = new Address(["connect"]);
        var ch = new Handler(this._commands.getAddress()["+"](cn), this, this._h_connect);
        var vc = new Vocabulary();
        vc.insert("address", new Uint64(Object.objectType.String));
        vc.insert("port", new Uint64(Object.objectType.Uint64));
        this._commands.addCommand("connect", ch, vc);
        this._commands.enableCommand("connect", true);
        cn.destructor();
    },
    "destructor": function() {
        this._server.off("newConnection", this._onNewConnection, this);
        this._server.off("closedConnection", this._onClosedConnection, this);
        
        this._commands.removeCommand("connect");
        
        for (var key in this._nodes) {
            this._commands.removeCommand("disconnect" + key);
        }
        
        Subscribable.fn.destructor.call(this);
    },
    "addIgnoredNode": function(name) {
        this._ignoredNodes[name] = true;
    },
    "sendTo": function(key, event) {
        var id = this._nodes[key];
        if (!id) {
            throw new Error("An attempt to access non existing node in connector");
        }
        this._server.getConnection(id).send(event);
    },
    "_onNewConnection": function(socket) {
        var name = socket.getRemoteName().toString();
        
        if (this._ignoredNodes[name] === undefined) {
            if (this._nodes[name] === undefined) {
                if (this._server.getName().toString() === name) {
                    this.trigger("serviceMessage", "An attempt to connect node to itself, closing connection", 1);
                    setTimeout(this._server.closeConnection.bind(this._server, socket.getId()));
                } else {
                    var dc = "disconnect";
                    var dn = dc + name;
                    var dh = new Handler(this._commands.getAddress()["+"](new Address([dc, name])), this, this._h_disconnect);
                    this._commands.addCommand(dn, dh, new Vocabulary());
                    this._commands.enableCommand(dn, true);
                    
                    this._nodes[name] = socket.getId();
                    
                    this.trigger("serviceMessage", "New connection, id: " + socket.getId().toString(), 0);
                    socket.on("message", this._dispatcher.pass, this._dispatcher);
                    this.trigger("nodeConnected", name);
                }
            } else {
                this.trigger("serviceMessage", "Node " + name + " tried to connect, but connection with that node is already open, closing new connection", 1);
                setTimeout(this._server.closeConnection.bind(this._server, socket.getId()));
            }
        } else {
            this.trigger("serviceMessage", "New connection, id: " + socket.getId().toString(), 0);
            socket.on("message", this._dispatcher.pass, this._dispatcher);
        }
    },
    "_onClosedConnection": function(socket) {
        this.trigger("serviceMessage", "Connection closed, id: " + socket.getId().toString());
        
        var name = socket.getRemoteName().toString();
        if (this._ignoredNodes[name] === undefined) {
            if (this._nodes[name]) {
                this._commands.removeCommand("disconnect" + name);
                delete this._nodes[name];
                this.trigger("nodeDisconnected", name);
            }
        }
    },
    "getNodeSocket": function(key) {
        var id = this._nodes[key];
        if (!id) {
            throw new Error("An attempt to access non existing node in connector");
        }
        return this._server.getConnection(id);
    },
    "_h_connect": function(ev) {
        var vc = ev.getData();
        this._server.openConnection(vc.at("address"), vc.at("port"));
    },
    "_h_disconnect": function(ev) {
        var addr = ev.getDestination();
        var id = this._nodes[addr.back().toString()];
        if (id) {
            this._server.closeConnection(id);
        }
        
    }
});

module.exports = Connector;
