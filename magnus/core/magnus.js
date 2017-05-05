"use strict";
var Subscribable = require("../lib/utils/subscribable");
var Socket = require("../lib/wSocket/socket");
var Server = require("../lib/wSocket/server");
var Address = require("../lib/wType/address");
var String = require("../lib/wType/string");
var Vocabulary = require("../lib/wType/vocabulary");
var Dispatcher = require("../lib/wDispatcher/dispatcher");
var Logger = require("../lib/wDispatcher/logger");
var log = require("../lib/log")(module);

var Commands = require("./commands");
var Connector = require("./connector");

var GlobalControls = require("../lib/wModel/globalControls");
var PageStorage = require("../lib/wModel/pageStorage");
var ModelString = require("../lib/wModel/string");
var Attributes = require("../lib/wModel/attributes");

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
        this._initConnector();
        this._initPages();
        
        var port = this._cfg.get("webSocketServerPort");
        this.server.listen(port);
        
        global.magnus = this;
    },
    "connectCorax": function() {
        this.coraxSocket.open("localhost", 8080);
    },
    "_initConnector": function() {
        this._connector = new Connector("Magnus", this.dispatcher, this.server, this._commands);
        
        this._connector.on("serviceMessage", this._onModelServiceMessage, this);
        this._connector.on("connectionsCountChange", this._onConnectionsCountChange, this);
        this._connector.on("connectedNode", this._onConnectedNode, this);
        this._connector.on("disconnectedNode", this._onDisconnectedNode, this);
        
        this._connector.addNode("Corax");
        this._connector.addNode("Perturabo");
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
        this._commands = new Commands(new Address(["management"]));
        
        var version = new ModelString(new Address(["version"]), this._cfg.get("version"));
        version.addProperty("backgroundColor","secondaryColor");
        version.addProperty("color", "secondaryFontColor");
        version.addProperty("fontFamily", "smallFont");
        version.addProperty("fontSize", "smallFontSize");
        
        this._attributes = new Attributes(new Address(["attributes"]));
        this._gc = new GlobalControls(new Address(["magnus", "gc"]));
        this._ps = new PageStorage(new Address(["magnus", "ps"]))
        
        this._commands.on("serviceMessage", this._onModelServiceMessage, this);
        this._gc.on("serviceMessage", this._onModelServiceMessage, this);
        this._ps.on("serviceMessage", this._onModelServiceMessage, this);
        this._attributes.on("serviceMessage", this._onModelServiceMessage, this);
        
        this._attributes.addAttribute("name", new ModelString(new Address(["attributes", "name"]), "Magnus"));
        this._attributes.addAttribute("connectionsAmount", new ModelString(new Address(["connectionsAmount"]), "0"));
        this._attributes.addAttribute("version", version);
        this._gc.addModelAsLink("version", version);
        
        this._commands.register(this.dispatcher, this.server);
        this._gc.register(this.dispatcher, this.server);
        this._ps.register(this.dispatcher, this.server);
        this._attributes.register(this.dispatcher, this.server);
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
    },
    "_initServer": function() {
        this.server = new Server("Magnus");
        this.server.on("ready", this._onServerReady, this)
    },
    "_onConnectedNode": function(nodeName) {
        switch (nodeName) {
            case "Perturabo":
                break;
        }
    },
    "_onConnectionsCountChange": function(count) {
        this._attributes.setAttribute("connectionsAmount", count);
    },
    "_onDisconnectedNode": function(nodeName) {
        switch (nodeName) {
            case "Perturabo":
                break;
        }
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
    "_onServerReady": function() {
        log.info("Magnus is listening on port " + this._cfg.get("webSocketServerPort"));
        log.info("Magnus is ready");
    }
});

module.exports = Magnus;
