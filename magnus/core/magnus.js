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

var Magnus = Subscribable.inherit({
    "className": "Magnus",
    "constructor": function() {
        Subscribable.fn.constructor.call(this);
        
        this.dispatcher = new Dispatcher();
        this._h_test = new Handler(new Address(["magnus", "test"]), this, this._test);
        this.dispatcher.registerHandler(this._h_test);
        
        this.server = new Server("Magnus");
        this.server.on("newConnection", this._onNewConnection, this);
        this.server.listen(8081);
        
        this.coraxSocket = new Socket("Magnus");
        this.coraxSocket.on("connected", this._onCoraxConnected, this);
        this.coraxSocket.on("message", this.dispatcher.pass, this.dispatcher);
        this.coraxSocket.open("localhost", 8080);
        
        console.log("Magnus is listening on port 8081");
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
    },
    "_test": function(e) {
        console.log(e.toString());
        
        var data = e.getData();
        var lt = new Address(["lorgar", "test"]);
        var src = data.at("source");
        
        if (src["=="](lt)) {
            var socket = this.server.getConnection(e.getSenderId());
            
            var address = lt.clone();
            var vc = new Vocabulary();
            vc.insert("msg", new String("Hello, I'm Magnus"));
            vc.insert("source", new Address(["magnus", "test"]));
        
            var ev = new Event(address, vc);
            ev.setSenderId(socket.getId().clone());
            socket.send(ev);
            ev.destructor()
        }
    }
});

Magnus.onSocketDisconnected = function() {
    console.log("Connection closed, id: " + this.soc.getId().toString());
}

module.exports = Magnus;
