"use strict";
var Class = require("../utils/class");
var id = 0;

var Handler = Class.inherit({
    "className": "Handler",
    "constructor": function(address, instance, method) {
        Class.fn.constructor.call(this);
        
        this._id = id++;
        
        this.address = address;
        this._ctx = instance;
        this._mth = method;
    },
    "destructor": function() {
        this.address.destructor();
        
        Class.fn.destructor.call(this);
    },
    "pass": function(event) {
        this._mth.call(this._ctx, event);
    },
    "==": function(other) {
        if (!(other instanceof Handler)) {
            throw new Error("Can compare only Handler with Handler");
        }
        return this._id === other._id;
    },
    ">": function(other) {
        if (!(other instanceof Handler)) {
            throw new Error("Can compare only Handler with Handler");
        }
        return this._id > other._id;
    },
    "<": function(other) {
        if (!(other instanceof Handler)) {
            throw new Error("Can compare only Handler with Handler");
        }
        return this._id < other._id;
    }
});

module.exports = Handler;
