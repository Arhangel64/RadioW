"use strict";
var Object = require("./object");
        
var Uint64 = Object.inherit({
    "className": "Uint64",
    "constructor": function(int) {
        Object.fn.constructor.call(this);
        
        this._h = 0;
        this._l = 0;
        
        this._parseSource(int || 0);
    },
    "<": function(other) {
        if (!(other instanceof Uint64)) {
            throw new Error("Can compare Uint64 only with Uint64");
        }
        if (this._h < other._h) {
            return true;
        } else if(this._h === other._h) {
            return this._l < other._l;
        } else {
            return false;
        }
    },
    ">": function(other) {
        if (!(other instanceof Uint64)) {
            throw new Error("Can compare Uint64 only with Uint64");
        }
        if (this._h > other._h) {
            return true;
        } else if(this._h === other._h) {
            return this._l > other._l;
        } else {
            return false;
        }
    },
    "==": function(other) {
        if (this.getType() !== other.getType()) {
            return false;
        }
        return (this._h == other._h) && (this._l == other._l);
    },
    "++": function() {
        ++this._l;
        if (this._l === 4294967296) {
            this._l = 0;
            ++this._h;
        }
    },
    "clone": function() {
        var clone = new Uint64();
        clone._l = this._l;
        clone._h = this._h;
        
        return clone;
    },
    "deserialize": function(ba) {
        this._h = ba.pop32();
        this._l = ba.pop32();
    },
    "length": function() {
        return 1;
    },
    "serialize": function(ba) {
        ba.push32(this._h);
        ba.push32(this._l);
    },
    "size": function() {
        return 8;
    },
    "toString": function() {
        if (this._h !== 0) {
            console.log(this._h);
            console.log(this._l);
            throw new Error("Don't know yet how to show uint64 in javascript");
        }
        return this._l.toString();
    },
    "valueOf": function() {
        if (this._h !== 0) {
            throw new Error("Don't know yet how to show uint64 in javascript");
        }
        return this._l;
    },
    "_parseSource": function(int) {
        if (parseInt(int) !== int) {
            throw new Error("Wrong argument to construct Uint64");
        }
        
        this._l = int & 0xffffffff;
    }
});

module.exports = Uint64;
