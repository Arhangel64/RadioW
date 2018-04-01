"use strict";

var Object = require("./object");
var types = {
    "String"    :   require("./string"),
    "Vocabulary":   require("./vocabulary"),
    "Uint64"    :   require("./uint64"),
    "Address"   :   require("./address"),
    "Boolean"   :   require("./boolean"),
    "Event"     :   require("./event"),
    "Vector"    :   require("./vector"),
    "Blob"      :   require("./blob")
}

var storage = global.Object.create(null);

for (var name in types) {
    if (types.hasOwnProperty(name)) {
        var typeId = Object.objectType[name];
        if (typeId === undefined) {
            throw new Error("wType initialization error - can't find type id for type " + name);
        }
        storage[typeId] = types[name];
    }
}

function create(/*ByteArray*/ba) {
    var type = ba.pop8();
    var Type = storage[type];
    
    if (Type === undefined) {
        throw new Error("Unsupported data type found during deserialization: " + type);
    }
    
    var obj = new Type();
    obj.deserialize(ba);
    
    return obj;
}

Object.fromByteArray = create;
module.exports = create;
