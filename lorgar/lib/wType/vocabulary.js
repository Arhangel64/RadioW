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
                return this._data[str].clone(); //TODO may be, it's better to clone?
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
                this._length = ba.pop_front();
                
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
                this._data[key] = value.clone(); //TODO may be, it's better to clone?
                
                ++this._length;
            },
            "serialize": function(ba) {
                ba.push_back(this._length);
                
                for (var key in this._data) {
                    var sKey = new String(key);
                    sKey.serialize(ba);
                    
                    ba["<<"](this._data[key]);
                }
            },
            "size": function() {
                return this._length;
            }
        });
        
        return Vocabulary;
    });
})();