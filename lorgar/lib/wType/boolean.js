"use strict";
(function boolean_js() {
    var moduleName = "lib/wType/boolean";
    
    var defineArray = [];
    defineArray.push("lib/wType/object");
    
    define(moduleName, defineArray, function boolean_module() {
        var Object = require("lib/wType/object");
        
        var Boolean = Object.inherit({
            "className": "Boolean",
            "constructor": function(bool) {
                Object.fn.constructor.call(this);
                
                this._data = bool === true;
            },
            "<": function(other) {
                if (!(other instanceof Boolean)) {
                    throw new Error("Can compare Boolean only with Boolean");
                }
                return this._data < other._data;
            },
            ">": function(other) {
                if (!(other instanceof Boolean)) {
                    throw new Error("Can compare Boolean only with Boolean");
                }
                return this._data > other._data;
            },
            "==": function(other) {
                if (!(other instanceof Boolean)) {
                    throw new Error("Can compare Boolean only with Boolean");
                }
                return this._data === other._data;
            },
            "clone": function() {
                return new Boolean(this._data);
            },
            "deserialize": function(ba) {
                var int = ba.pop_front();
                
                if (int === 0) {
                    this._data = false;
                } else {
                    this._data = true;
                }
            },
            "serialize": function(ba) {
                if (this._data) {
                    ba.push_back(0xffffffff);
                } else {
                    ba.push_back(0);
                }
            },
            "toString": function() {
                return this._data.toString();
            },
            "valueOf": function() {
                return this._data;
            }
        });
        
        return Boolean;
    });
})();