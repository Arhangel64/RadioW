"use strict";
(function char32_js() {
    var moduleName = "lib/wType/char32";
    
    var defineArray = [];
    defineArray.push("lib/wType/object");
    
    define(moduleName, defineArray, function char32_module() {
        var Object = require("lib/wType/object");
        
        var Char32 = Object.inherit({
            "className": "Char32",
            "constructor": function(source) {
                Object.fn.constructor.call(this);
                
                this._parseSource(source || 0);
            },
            "toString": function() {
                return String.fromCharCode(this._l);
            },
            "valueOf": function() {
                return (this._h << 16) | this._l;
            },
            "size": function() {
                return 1;
            },
            "_parseSource": function(source) {
                if ((source | 0) !== source) {
                    throw new Error("Wrong argument to construct Char32");
                }
                this._h = source >> 16;
                this._l = source & 0xffff;
            }
        });
        
        Char32.fromChar = function(char) {
            return new Char32(char.charCodeAt(0));
        };
        Char32.fromCharCode = function(charCode) {
            return new Char32(charCode);
        }
        
        return Char32;
    });
})();