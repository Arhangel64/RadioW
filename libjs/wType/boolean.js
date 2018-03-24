"use strict";
var Object = require("./object");
        
var Boolean = Object.inherit({
    "className": "Boolean",
    "constructor": function(bool) {
        Object.fn.constructor.call(this);
        
        this._data = bool === true;
    },
    "<": function(other) {
        if (!(other instanceof Boolean)) {
            throw new Error("Can compare Boolean only with Boolean");
        }
        return this._data < other._data;
    },
    ">": function(other) {
        if (!(other instanceof Boolean)) {
            throw new Error("Can compare Boolean only with Boolean");
        }
        return this._data > other._data;
    },
    "==": function(other) {
        if (this.getType() !== other.getType()) {
            return false;
        }
        return this._data === other._data;
    },
    "clone": function() {
        return new Boolean(this._data);
    },
    "deserialize": function(ba) {
        var int = ba.pop();
        
        if (int === 253) {
            this._data = true;
        } else {
            this._data = false;
        }
    },
    "serialize": function(ba) {
        if (this._data) {
            ba.push(253);
        } else {
            ba.push(0);
        }
    },
    "toString": function() {
        return this._data.toString();
    },
    "valueOf": function() {
        return this._data;
    }
});

module.exports = Boolean;
