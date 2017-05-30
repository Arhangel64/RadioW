"use strict";

var DefaultHandler = require("./defaulthandler");
var Handler = require("./handler");

var Address = require("../wType/address");
var AbstractMap = require("../wContainer/abstractmap");

var HandlersMap = AbstractMap.template(Address, Handler);

var ParentReporter = DefaultHandler.inherit({
    "className": "ParentReporter",
    "constructor": function() {
        DefaultHandler.fn.constructor.call(this);
        
        this._handlers = new HandlersMap(false);
    },
    "destructor": function() {
        this._handlers.destructor();
        
        DefaultHandler.fn.destructor.call(this);
    },
    "call": function(ev) {
        var addr = ev.getDestination();
        var result = [];
        
        var itr = this._handlers.begin();
        var end = this._handlers.end();
        for (; !itr["=="](end); itr["++"]()) {
            var pair = itr["*"]();
            if (addr.begins(pair.first)) {
                result[pair.first.size()] = pair.second;        //it's a dirty javascript trick
            }                                                   //I need the longest of matching, and that's how I'm gonna get it
        }
        if (result.length) {
            result[result.length - 1].pass(ev);
            return true;
        } else {
            return false;
        }
    },
    "registerParent": function(parentAddr, handler) {
        this._handlers.insert(parentAddr, handler);
    },
    "unregisterParent": function(parentAddr) {
        var itr = this._handlers.find(parentAddr);
        if (!itr["=="](this._handlers.end())) {
            this._handlers.erase(itr);
        } else {
            throw new Error("An attempt to unregister unregistered parent in ParentReporter");
        }
    }
});

module.exports = ParentReporter;
