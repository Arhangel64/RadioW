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
        
        this._length = Object.pop32uint(ba);
        
        for (var i = 0; i < this._length; ++i) {
            var key = new String();
            key.deserialize(ba);
            
            var value = ba[">>"]();
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
    "serialize": function(ba) {
        Object.push32uint(this._length, ba);
        
        for (var key in this._data) {
            var sKey = new String(key);
            sKey.serialize(ba);
            
            ba["<<"](this._data[key]);
        }
    },
    "size": function() {
        return this._length;
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
