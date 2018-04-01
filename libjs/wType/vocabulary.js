"use strict";
var Object = require("./object");
var String = require("./string");

var Vocabulary = Object.inherit({
    "className": "Vocabulary",
    "constructor": function() {
        Object.fn.constructor.call(this);
        
        this._data = global.Object.create(null);
        this._length = 0;
    },
    "destructor": function() {
        this.clear();
        
        Object.fn.destructor.call(this);
    },
    "==": function(other) {
        if (this.getType() !== other.getType()) {
            return false;
        }
        
        if (this._length !== other._length) {
            return false;
        }
        
        var keysMe = this.getKeys();
        var key;
        var mValue;
        var oValue;
        
        for (var i = 0; i < this._length; ++i) {
            key = keysMe[i];
            oValue = other._data[key];
            if (oValue === undefined) {
                return false;
            }
            mValue = this._data[key];
            if (!oValue["=="](mValue)) {
                return false;
            }
        }
        return true;
    },
    "at": function(str) {
        return this._data[str];
    },
    "clear": function() {
        for (var key in this._data) {
            this._data[key].destructor();
        }
        
        this._data = global.Object.create(null);
        this._length = 0;
    },
    "clone": function() {
        var clone = new Vocabulary();
        
        for (var key in this._data) {
            clone._data[key] = this._data[key].clone();
        }
        clone._length = this._length;
        
        return clone;
    },
    "deserialize": function(ba) {
        this.clear();
        
        this._length = ba.pop32()
        
        for (var i = 0; i < this._length; ++i) {
            var key = new String();
            key.deserialize(ba);
            
            var value = Object.fromByteArray(ba);
            this._data[key.toString()] = value;
        }
    },
    "erase": function(key) {
        var value = this._data[key];
        if (value === undefined) {
            throw new Error("An attempt to erase not existing object from vocabulary");
        }
        value.destructor();
        delete this._data[key];
        --this._length;
    },
    "getKeys": function() {
        return global.Object.keys(this._data);
    },
    "has": function(key) {
        return this._data[key] instanceof Object;
    },
    "insert": function(key, value) {
        if (!(value instanceof Object)) {
            throw new Error("An attempt to insert not a W::Object into vocabulary");
        }
        var oldValue = this._data[key];
        if (oldValue !== undefined) {
            oldValue.destructor();
            --this._length;
        }
        this._data[key] = value
        
        ++this._length;
    },
    "length": function() {
        return this._length;
    },
    "serialize": function(ba) {
        ba.push32(this._length);
        
        for (var key in this._data) {
            var sKey = new String(key);
            var value = this._data[key];
            
            sKey.serialize(ba);
            ba.push8(value.getType());
            value.serialize(ba);
        }
    },
    "size": function() {
        var size = 4;
        
        for (var key in this._data) {
            var sKey = new String(key);
            var value = this._data[key];
            
            size += sKey.size();
            size += value.size() + 1;
        }
        
        return size;
    },
    "toString": function() {
        var str = "{";
        
        var ft = true;
        
        for (var key in this._data) {
            if (ft) {
                ft = false;
            } else {
                str += ", ";
            }
            str += key + ": ";
            str += this._data[key].toString();
            
        }
        str += "}";
        return str;
    }
});

module.exports = Vocabulary;
