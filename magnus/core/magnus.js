"use strict";
var Subscribable = require("../lib/utils/subscribable");
var Socket = require("../lib/wSocket/socket");
var Server = require("../lib/wSocket/server");
var Address = require("../lib/wType/address");
var String = require("../lib/wType/string");
var Event = require("../lib/wType/event");
var Vocabulary = require("../lib/wType/vocabulary");
var Dispatcher = require("../lib/wDispatcher/dispatcher");
var Handler = require("../lib/wDispatcher/handler");
var Logger = require("../lib/wDispatcher/logger");
var log = require("../lib/log")(module);

var GlobalControls = require("../models/globalControls");
var PageStorage = require("../models/pageStorage");

var Magnus = Subscribable.inherit({
    "className": "Magnus",
    "constructor": function(config) {
        Subscribable.fn.constructor.call(this);
        
        this._cfg = config;
        
        this._initDispatcher();
        this._initServer();
        this._initModels();
        
        var port = this._cfg.get("webSocketServerPort");
        this.server.listen(port);
        global.magnus = this;
        log.info("Magnus is listening on port " + port);
    },
    "connectCorax": function() {
        this.coraxSocket.open("localhost", 8080);
    },
    "_initCoraxSocket": function() {
        this.coraxSocket = new Socket("Magnus");
        this.coraxSocket.on("connected", this._onCoraxConnected, this);
        this.coraxSocket.on("message", this.dispatcher.pass, this.dispatcher);
    },
    "_initDispatcher": function() {
        this.dispatcher = new Dispatcher();
        this._logger = new Logger();
        this.dispatcher.registerDefaultHandler(this._logger);
    },
    "_initModels": function() {
        this._gc = new GlobalControls(new Address(["magnus", "gc"]));
        this._ps = new PageStorage(new Address(["magnus", "ps"]))
        
        this._gc.register(this.dispatcher);
        this._ps.register(this.dispatcher);
    },
    "_initServer": function() {
        this.server = new Server("Magnus");
        this.server.on("newConnection", this._onNewConnection, this);
    },
    "_onCoraxConnected": function() {
        var address = new Address(["corax", "test"]);
        var vc = new Vocabulary();
        vc.insert("msg", new String("Hello, I'm Magnus"));
        vc.insert("source", new Address(["magnus", "test"]));
        
        var ev = new Event(address, vc);
        ev.setSenderId(this.coraxSocket.getId().clone());
        
        this.coraxSocket.send(ev);
        ev.destructor();
    },
    "_onNewConnection": function(socket) {
        socket.on("message", this.dispatcher.pass, this.dispatcher);
        socket.one("disconnected", Magnus.onSocketDisconnected, {mgn: this, soc: socket});
        
        log.info("New connection, id: " + socket.getId().toString());
    }
});

Magnus.onSocketDisconnected = function() {
    log.info("Connection closed, id: " + this.soc.getId().toString());
}

module.exports = Magnus;
