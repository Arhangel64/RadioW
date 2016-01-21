"use strict";
(function bytearray_js() {
    var moduleName = "lib/wType/bytearray";
    
    var defineArray =[];
    defineArray.push("lib/wType/object");
    
    defineArray.push("lib/wType/string");
    defineArray.push("lib/wType/uint64");
    defineArray.push("lib/wType/vocabulary");
    defineArray.push("lib/wType/address");
    
    define(moduleName, defineArray, function bytearray_module() {
        var Object = require("lib/wType/object");
        var String = require("lib/wType/string");
        var Uint64 = require("lib/wType/uint64");
        var Vocabulary = require("lib/wType/vocabulary");
        var Address = require("lib/wType/address");
        
        var ByteArray = Object.inherit({
            "className": "ByteArray",
            "constructor": function(arr) {
                Object.fn.constructor.call(this);
                
                if ((arr !== undefined) && !(uint8arrr instanceof Array)) {
                    throw new Error("Wrong argument to construct ByteArray");
                }
                
                this._data = arr || [];
            },
            "<<": function(obj) {
                if (!(obj instanceof Object)) {
                    throw new Error("An attempt to serialize not a W::Object");
                }
                this.push_back(obj.getType());
                obj.serialize(this);
            },
            ">>": function() {
                var type = this.pop_front();
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
                    default:
                        throw new Error("Unsupported data type founf during deserialization");
                }
                
                var obj = new Type();
                obj.deserialize(this);
                
                return obj;
            },
            "size": function() {
                return this._data.length;
            },
            "push_back": function(int) {
                if ((int | 0) !== int) {
                    throw new Error("Wrong argument to push into the ByteArray");
                }
                var hh = (int >> 24) & 0xff;
                var hl = (int >> 16) & 0xff;
                var lh = (int >> 8) & 0xff;
                var ll = int & 0xff;
                
                this._data.push(hh);
                this._data.push(hl);
                this._data.push(lh);
                this._data.push(ll);
            },
            "pop_front": function() {
                var temp = new Uint8Array(this._data.splice(0, 4));
                
                if (temp.length != 4) {
                    throw new Error("Not enough data in ByteArray to pop");
                }
                
                var ret = temp[0] << 24;
                ret = ret | (temp[1] << 16);
                ret = ret | (temp[2] << 8);
                ret = ret | temp[3];
                
                return ret;
            }
        });
        
        return ByteArray;
    });
})();