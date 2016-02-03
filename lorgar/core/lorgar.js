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
                
                this.socket = new Socket("Lorgar");
                this.dispatcher = new Dispatcher();
                this._h_test = new Handler(new Address(["lorgar", "test"]), this, this._test);
                
                this.dispatcher.registerHandler(this._h_test);
                
                this.socket.on("connected", this._socketConnected, this);
                this.socket.on("disconnected", this._socketDisconnected, this);
                this.socket.on("error", this._socketError, this);
                
                this.socket.on("message", this.dispatcher.pass, this.dispatcher);
                
                this.socket.open("localhost", 8080);
            },
            "destructor": function() {
                this.socket.close();
                this.dispatcher.unregisterHandler(this._h_test);
                
                this._h_test.destructor();
                this.dispatcher.destructor();
                this.socket.destructor();
                
                Class.fn.destructor.call(this);
            },
            "_socketConnected": function() {
                var address = new Address(["corax", "test"]);
                var vc = new Vocabulary();
                vc.insert("msg", new String("Hello, I'm Lorgar"));
                vc.insert("source", new Address(["lorgar", "test"]));
                
                var ev = new Event(address, vc);
                ev.setSenderId(this.socket.getId());
                
                this.socket.send(ev);
            },
            "_socketDisconnected": function() {
                console.log("socket disconnected");
            },
            "_socketError": function(e) {
                console.log("socket error: ");
                console.log(e);
            },
            "_test": function(e) {
                console.log(e);
            }
        });
        
        return Lorgar;
    });
})();