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
    
    defineArray.push("lib/wController/globalControls");
    defineArray.push("lib/wController/pageStorage");
    defineArray.push("lib/wController/page");
    defineArray.push("lib/wController/localModel");
    
    defineArray.push("views/view");
    defineArray.push("views/layout");
    defineArray.push("views/gridLayout");
    defineArray.push("views/page");
    defineArray.push("views/mainLayout");
    
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
        
        var GlobalControls = require("lib/wController/globalControls");
        var PageStorage = require("lib/wController/pageStorage");
        var PageController = require("lib/wController/page");
        var LocalModel = require("lib/wController/localModel");
        
        var View = require("views/view");
        var Layout = require("views/layout");
        var GridLayout = require("views/gridLayout");
        var Page = require("views/page");
        var MainLayout = require("views/mainLayout");
        
        var Lorgar = Class.inherit({
            "className": "Lorgar",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                this._currentPageCtl = undefined;
                this._nodes = Object.create(null);
                
                this._initDispatcher();
                
                this._prepareNode("Magnus", "localhost", 8081);
                this._prepareNode("Corax", "localhost", 8080);
                
                this._initModels();
                this._initViews();
                
                this.connectNode("Magnus");
                this.connectNode("Corax");
                window.onpopstate = this._onHistoryPopState.bind(this)
            },
            "destructor": function() {
                window.onpopstate = undefined;
                if (this._currentPageCtl) {
                    this._currentPage.destructor();
                    this._currentPageCtl.destructor();
                }
                
                this._gc.destructor();
                this._ps.destructor();
                this._mainColorHelper.destructor();
                this._emptyHelper.destructor();
                
                this._body.destructor();
                
                this.coraxSocket.close();
                this.dispatcher.unregisterDefaultHandler(this._logger);
                
                this._logger.destructor();
                this.dispatcher.destructor();
                //this.magnusSocket.destructor();
                //this.coraxSocket.destructor();
                
                Class.fn.destructor.call(this);
            },
            "changePage": function(addr) {
                if (this._currentPageCtl && this._currentPageCtl.getPairAddress()["=="](addr)) {
                    return;
                }
                this._ps.getPageName(addr);
                this._initPageController(addr.clone());
            },
            "connectNode": function(name) {
                var node = this._nodes[name];
                if (node === undefined) {
                    throw new Error("An attempt to connect not prepared node " + name);
                }
                
                node.socket.open(node.address, node.port);
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
                this._logger = new Logger();
                this.dispatcher.registerDefaultHandler(this._logger);
            },
            "_initModels": function() {
                this._gc = new GlobalControls(new Address(["magnus", "gc"]));
                this._ps = new PageStorage(new Address(["magnus", "ps"]));
                
                this._mainColorHelper = new LocalModel({backgroundColor: "mainColor"});
                this._emptyHelper = new LocalModel();
                
                this._gc.on("themeSelected", this.setTheme, this);
                
                this._gc.register(this.dispatcher, this._nodes.Magnus.socket);
                this._ps.register(this.dispatcher, this._nodes.Magnus.socket);
                
                this._ps.on("pageName", this._onPageName, this);
            },
            "_initPageController": function(addr) {
                if (this._currentPageCtl) {
                    this._currentPage.destructor();
                    this._currentPageCtl.destructor();
                }
                this._currentPageCtl = new PageController(addr);
                this._currentPageCtl.register(this.dispatcher, this._nodes.Magnus.socket);
                this._currentPage = new Page(this._currentPageCtl);
                this._currentPageCtl.subscribe();
                this._mainLayout.append(this._currentPage, 1, 1, 1, 1);
            },
            "_initViews": function() {
                this._body = new Layout(this._emptyHelper);
                this._mainLayout = new MainLayout(this._gc);
                
                document.body.innerHTML = "";
                document.body.appendChild(this._body._e);
                window.addEventListener("resize",this._onWindowResize.bind(this) ,false);
                
                this._body.setSize(document.body.offsetWidth, document.body.offsetHeight);
                this._body.append(this._mainLayout);
                var spacerL = new View(this._mainColorHelper, {
                    maxWidth: 50
                });
                var spacerR = new View(this._mainColorHelper, {
                    maxWidth: 50
                });
                this._mainLayout.append(spacerL, 1, 0, 1, 1);
                this._mainLayout.append(spacerR, 1, 2, 1, 1);
            },
            "_onHistoryPopState": function(e) {
                this._initPageController(new Address(e.state.address));
            },
            "_onPageName": function(name) {
                window.history.pushState({
                    address: this._currentPageCtl.getPairAddress().toArray()
                }, "", name);
            },
            "_onSocketConnected": function(name) {
                console.log(name + " socket connected");
                var node = this._nodes[name];
                node.connected = true;
                
                for (var id in node.foreigns) {
                    if (node.foreigns[id].subscribed) {
                        node.foreigns[id].controller.subscribe();
                    }
                }
                
                if (name === "Magnus") {
                    this._gc.subscribe();
                    
                    if (!this._currentPageCtl) {
                        this._ps.getPageAddress(location.pathname);
                        this._ps.one("pageAddress", this._initPageController, this);
                    }
                }
            },
            "_onSocketDisconnected": function(name) {
                console.log(name + " socket disconnected");
                var node = this._nodes[name];
                node.connected = false;
                
                for (var id in node.foreigns) {
                    if (node.foreigns[id].subscribed) {
                        node.foreigns[id].controller._onSocketDisconnected;
                    }
                }
            },
            "_onSocketError": function(name) {
                console.log(name + " socket error: ");
                console.log(e);
            },
            "_onWindowResize": function() {
                this._body.setSize(document.body.offsetWidth, document.body.offsetHeight);
            },
            "_prepareNode": function(name, address, port) {
                if (this._nodes[name]) {
                    throw new Error("An attempt to prepeare node " + name + " for the second time");
                }
                var obj = Object.create(null);
                obj.name = name;
                obj.address = address;
                obj.port = port;
                obj.socket = new Socket("Lorgar");
                obj.connected = false;
                obj.foreigns = Object.create(null);
                
                obj.socket.on("connected", this._onSocketConnected.bind(this, name));
                obj.socket.on("disconnected", this._onSocketDisconnected.bind(this, name));
                obj.socket.on("error", this._onSocketError.bind(this, name));
                obj.socket.on("message", this.dispatcher.pass, this.dispatcher);
                
                this._nodes[name] = obj;
            },
            "registerForeignController": function(node, controller) {
                var node = this._nodes[node];
                if (node === undefined) {
                    throw new Error("An attempt to register controller to an unknown node " + node);
                }
                
                if (node.foreigns[controller.id] !== undefined) {
                    throw new Error("An attempt to register a controller under node " + node + " for a second time");
                }
                var obj = Object.create(null);
                obj.controller = controller;
                obj.subscribed = false;
                node.foreigns[controller.id] = obj;
                controller.register(this.dispatcher, node.socket);
            },
            "setTheme": function(theme) {
                View.setTheme(theme);
            },
            "subscribeForeignController": function(node, controller) {
                var node = this._nodes[node];
                if (node === undefined) {
                    throw new Error("An attempt to subscribe a controller to an unknown node " + node);
                }
                
                if (node.foreigns[controller.id] === undefined) {
                    throw new Error("An attempt to subscribe not registered controller to node " + node);
                }
                node.foreigns[controller.id].subscribed = true;
                controller.subscribe();
            },
            "unregisterForeignController": function(node, controller) {
                var node = this._nodes[node];
                if (node === undefined) {
                    throw new Error("An attempt to unregister a controller from an unknown node " + node);
                }
                
                if (node.foreigns[controller.id] === undefined) {
                    throw new Error("An attempt to unregister not registered controller from node " + node);
                }
                delete node.foreigns[controller.id];
                controller.unregister();
            },
            "unsubscribeForeignController": function(node, controller) {
                var node = this._nodes[node];
                if (node === undefined) {
                    throw new Error("An attempt to unsubscribe a controller from an unknown node " + node);
                }
                
                if (node.foreigns[controller.id] === undefined) {
                    throw new Error("An attempt to unsubscribe not registered controller from node " + node);
                }
                node.foreigns[controller.id].subscribed = false;
                controller.unsubscribe();
            }
        });
        
        return Lorgar;
    });
})();
