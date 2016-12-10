"use strict";
var WTest = require("./test");
var Uint64 = require("../wType/uint64");

var TUint64 = WTest.inherit({
    "className": "TUint64",
    "constructor": function() {
        WTest.fn.constructor.call(this, "Uint64");
        
        this._actions.push(this.testEq);
        this._actions.push(this.testGt);
        this._actions.push(this.testLt);
    },
    "testEq": function() {
        var first = new Uint64(5);
        var second = new Uint64(5);
        
        if (!first["=="](second)) {
            throw new Error("problem with equals low");
        }
    },
    "testGt": function() {
        var first = new Uint64(5);
        var second = new Uint64(4);
        
        if (!first[">"](second)) {
            throw new Error("problem with greater low");
        }
    },
    "testLt": function() {
        var first = new Uint64(4);
        var second = new Uint64(5);
        
        if (!first["<"](second)) {
            throw new Error("problem with lower low");
        }
    }
});

module.exports = TUint64;
