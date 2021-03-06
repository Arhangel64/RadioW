"use strict";
var Class = require("../utils/class");
var id = 0;

var DefaultHandler = Class.inherit({
    "className": "DefaultHandler",
    "constructor": function() {
        Class.fn.constructor.call(this);
        
        this._id = id++;
    },
    "==": function(other) {
        if (!(other instanceof DefaultHandler)) {
            throw new Error("Can compare only DefaultHandler with DefaultHandler");
        }
        return this._id === other._id;
    },
    ">": function(other) {
        if (!(other instanceof DefaultHandler)) {
            throw new Error("Can compare only DefaultHandler with DefaultHandler");
        }
        return this._id > other._id;
    },
    "<": function(other) {
        if (!(other instanceof DefaultHandler)) {
            throw new Error("Can compare only DefaultHandler with DefaultHandler");
        }
        return this._id < other._id;
    },
    "call": function(event) {
        throw new Error("Attempt to call pure abstract default handler");
    }
});

module.exports = DefaultHandler;
