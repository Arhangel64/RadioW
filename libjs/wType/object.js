"use strict";
var Class = require("../utils/class");
        
var Object = Class.inherit({
    "className": "Object",
    "constructor": function() {
        Class.fn.constructor.call(this);
    },
    "<": function(other) {
        throw new Error(this.className + " has no reimplemented method \"<\"");
    },
    ">": function(other) {
        throw new Error(this.className + " has no reimplemented method \">\"");
    },
    "==": function(other) {
        throw new Error(this.className + " has no reimplemented method \"==\"");
    },
    "clone": function() {
        throw new Error(this.className + " has no reimplemented method \"clone\"");
    },
    "getType": function() {
        var type = Object.objectType[this.className];
        
        if (type === undefined) {
            throw new Error("Undefined type of " + this.className);
        }
        
        return type;
    },
    "toString": function() {
        throw new Error(this.className + " has no reimplemented method \"toString\"");
    },
    "valueOf": function() {
        throw new Error(this.className + " has no reimplemented method \"valueOf\"");
    }
});

Object.objectType = {
    "String"    :   0,
    "Vocabulary":   1,
    "Uint64"    :   2,
    "Address"   :   3,
    "Boolean"   :   4,
    "Event"     :   5,
    "Vector"    :   6,
    "Blob"      :   7
};

Object.push32uint = function(int, ba) {
    var hh = (int >> 24) & 0xff;
    var hl = (int >> 16) & 0xff;
    var lh = (int >> 8) & 0xff;
    var ll = int & 0xff;
        
    ba.push(hh);
    ba.push(hl);
    ba.push(lh);
    ba.push(ll);
};

Object.pop32uint = function(ba) {
    var ret = ba.pop() << 24;
    ret = ret | (ba.pop() << 16);
    ret = ret | (ba.pop() << 8);
    ret = ret | ba.pop();
        
    return ret;
}

Object.push16uint = function(int, ba) {
    var h = (int >> 8) & 0xff;
    var l = int & 0xff;
    
    ba.push(h);
    ba.push(l);
}

Object.pop16uint = function(ba) {
    var ret = (ba.pop() << 8);
    ret = ret | ba.pop();
    
    return ret;
}

module.exports = Object;
