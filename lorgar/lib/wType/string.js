"use strict";
(function string_js() {
    var moduleName = "lib/wType/string";
    
    var defineArray = [];
    defineArray.push("lib/wType/object");
    defineArray.push("lib/wType/char32");
    
    define(moduleName, defineArray, function string_module() {
        var Object = require("lib/wType/object");
        var Char32 = require("lib/wType/char32");
        
        var String = Object.inherit({
            "className": "String",
            "constructor": function(source) {
                Object.fn.constructor.call(this);
                
                this._data = [];
                this._parseSource(source || "");
            },
            "deserialize": function(ba) {
                var size = ba.pop_front();
                
                for (var i = 0; i < size; ++i) {
                    this._data.push(Char32.fromCharCode(ba.pop_front()));
                }
            },
            "serialize": function(ba) {
                ba.push_back(this.size());
                
                for (var i = 0; i < this._data.length; ++i) {
                    ba.push_back(this._data[i].valueOf());
                }
            },
            "size": function() {
                return this._data.length;
            },
            "toString": function() {
                var ret = "";
                for (var i = 0; i < this._data.length; ++i) {
                    ret += this._data[i].toString();
                }
                return ret;
            },
            "valueOf": function() {
                return this.toString();
            },
            "_parseSource": function(source) {
                if (typeof source !== "string") {
                    throw new Error("Wrong argument to construct String");
                }
                for (var i = 0; i < source.length; ++i) {
                    this._data.push(Char32.fromChar(source[i]));
                }
            }
        });
        
        return String;
    });
})();