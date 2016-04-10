"use strict";
var Subscribable = require("../lib/utils/subscribable");
var Socket = require("../lib/wSocket/wSocket");
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
        
        this.coraxSocket = new Socket("Magnus");
        this.coraxSocket.on("connected", this._onCoraxConnected, this);
        this.coraxSocket.on("message", this.dispatcher.pass, this.dispatcher);
        this.coraxSocket.open("localhost", 8080);
    },
    "_onCoraxConnected": function() {
        var address = new Address(["corax", "test"]);
        var vc = new Vocabulary();
        vc.insert("msg", new String("Hello, I'm Magnus"));
        vc.insert("source", new Address(["magnus", "test"]));
        
        var ev = new Event(address, vc);
        ev.setSenderId(this.coraxSocket.getId());
        
        this.coraxSocket.send(ev);
    },
    "_test": function(e) {
        console.log(e);
    }
});

module.exports = Magnus;