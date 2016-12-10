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

var GlobalControls = require("../lib/wModel/globalControls");
var PageStorage = require("../lib/wModel/pageStorage");
var ModelString = require("../lib/wModel/string");

var HomePage = require("../pages/home");
var MusicPage = require("../pages/music");
var TestPage = require("../pages/test");

var Magnus = Subscribable.inherit({
    "className": "Magnus",
    "constructor": function(config) {
        Subscribable.fn.constructor.call(this);
        
        this._cfg = config;
        
        this._initDispatcher();
        this._initServer();
        this._initModels();
        this._initPages();
        
        var port = this._cfg.get("webSocketServerPort");
        this.server.listen(port);
        
        global.magnus = this;
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
        this._gc = new GlobalControls(new Address(["magnus", "gc"]), {version: this._cfg.get("version")});
        this._ps = new PageStorage(new Address(["magnus", "ps"]))
        
        this._name = new ModelString(new Address(["name"]), "Magnus");
        this._connectionsAmount = new ModelString(new Address(["connectionsAmount"]), "0");
        
        this._gc.on("serviceMessage", this._onModelServiceMessage, this);
        this._ps.on("serviceMessage", this._onModelServiceMessage, this);
        this._name.on("serviceMessage", this._onModelServiceMessage, this);
        this._connectionsAmount.on("serviceMessage", this._onModelServiceMessage, this);
        
        this._gc.register(this.dispatcher, this.server);
        this._ps.register(this.dispatcher, this.server);
        this._name.register(this.dispatcher, this.server);
        this._connectionsAmount.register(this.dispatcher, this.server);
    },
    "_initPages": function() {
        var root = new HomePage(new Address(["pages", "/"]));
        this._ps.addPage(root, ["/", "/index.html"]);
        this._gc.addNav("Home", root.getAddress());
        
        var music = new MusicPage(new Address(["pages", "/music"]));
        this._ps.addPage(music, ["/music", "/music/", "/music.html"]);
        this._gc.addNav("Music", music.getAddress());
        
        var test = new TestPage(new Address(["pages", "/test"]));
        this._ps.addPage(test, ["/test", "/test/", "/test.html"]);
        this._gc.addNav("Testing...", test.getAddress());
        
        music.addBand("Avenged sevenfold");
        music.addBand("Megadeth");
        music.addBand("Iron Maiden");
        music.addBand("As I Lay Dying");
    },
    "_initServer": function() {
        this.server = new Server("Magnus");
        this.server.on("newConnection", this._onNewConnection, this);
        this.server.on("ready", this._onServerReady, this)
    },
    "_onCoraxConnected": function() {
//         var address = new Address(["corax", "test"]);
//         var vc = new Vocabulary();
//         vc.insert("msg", new String("Hello, I'm Magnus"));
//         vc.insert("source", new Address(["magnus", "test"]));
//         
//         var ev = new Event(address, vc);
//         ev.setSenderId(this.coraxSocket.getId().clone());
//         
//         this.coraxSocket.send(ev);
//         ev.destructor();
    },
    "_onModelServiceMessage": function(msg, severity) {
        var fn;
        
        switch (severity) {
            case 2:
                fn = log.error;
                break;
            case 1:
                fn = log.warn;
                break;
            case 0:
            default:
                fn = log.info;
                break;
        }
        
        fn(msg);
    },
    "_onNewConnection": function(socket) {
        socket.on("message", this.dispatcher.pass, this.dispatcher);
        socket.one("disconnected", Magnus.onSocketDisconnected.bind(this, socket));
        
        log.info("New connection, id: " + socket.getId().toString());
        
        this._connectionsAmount.set(this.server.getConnectionsCount().toString());
    },
    "_onServerReady": function() {
        log.info("Magnus is listening on port " + this._cfg.get("webSocketServerPort"));
        log.info("Magnus is ready");
    }
});

Magnus.onSocketDisconnected = function(socket) {
    log.info("Connection closed, id: " + socket.getId().toString());
    
    this._connectionsAmount.set(this.server.getConnectionsCount().toString());
}

module.exports = Magnus;
