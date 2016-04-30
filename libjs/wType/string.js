"use strict";
var Object = require("./object");
        
var String = Object.inherit({
    "className": "String",
    "constructor": function(source) {
        Object.fn.constructor.call(this);
        
        this._data = "";
        this._parseSource(source || "");
    },
    "destructor": function () {
        this.clear();
        
        Object.fn.destructor.call(this);
    },
    "<": function(other) {
        if (!(other instanceof String)) {
            throw new Error("Can compare String only with String");
        }
        return this._data < other._data;
    },
    ">": function(other) {
        if (!(other instanceof String)) {
            throw new Error("Can compare String only with String");
        }
        return this._data > other._data;
    },
    "==": function(other) {
        if (!(other instanceof String)) {
            throw new Error("Can compare String only with String");
        }
        
        return this._data === other._data;
    },
    "+=": function(str) {
        this._data += str.toString();
    },
    "clear": function() {
        this._data = "";
    },
    "clone": function() {
        var clone = new String(this._data);
        
        return clone;
    },
    "deserialize": function(ba) {
        this.clear();
        var size = Object.pop32int(ba);
        
        for (var i = 0; i < size; ++i) {
            var h = ba.pop();
            var l = ba.pop();
            
            this._data += global.String.fromCharCode((h << 4) | l);
        }
    },
    "serialize": function(ba) {
        Object.push32int(this._data.length, ba);
        
        for (var i = 0; i < this._data.length; ++i) {
            var code = this._data.charCodeAt(i);
            ba.push(code >> 8);
            ba.push(code & 0xff);
        }
    },
    "size": function() {
        return this._data.length;
    },
    "toString": function() {
        return this._data;
    },
    "valueOf": function() {
        return this.toString();
    },
    "_parseSource": function(source) {
        if (typeof source !== "string") {
            throw new Error("Wrong argument to construct String");
        }
        this._data = source;
    }
});

module.exports = String;