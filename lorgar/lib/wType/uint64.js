"use strict";
(function uint64_js() {
    var moduleName = "lib/wType/uint64"
    
    var defineArray = [];
    defineArray.push("lib/wType/object");
    
    define(moduleName, defineArray, function uint64_module() {
        var Object = require("lib/wType/object");
        
        var Uint64 = Object.inherit({
            "className": "Uint64",
            "constructor": function(int) {
                Object.fn.constructor.call(this);
                
                this._h = 0;
                this._l = 0;
                
                this._parseSource(int | 0);
            },
            "<": function(other) {
                if (!(other instanceof Uint64)) {
                    throw new Error("Can compare Uint64 only with Uint64");
                }
                if (this._h < other._h) {
                    return true;
                } else if(this._h === other._h) {
                    return this._l < other._l;
                } else {
                    return false;
                }
            },
            ">": function(other) {
                if (!(other instanceof Uint64)) {
                    throw new Error("Can compare Uint64 only with Uint64");
                }
                if (this._h > other._h) {
                    return true;
                } else if(this._h === other._h) {
                    return this._l > other._l;
                } else {
                    return false;
                }
            },
            "==": function(other) {
                if (!(other instanceof Uint64)) {
                    throw new Error("Can compare Uint64 only with Uint64");
                }
                return (this._h == other._h) && (this._l == other._l);
            },
            "clone": function() {
                var clone = new Uint64();
                clone._l = this._l;
                clone._h = this._h;
                
                return clone;
            },
            "deserialize": function(ba) {
                this._h = ba.pop_front();
                this._l = ba.pop_front();
            },
            "serialize": function(ba) {
                ba.push_back(this._h);
                ba.push_back(this._l);
            },
            "size": function() {
                return 2;
            },
            "toString": function() {
                if (this._h !== 0) {
                    throw new Error("Don't know yet how to show uint64 in javascript");
                }
                return this._l.toString();
            },
            "valueOf": function() {
                if (this._h !== 0) {
                    throw new Error("Don't know yet how to show uint64 in javascript");
                }
                return this._l;
            },
            "_parseSource": function(int) {
                if (((int | 0) !== int) || int < 0) {
                    throw new Error("Wrong argument to construct Uint64");
                }
                
                this._l = int;
            }
        });
        
        return Uint64;
    });
})();