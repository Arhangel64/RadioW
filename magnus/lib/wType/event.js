"use strict";
var Object = require("./object");
var Uint64 = require("./uint64");
var Address = require("./address");
var Boolean = require("./boolean");

var Event = Object.inherit({
    "className": "Event",
    "constructor": function(addr, object, isSystem) {
        Object.fn.constructor.call(this);
        
        if (!(object instanceof Object) && object !== undefined) {
            throw new Error("Wrong arguments to construct Event");
        }
        if (!(addr instanceof Address) && addr !== undefined) {
            throw new Error("Wrong arguments to construct Event");
        }
        
        this._destination = addr !== undefined ? addr : new Address();
        this._data = object !== undefined ? object : new Object();
        this._senderId = new Uint64();
        this._system = new Boolean(isSystem);
    },
    "destructor": function() {
        this.clear();
        
        Object.fn.destructor.call(this);
    },
    "==": function(other) {
        if (!(other instanceof Event)) {
            throw new Error("Can compare Event only with Event");
        }
        return  this._destination["=="](other._destination) &&
                this._system["=="](other._system) &&
                this._senderId["=="](other._senderId) &&
                this._data["=="](other._data)
    },
    "clear": function() {
        this._system.destructor();
        this._destination.destructor();
        this._senderId.destructor();
        this._data.destructor();
    },
    "clone": function() {
        var clone = new Event();
        
        clone._destination = this._destination.clone();
        clone._data = this._data.clone();
        clone._senderId = this._senderId.clone();
        clone._system = this._system.clone();
        
        return clone;
    },
    "deserialize": function(ba) {
        this._system = new Boolean();
        this._system.deserialize(ba);
        
        if (!this.isSystem()) { 
            this._destination = new Address();
            this._destination.deserialize(ba);
        
            this._senderId = new Uint64();
            this._senderId.deserialize(ba);
        }
        
        this._data = ba[">>"]();
    },
    "getData": function() {
        return this._data;
    },
    "getDestination": function() {
        return this._destination;
    },
    "isSystem": function() {
        return this._system.valueOf();
    },
    "serialize": function(ba) {
        this._system.serialize(ba);
        
        if (!this.isSystem()) {
            this._destination.serialize(ba);
            this._senderId.serialize(ba);
        }
        
        ba["<<"](this._data);
    },
    "setSenderId": function(id) {
        if (!(id instanceof Uint64)) {
            throw new Error("Can't set id, which is not Uint64");
        }
        this._senderId = id;
    },
    "toString": function() {
        var str = "{";
        
        str += "\nsystem: " + this._system.toString();
        str += "\ndestination: " + this._destination.toString();
        str += "\nsender: " + this._senderId.toString();
        str += "\ndata: " + this._data.toString();
        
        str += "\n}";
        
        return str;
    }
});

module.exports = Event;