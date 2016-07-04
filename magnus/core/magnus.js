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
var Page = require("../models/page");
var ModelString = require("../models/string");

var MusicPage = require("../pages/music");

var Magnus = Subscribable.inherit({
    "className": "Magnus",
    "constructor": function(config) {
        Subscribable.fn.constructor.call(this);
        global.magnus = this;
        
        this._cfg = config;
        
        this._initDispatcher();
        this._initServer();
        this._initModels();
        this._initPages();
        
        var port = this._cfg.get("webSocketServerPort");
        this.server.listen(port);
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
    "_initPages": function() {
        var root = new Page(new Address(["pages", "/"]));
        var msg = new ModelString(root._address["+"](new Address(["message"])), "This is the root page");
        msg.addProperty("fontFamily", "casualFont");
        root.addItem(msg, 0, 0, 1, 1);
        this._ps.addPage(root, ["/", "/index.html"]);
        this._gc.addNav("Home", root.getAddress());
        
        var music = new MusicPage(new Address(["pages", "/music"]));
        this._ps.addPage(music, ["/music", "/music/", "/music.html"]);
        this._gc.addNav("Music", music.getAddress());
        
        var test = new Page(new Address(["pages", "/test"]));
        msg = new ModelString(test._address["+"](new Address(["message"])), "This is a test page");
        msg.addProperty("fontFamily", "casualFont");
        test.addItem(msg, 0, 0, 1, 1);
        this._ps.addPage(test, ["/test", "/test/", "/test.html"]);
        
        this._gc.addNav("Testing...", test.getAddress());
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
