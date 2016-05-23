"use strict";
var Object = require("./object");
var String = require("./string");
var Uint64 = require("./uint64");
var Vocabulary = require("./vocabulary");
var Address = require("./address");
var Boolean = require("./boolean");
var Event = require("./event");
var Vector = require("./vector");

var ByteArray = Object.inherit({
    "className": "ByteArray",
    "constructor": function(arr) {
        Object.fn.constructor.call(this);
        
        if ((arr !== undefined) && !(arr instanceof Array) && !(arr instanceof Uint8Array)) {
            throw new Error("Wrong argument to construct ByteArray");
        }
        
        if (arr instanceof Uint8Array) {
            var raw = arr;
            arr = Array.from(raw);
        }
        
        this._data = arr || [];
    },
    "<<": function(obj) {
        if (!(obj instanceof Object)) {
            throw new Error("An attempt to serialize not a W::Object");
        }
        this.push(obj.getType());
        obj.serialize(this);
    },
    ">>": function() {
        var type = this.pop();
        var Type;
        
        switch (type) {
            case Object.objectType.String:
                Type = String;
                break;
            case Object.objectType.Uint64:
                Type = Uint64;
                break;
            case Object.objectType.Vocabulary:
                Type = Vocabulary;
                break;
            case Object.objectType.Address:
                Type = Address;
                break;
            case Object.objectType.Boolean:
                Type = Boolean;
                break;
            case Object.objectType.Event:
                Type = Event;
                break;
            case Object.objectType.Vector:
                Type = Vector;
                break;
            default:
                throw new Error("Unsupported data type found during deserialization: " + type);
        }
        
        var obj = new Type();
        obj.deserialize(this);
        
        return obj;
    },
    "size": function() {
        return this._data.length;
    },
    "push": function(int) {
        
        if ((int < 0) || (int > 255)) {
            throw new Error("An attempt to push into byte array a number bigger than byte");
        }
        
        this._data.push(int);
    },
    "pop": function() {
        return this._data.shift();
    },
    "toArrayBuffer": function() {
        var uarr = new Uint8Array(this._data);
        
        return uarr.buffer;
    }
});

module.exports = ByteArray;