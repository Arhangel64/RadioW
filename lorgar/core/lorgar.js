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
    defineArray.push("models/pageStorage");
    
    defineArray.push("views/view");
    defineArray.push("views/layout");
    defineArray.push("views/gridLayout");
    defineArray.push("views/page");
    
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
        var PageStorage = require("models/pageStorage");
        
        var View = require("views/view");
        var Layout = require("views/layout");
        var GridLayout = require("views/gridLayout");
        var Page = require("views/page");
        
        var Lorgar = Class.inherit({
            "className": "Lorgar",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                this._currentPageModel = undefined;
                
                this._initDispatcher();
                this._initMagnusSocket();
                this._initCoraxSocket();
                this._initViews();
                this._initModels();
                
                this.connectMagnus();
                //this.connectCorax();
            },
            "destructor": function() {
                if (this._currentPageModel) {
                    this._currentPageModel.destructor();
                }
                
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
                this._gc = new GlobalControls(new Address(["magnus", "gc"]), this._mainLayout);
                this._ps = new PageStorage(new Address(["magnus", "ps"]));
                
                this._gc.register(this.dispatcher, this.magnusSocket);
                this._ps.register(this.dispatcher, this.magnusSocket);
            },
            "_initPageModel": function(addr) {
                console.log(addr.toString());
            },
            "_initViews": function() {
                this._body = new Layout();
                this._currentPage = new Page();
                this._mainLayout = new GridLayout();
                
                document.body.innerHTML = "";
                document.body.appendChild(this._body._e);
                window.addEventListener("resize",this._onWindowResize.bind(this) ,false);
                
                this._body.setSize(document.body.offsetWidth, document.body.offsetHeight);
                this._body.append(this._mainLayout);
                this._mainLayout.append(this._currentPage, 1, 0, 1, 1);
            },
            "_magnusSocketConnected": function() {
                this._gc.subscribe();
                
                if (!this._currentPageModel) {
                    this._ps.getPageAddress(location.pathname);
                    this._ps.one("pageAddress", this._initPageModel, this);
                }
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