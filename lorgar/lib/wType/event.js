"use strict";
(function event_js() {
    var moduleName = "lib/wType/event";
    
    var defineArray = [];
    defineArray.push("lib/wType/object")
    defineArray.push("lib/wType/uint64");
    defineArray.push("lib/wType/address");
    
    define(moduleName, defineArray, function event_module() {
        var Object = require("lib/wType/object");
        var Uint64 = require("lib/wType/uint64");
        var Address = require("lib/wType/address");
        
        var Event Object.inherit({
            "className": "Event",
            "constructor": function() {
                Object.fn.constructor.call(this);
                
                this._destination;
                this._data;
                this._senderId;
                this._system;
            },
            "==": function(other) {
                return  this._destination["=="](other._destination) &&
                        this._system["=="](other._system) &&
                        this._senderId["=="](other._senderId) &&
                        this._data["=="](other._data)
            },
            "clone": function(other) {
                var clone = new Event();
                
                clone._destination = this._destination.clone();
                clone._data = this._data.clone();
                clone._senderId = this._senderId.clone();
                clone._system = this._system.clone();
                
                return clone;
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
        
        return Event;
    });
})();