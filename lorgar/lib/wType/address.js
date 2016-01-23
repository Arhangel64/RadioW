"use strict";
(function address_js() {
    var moduleName = "lib/wType/address";
    
    var defineArray = [];
    defineArray.push("lib/wType/object");
    defineArray.push("lib/wType/string");
    
    define(moduleName, defineArray, function address_module() {
        var Object = require("lib/wType/object");
        var String = require("lib/wType/string");
        
        var Address = Object.inherit({
            "className": "Address",
            "constructor": function(data) {
                Object.fn.constructor.call(this);
                
                this._data = [];
                this._parseSource(data || []);
            },
            "destructor": function() {
                this.clear();
                
                Object.fn.destructor.call(this);
            },
            "<": function(other) {
                if (!(other instanceof Address)) {
                    throw new Error("Can compare Address only with Address");
                }
                
                for (var i = 0; i < this._data.length; ++i) {
                    hopMe = this._data[i];
                    hopOt = other._data[i];
                    
                    if (hopOt === undefined) {
                        return false;
                    }
                    
                    if ( !(hopMe["<"](hopOt)) ) {
                        return false;
                    }
                }
                return true;
            },
            ">": function(other) {
                if (!(other instanceof Address)) {
                    throw new Error("Can compare Address only with Address");
                }
                
                for (var i = 0; i < this._data.length; ++i) {
                    hopMe = this._data[i];
                    hopOt = other._data[i];
                    
                    if (hopOt === undefined) {
                        return true;
                    }
                    
                    if ( !(hopMe[">"](hopOt)) ) {
                        return false
                    }
                }
                return true
            },
            "==": function(other) {
                if (!(other instanceof Address)) {
                    throw new Error("Can compare Address only with Address");
                }
                
                if (this._data.length !== other._data.length) {
                    return false;
                }
                for (var i = 0; i < this._data.length; ++i) {
                    hopMe = this._data[i];
                    hopOt = other._data[i];
                    if ( !(hopMe["=="](hopOt)) ) {
                        return false;
                    }
                }
                return true;
            },
            "+": function(other) {
                var res = this.clone();
                res["+="](other);
                return res;
            },
            "+=": function(other) {
                if (other instanceof Address) {
                    for (var i = 0; i < other._data.length; ++i) {
                        this._data.push(other._data[i].clone());
                    }
                } else {
                    throw new Error("Can add to Address only Address");
                }
                
                return this;
            },
            "clear": function() {
                for (var i = 0; i < this._data.length; ++i) {
                    this._data[i].destructor();
                }
                this._data = [];
            },
            "clone": function() {
                var clone = new Address();
                
                for (var i = 0; i < this._data.length; ++i) {
                    clone._data.push(this._data[i].clone());
                }
                
                return clone;
            },
            "deserialize": function(ba) {
                this.clear()
                var length = Object.pop32int(ba);
                
                for (var i = 0; i < length; ++i) {
                    var hop = new String();
                    hop.deserialize(ba);
                    this._data.push(hop);
                }
            },
            "serialize": function(ba) {
                Object.push32int(this._data.length, ba);
                
                for (var i = 0; i < this._data.length; ++i) {
                    this._data[i].serialize(ba);
                }
            },
            "size": function() {
                return this._data.length;
            },
            "toString": function() {
                var str = "";
                str += "["
                
                for (var i = 0; i < this._data.length; ++i) {
                    if (i !== 0) {
                        str +=", ";
                    }
                    str += this._data[i].toString();
                }
                
                str += "]";
                return str;
            },
            "_parseSource": function(data) {
                for (var i = 0; i < data.length; ++i) {
                    this._data.push(new String(data[i]));
                }
            }
        });
        
        return Address;
    });
})();