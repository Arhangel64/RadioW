"use strict";
var Class = require("../utils/class");
var Object = require("./object");
var String = require("./string");
var Uint64 = require("./uint64");
var Vocabulary = require("./vocabulary");
var Address = require("./address");
var Boolean = require("./boolean");
var Event = require("./event");
var Vector = require("./vector");
var Blob = require("./blob");

var ByteArray = Class.inherit({
    "className": "ByteArray",
    "constructor": function(arr) {
        Class.fn.constructor.call(this);
        
        if ((arr !== undefined) && !(arr instanceof Array) && !(arr instanceof Uint8Array)) {
            throw new Error("Wrong argument to construct ByteArray");
        }
        
        this._referenceMode = arr instanceof Uint8Array;
        this._data = arr || [];
        this._index = 0;
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
            case Object.objectType.Blob:
                Type = Blob;
                break;
            default:
                throw new Error("Unsupported data type found during deserialization: " + type);
        }
        
        var obj = new Type();
        obj.deserialize(this);
        
        return obj;
    },
    "size": function() {
        return this._data.length - this._index;
    },
    "push": function(int) {
        if ((int < 0) || (int > 255)) {
            throw new Error("An attempt to push into byte array a number bigger than byte");
        }
        
        if (this._referenceMode) {
            this._data = Array.from(this._data);
            this._referenceMode = false;
        }
        
        this._data.push(int);
    },
    "pop": function() {
        return this._data[this._index++];
        
    },
    "toArrayBuffer": function() {
        var uarr;
        if (this._referenceMode) {
            uarr = this._data;
        } else {
            uarr = new Uint8Array(this._data);
        }
        
        return uarr.buffer;
    }
});

module.exports = ByteArray;
