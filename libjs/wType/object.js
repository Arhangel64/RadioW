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
    "length": function() {
        throw new Error(this.className + " has no reimplemented method \"length\"");
    },
    "size": function() {
        throw new Error(this.className + " has no reimplemented method \"size\"");
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

Object.reverseObjectType = {
    0   :   "String",
    1   :   "Vocabulary",
    2   :   "Uint64",
    3   :   "Address",
    4   :   "Boolean",
    5   :   "Event",
    6   :   "Vector",
    7   :   "Blob"
}

Object.fromByteArray = function() {
    throw new Error("Initialization error. Object.fromByteArray is not implemented, it implements in factory.js");
}

module.exports = Object;
