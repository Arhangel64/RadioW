"use strict";
(function vocabulary_js() {
    var moduleName = "lib/wType/vocabulary";
    
    var defineArray = [];
    defineArray.push("lib/wType/object");
    defineArray.push("lib/wType/string");
    
    define(moduleName, defineArray, function vocabulary_module() {
        var Object = require("lib/wType/object");
        var String = require("lib/wType/string");
        
        var Vocabulary = Object.inherit({
            "className": "Vocabulary",
            "constructor": function() {
                Object.fn.constructor.call(this);
                
                this._data = window.Object.create(null);
                this._length = 0;
            },
            "destructor": function() {
                this.clear();
                
                Object.fn.destructor.call(this);
            },
            "at": function(str) {
                return this._data[str];
            },
            "clear": function() {
                for (var key in this._data) {
                    this._data[key].destructor();
                }
                
                this._data = window.Object.create(null);
                this._length = 0;
            },
            "clone": function() {
                var clone = new Vocabulary();
                
                for (var key in this._data) {
                    clone._data[key] = this._data[key].clone();
                }
                clone._length = this._length;
                
                return clone;
            },
            "deserialize": function(ba) {
                this.clear();
                
                this._length = Object.pop32int(ba);
                
                for (var i = 0; i < this._length; ++i) {
                    var key = new String();
                    key.deserialize(ba);
                    
                    var value = ba[">>"]();
                    this._data[key.toString()] = value;
                }
            },
            "insert": function(key, value) {
                if (!(value instanceof Object)) {
                    throw new Error("An attempt to insert not a W::Object into vocabulary");
                }
                this._data[key] = value
                
                ++this._length;
            },
            "serialize": function(ba) {
                Object.push32int(this._length, ba);
                
                for (var key in this._data) {
                    var sKey = new String(key);
                    sKey.serialize(ba);
                    
                    ba["<<"](this._data[key]);
                }
            },
            "size": function() {
                return this._length;
            },
            "toString": function() {
                var str = "{";
                
                var ft = true;
                
                for (var key in this._data) {
                    if (ft) {
                        ft = false;
                    } else {
                        str += ", ";
                    }
                    str += key;
                    str += this._data[key].toString();
                    
                }
                str += "}";
                return str;
            }
        });
        
        return Vocabulary;
    });
})();