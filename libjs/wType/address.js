"use strict";
var Object = require("./object");
var String = require("./string");

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
        var hopMe;
        var hopOt;
        
        for (var i = 0; i < this._data.length; ++i) {
            hopMe = this._data[i];
            hopOt = other._data[i];
            
            if (hopOt === undefined) {
                return false;
            }
            
            if (hopMe["<"](hopOt)) {
                return true;
            }
            if (hopMe[">"](hopOt)) {
                return false;
            }
        }
        return this._data.length < other._data.length;
    },
    ">": function(other) {
        if (!(other instanceof Address)) {
            throw new Error("Can compare Address only with Address");
        }
        var hopMe;
        var hopOt;
        
        for (var i = 0; i < this._data.length; ++i) {
            hopMe = this._data[i];
            hopOt = other._data[i];
            
            if (hopOt === undefined) {
                return true;
            }
            
            if (hopMe[">"](hopOt)) {
                return true;
            }
            if (hopMe["<"](hopOt)) {
                return false;
            }
        }
        return this._data.length > other._data.length;
    },
    "==": function(other) {
        if (this.getType() !== other.getType()) {
            return false;
        }
        
        if (this._data.length !== other._data.length) {
            return false;
        }
        
        var hopMe;
        var hopOt;
        
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
    "<<": function(n) {
        var res = new Address();
        for (var i = n; i < this._data.length; ++i) {
            res._data.push(this._data[i].clone());
        }
        return res;
    },
    ">>": function(n) {
        var res = new Address();
        for (var i = 0; i < this._data.length - n; ++i) {
            res._data.push(this._data[i].clone());
        }
        return res;
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
        var length = ba.pop32();
        
        for (var i = 0; i < length; ++i) {
            var hop = new String();
            hop.deserialize(ba);
            this._data.push(hop);
        }
    },
    "serialize": function(ba) {
        ba.push32(this._data.length)
        
        for (var i = 0; i < this._data.length; ++i) {
            this._data[i].serialize(ba);
        }
    },
    "length": function() {
        return this._data.length;
    },
    "size": function() {
        var size = 4;
        for (var i = 0; i < this._data.length; ++i) {
            size += this._data[i].size();
        }
        
        return size;
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
    "begins": function(other) {
        var size = other._data.length;
        if (size > this._data.length) {
            return false;
        }
        
        for (var i = 0; i < size; ++i) {
            var myHop = this._data[i];
            var othHop = other._data[i];
            
            if (!myHop["=="](othHop)) {
                return false;
            }
        }
        
        return true;
    },
    "ends": function(other) {
        var size = other._data.length;
        if (size > this._data.length) {
            return false;
        }
        
        for (var i = 1; i <= size; ++i) {
            var myHop = this._data[this._data.length - i];
            var othHop = other._data[other._data.length - i];
            
            if (!myHop["=="](othHop)) {
                return false;
            }
        }
        
        return true;
    },
    "back": function() {
        return this._data[this._data.length - 1].clone();
    }, 
    "front": function() {
        return this._data[0].clone();
    },
    "toArray": function() {
        var arr = [];
        for (var i = 0; i < this._data.length; ++i) {
            arr.push(this._data[i].toString());
        }
        return arr;
    },
    "_parseSource": function(data) {
        for (var i = 0; i < data.length; ++i) {
            this._data.push(new String(data[i]));
        }
    }
});

module.exports = Address;
