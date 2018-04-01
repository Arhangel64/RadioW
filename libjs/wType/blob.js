"use strict";

var Object = require("./object");

var Blob = Object.inherit({
    "className": "Blob",
    "constructor": function(/*ArrayBuffer*/data) {
        Object.fn.constructor.call(this);
        
        data = data || new ArrayBuffer(0);
        this._data = data;
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
        var clone = new Blob(this._data.slice(0));
        
        return clone;
    },
    "deserialize": function(ba) {
        var length = ba.pop32();
        this._data = new ArrayBuffer(length);
        var view = new Uint8Array(this._data);
        
        for (var i = 0; i < length; ++i) {
            view[i] = ba.pop8();
        }
    },
    "length": function() {
        return this._data.byteLength;
    },
    "serialize": function(ba) {
        ba.push32(this._data.byteLength);
        var view = new Uint8Array(this._data);
        
        for (var i = 0; i < view.length; ++i) {
            ba.push8(view[i]);
        }
    },
    "size": function() {
        return this._data.byteLength + 4;
    },
    "toString": function() {
        return "File <" + this._data.byteLength + ">";
    },
    "valueOf": function() {
        return this._data;
    }
});

module.exports = Blob;
