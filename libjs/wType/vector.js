"use strict";
var Object = require("./object");

var Vector = Object.inherit({
    "className": "Vector",
    "constructor": function() {
        Object.fn.constructor.call(this);
        
        this._data = [];
    },
    "destructor": function() {
        this.clear();
        
        Object.fn.destructor.call(this);
    },
    "==": function(other) {
        if (this.getType() !== other.getType()) {
            return false;
        }
        
        if (this._data.length !== other._data.length) {
            return false;
        }
        
        var hopMe;
        var hopOt;
        
        for (var i = 0; i < this._data.length; ++i) {
            hopMe = this._data[i];
            hopOt = other._data[i];
            if ( !(hopMe["=="](hopOt)) ) {
                return false;
            }
        }
        return true;
    },
    "at": function(index) {
        return this._data[index];
    },
    "clear": function() {
        for (var i = 0; i < this._data.length; ++i) {
            this._data[i].destructor();
        }
        
        this._data = [];
    },
    "clone": function() {
        var clone = new Vector();
        
        for (var i = 0; i < this._data.length; ++i) {
            clone.push(this._data[i].clone());
        }
        
        return clone;
    },
    "deserialize": function(ba) {
        this.clear();
        
        var length = ba.pop32();
        
        for (var i = 0; i < length; ++i) {
            var value = Object.fromByteArray(ba);
            this.push(value);
        }
    },
    "length": function() {
        return this._data.length;
    },
    "push": function(value) {
        if (!(value instanceof Object)) {
            throw new Error("An attempt to insert not a W::Object into a vector");
        }
        this._data.push(value);
    },
    "serialize": function(ba) {
        ba.push32(this._data.length);
        
        for (var i = 0; i < this._data.length; ++i) {
            var el = this._data[i];
            ba.push8(el.getType());
            el.serialize(ba);
        }
    },
    "size": function() {
        var size = 4;
        
        for (var i = 0; i < this._data.length; ++i) {
            size += this._data[i].size() + 1;
        }
        
        return size;
    },
    "toString": function() {
        var str = "[";
        
        var ft = true;
        
        for (var i = 0; i < this._data.length; ++i) {
            if (ft) {
                ft = false;
            } else {
                str += ", ";
            }
            str += this._data[i].toString();
            
        }
        str += "]";
        return str;
    }
});

module.exports = Vector;
