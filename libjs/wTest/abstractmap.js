"use strict";
var WTest = require("./test");
var AbstractMap = require("../wContainer/abstractmap");

var String = require("../wType/string");
var Address = require("../wType/address");

var TAbsctractMap = WTest.inherit({
    "className": "TAbsctractMap",
    "constructor": function() {
        WTest.fn.constructor.call(this, "AbstractMap");
        
        var Map = AbstractMap.template(Address, String);
        this._map = new Map();
        
        this._actions.push(this.testEnd);
        this._actions.push(this.testSize);
        this._actions.push(this.testIterators);
        this._actions.push(this.testClear);
    },
    "destructor": function() {
        this._map.destructor();
        
        WTest.fn.destructor.call(this);
    },
    "testEnd": function() {
        var noEl = this._map.find(new Address(["addr1", "hop1"]));
        if (!noEl["=="](this._map.end())) {
            throw new Error("problem with end!");
        }
    },
    "testSize": function() {
        this._map.insert(new Address(["addr1", "hop1"]), new String("hello"));
        this._map.insert(new Address(["addr2", "hop2"]), new String("world"));
        
        if (this._map.size() !== 2) {
            throw new Error("problem with insertion!");
        }
    },
    "testIterators": function() {
        var itr = this._map.find(new Address(["addr1", "hop1"]));
        
        if (itr["=="](this._map.end())) {
            throw new Error("problem with finding!");
        }
        
        if (itr["*"]().second.toString() !== "hello") {
            throw new Error("wrong element found");
        }
        itr["++"]();
        if (itr["*"]().second.toString() !== "world") {
            throw new Error("iterator dereferenced into wrong element after incrementetion");
        }
        
        this._map.erase(itr);
        itr = this._map.find(new Address(["addr2", "hop2"]));
        
        if (!itr["=="](this._map.end())) {
            throw new Error("problem with erasing!");
        }
    },
    "testClear": function() {
        this._map.clear();
        
        if (this._map.size() > 0) {
            throw new Error("problem with clearing!");
        }
    }
});

module.exports = TAbsctractMap;