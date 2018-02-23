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
        
        var length = Object.pop32uint(ba);
        
        for (var i = 0; i < length; ++i) {
            var value = ba[">>"]();
            this.push(value);
        }
    },
    "push": function(value) {
        if (!(value instanceof Object)) {
            throw new Error("An attempt to insert not a W::Object into vocabulary");
        }
        this._data.push(value);
    },
    "serialize": function(ba) {
        Object.push32uint(this._data.length, ba);
        
        for (var i = 0; i < this._data.length; ++i) {
            ba["<<"](this._data[i]);
        }
    },
    "size": function() {
        return this._data.length;
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
