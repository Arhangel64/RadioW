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
        if (this.getType() !== other.getType()) {
            return false;
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
        var size = ba.pop32();
        
        for (var i = 0; i < size; ++i) {
            var cc = ba.pop16();
            this._data += global.String.fromCharCode(cc);
        }
    },
    "length": function() {
        return this._data.length;
    },
    "serialize": function(ba) {
        ba.push32(this._data.length);
        
        for (var i = 0; i < this._data.length; ++i) {
            var code = this._data.charCodeAt(i);
            ba.push16(code);
        }
    },
    "size": function() {
        return this._data.length * 2 + 4;
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
