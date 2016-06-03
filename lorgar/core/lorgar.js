"use strict";
(function lorgar_js() {
    var moduleName = "core/lorgar";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    defineArray.push("lib/wSocket/socket");
    defineArray.push("lib/wDispatcher/dispatcher");
    defineArray.push("lib/wDispatcher/handler");
    defineArray.push("lib/wDispatcher/logger");
    
    defineArray.push("lib/wType/event");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wType/vocabulary");
    defineArray.push("lib/wType/string");
    
    defineArray.push("models/globalControls");
    
    defineArray.push("views/view");
    defineArray.push("views/layout");
    defineArray.push("views/gridLayout");
    
    define(moduleName, defineArray, function lorgar_module() {
        var Class = require("lib/utils/class");
        var Socket = require("lib/wSocket/socket");
        var Dispatcher = require("lib/wDispatcher/dispatcher");
        var Handler = require("lib/wDispatcher/handler");
        var Logger = require("lib/wDispatcher/logger");
        
        var Event = require("lib/wType/event");
        var Address = require("lib/wType/address");
        var Vocabulary = require("lib/wType/vocabulary");
        var String = require("lib/wType/string");
        
        var GlobalControls = require("models/globalControls");
        
        var View = require("views/view");
        var Layout = require("views/layout");
        var GridLayout = require("views/gridLayout");
        
        var Lorgar = Class.inherit({
            "className": "Lorgar",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                this._initDispatcher();
                this._initMagnusSocket();
                this._initCoraxSocket();
                this._initModels();
                
                this.connectMagnus();
                //this.connectCorax();
            },
            "destructor": function() {
                this._gc.destructor();
                this._body.destructor();
                
                this.coraxSocket.close();
                this.dispatcher.unregisterHandler(this._h_test);
                this.dispatcher.unregisterDefaultHandler(this._logger);
                
                this._h_test.destructor();
                this._logger.destructor();
                this.dispatcher.destructor();
                this.magnusSocket.destructor();
                this.coraxSocket.destructor();
                
                Class.fn.destructor.call(this);
            },
            "connectCorax": function() {
                this.coraxSocket.open("localhost", 8080);
            },
            "connectMagnus": function() {
                this.magnusSocket.open("localhost", 8081);
            },
            "_coraxSocketConnected": function() {
                var address = new Address(["corax", "test"]);
                var vc = new Vocabulary();
                vc.insert("msg", new String("Hello, I'm Lorgar"));
                vc.insert("source", new Address(["lorgar", "test"]));
                
                var ev = new Event(address, vc);
                ev.setSenderId(this.coraxSocket.getId());
                
                this.coraxSocket.send(ev);
            },
            "_coraxSocketDisconnected": function() {
                console.log("corax socket disconnected");
            },
            "_coraxSocketError": function(e) {
                console.log("corax socket error: ");
                console.log(e);
            },
            "_initCoraxSocket": function() {
                this.coraxSocket = new Socket("Lorgar");
                this.coraxSocket.on("connected", this._coraxSocketConnected, this);
                this.coraxSocket.on("disconnected", this._coraxSocketDisconnected, this);
                this.coraxSocket.on("error", this._coraxSocketError, this);
                this.coraxSocket.on("message", this.dispatcher.pass, this.dispatcher);
            },
            "_initDispatcher": function() {
                this.dispatcher = new Dispatcher();
                this._h_test = new Handler(new Address(["lorgar", "test"]), this, this._test);
                this._logger = new Logger();
                this.dispatcher.registerHandler(this._h_test);
                this.dispatcher.registerDefaultHandler(this._logger);
            },
            "_initMagnusSocket": function() {
                this.magnusSocket = new Socket("Lorgar");
                this.magnusSocket.on("connected", this._magnusSocketConnected, this);
                this.magnusSocket.on("disconnected", this._magnusSocketDisconnected, this);
                this.magnusSocket.on("error", this._magnusSocketError, this);
                this.magnusSocket.on("message", this.dispatcher.pass, this.dispatcher);
            },
            "_initModels": function() {
                this._body = new Layout();
                document.body.innerHTML = "";
                document.body.appendChild(this._body._e);
                window.addEventListener("resize",this._onWindowResize.bind(this) ,false);
                this._body.setSize(document.body.offsetWidth, document.body.offsetHeight);
                
                var mainGrid = new GridLayout();
                this._body.append(mainGrid);
                
                var red = new View();
                red._e.style.backgroundColor = "#aa0000";
                
                var blue = new View();
                blue._e.style.backgroundColor = "#0000aa";
                
                var green = new View();
                green._e.style.backgroundColor = "#00aa00";
                
                var yellow = new View();
                yellow._e.style.backgroundColor = "#aaaa00";
                
                var cian = new View();
                cian._e.style.backgroundColor = "#00aaaa";
                
                mainGrid.append(red, 1, 1, 1, 1);
                mainGrid.append(blue, 0, 1, 1, 2);
                mainGrid.append(green, 1, 2, 2, 1);
                mainGrid.append(yellow, 2, 0, 1, 2);
                mainGrid.append(cian, 0, 0, 2, 1);
                
                red.remove();
                blue.remove();
                green.remove();
                
                this._gc = new GlobalControls(new Address(["magnus", "gc"]), this._body);
                this._gc.register(this.dispatcher, this.magnusSocket);
            },
            "_magnusSocketConnected": function() {
                this._gc.subscribe();
            },
            "_magnusSocketDisconnected": function() {
                console.log("magnus socket disconnected");
            },
            "_magnusSocketError": function(e) {
                console.log("magnus socket error: ");
                console.log(e);
            },
            "_onWindowResize": function() {
                this._body.setSize(document.body.offsetWidth, document.body.offsetHeight);
            },
            "_test": function(e) {
                console.info(e.toString());
            }
        });
        
        return Lorgar;
    });
})();