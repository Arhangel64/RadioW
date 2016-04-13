"use strict";
(function lorgar_js() {
    var moduleName = "core/lorgar";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    defineArray.push("lib/wSocket/socket");
    defineArray.push("lib/wDispatcher/dispatcher");
    defineArray.push("lib/wDispatcher/handler");
    
    defineArray.push("lib/wType/event");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wType/vocabulary");
    defineArray.push("lib/wType/string");
    
    define(moduleName, defineArray, function lorgar_module() {
        var Class = require("lib/utils/class");
        var Socket = require("lib/wSocket/socket");
        var Dispatcher = require("lib/wDispatcher/dispatcher");
        var Handler = require("lib/wDispatcher/handler");
        var Event = require("lib/wType/event");
        var Address = require("lib/wType/address");
        var Vocabulary = require("lib/wType/vocabulary");
        var String = require("lib/wType/string");
        
        var Lorgar = Class.inherit({
            "className": "Lorgar",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                
                this.dispatcher = new Dispatcher();
                this._h_test = new Handler(new Address(["lorgar", "test"]), this, this._test);
                
                this.dispatcher.registerHandler(this._h_test);
                
                this.coraxSocket = new Socket("Lorgar");
                this.coraxSocket.on("connected", this._coraxSocketConnected, this);
                this.coraxSocket.on("disconnected", this._coraxSocketDisconnected, this);
                this.coraxSocket.on("error", this._coraxSocketError, this);
                this.coraxSocket.on("message", this.dispatcher.pass, this.dispatcher);
                
                this.magnusSocket = new Socket("Lorgar");
                this.magnusSocket.on("connected", this._magnusSocketConnected, this);
                this.magnusSocket.on("disconnected", this._magnusSocketDisconnected, this);
                this.magnusSocket.on("error", this._magnusSocketError, this);
                this.magnusSocket.on("message", this.dispatcher.pass, this.dispatcher);
                
                this.coraxSocket.open("localhost", 8080);
                this.magnusSocket.open("localhost", 8081);
            },
            "destructor": function() {
                this.coraxSocket.close();
                this.dispatcher.unregisterHandler(this._h_test);
                
                this._h_test.destructor();
                this.dispatcher.destructor();
                this.coraxSocket.destructor();
                
                Class.fn.destructor.call(this);
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
            "_magnusSocketConnected": function() {
                var address = new Address(["magnus", "test"]);
                var vc = new Vocabulary();
                vc.insert("msg", new String("Hello, I'm Lorgar"));
                vc.insert("source", new Address(["lorgar", "test"]));
                
                var ev = new Event(address, vc);
                ev.setSenderId(this.magnusSocket.getId());
                
                this.magnusSocket.send(ev);
            },
            "_magnusSocketDisconnected": function() {
                console.log("magnus socket disconnected");
            },
            "_magnusSocketError": function(e) {
                console.log("magnus socket error: ");
                console.log(e);
            },
            "_test": function(e) {
                console.log(e);
            }
        });
        
        return Lorgar;
    });
})();