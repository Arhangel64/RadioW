"use strict";
(function bytearray_js() {
    var moduleName = "lib/wType/bytearray";
    
    var defineArray =[];
    defineArray.push("lib/utils/class");
    
    define(moduleName, defineArray, function bytearray_module() {
        var Class = require("lib/utils/class");
        
        var ByteArray = Class.inherit({
            "className": "ByteArray",
            "constructor": function(uint8arrr) {
                Class.fn.constructor.call(this);
                
                if ((uint8arrr !== undefined) && !(uint8arrr instanceof Uint8Array)) {
                    throw new Error("Wrong argument to construct ByteArray");
                }
                
                this._data = [];
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