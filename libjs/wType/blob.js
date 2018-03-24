"use strict";

var Object = require("./object");

var Blob = Object.inherit({
    "className": "Blob",
    "constructor": function(/*ArrayBuffer*/data, /*Number*/addSize) {
        Object.fn.constructor.call(this);
        
        data = data || new ArrayBuffer(0);
        this._data = data;
        this._additionalSize = addSize || 0;
    },
    "==": function(other) {
        if (this.getType() !== other.getType()) {
            return false;
        }
        
        return this.size() == other.size();         //TODO let's pretend one shall never wish to compare blobs) 
    },
    "base64": function() {
        var arr = new Uint8Array(this._data);
        var bin = "";
        for (var i = 0; i < arr.length; ++i) {
            bin += String.fromCharCode(arr[i]);
        }
        
        return btoa(bin);
    },
    "clone": function() {
        var clone = new Blob(this._data.slice(0), this._additionalSize);
        
        return clone;
    },
    "size": function() {
        return this._data.byteLength;
    },
    "deserialize": function(ba) {
        var length = Object.pop32uint(ba);
        this._data = new ArrayBuffer(length);
        var view = new Uint8Array(this._data);
        
        for (var i = 0; i < length; ++i) {
            view[i] = ba.pop();
        }
    },
    "serialize": function(ba) {
        Object.push32uint(this._data.byteLength, ba);
        var view = new Uint8Array(this._data);
        
        for (var i = 0; i < view.length; ++i) {
            ba.push(view[i]);
        }
    },
    "toString": function() {
        return "File <" + this._data.byteLength + ">";
    },
    "valueOf": function() {
        return this._data;
    }
});

module.exports = Blob;
