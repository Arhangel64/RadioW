"use strict";
var Class = require("../lib/utils/class");
var TUint64 = require("../lib/wTest/uint64");
var TAbstractMap = require("../lib/wTest/abstractmap");
var TAbstractList = require("../lib/wTest/abstractlist");
var TAbstractOrder = require("../lib/wTest/abstractorder");
var log = require("../lib/log")(module);

var Test = Class.inherit({
    "className": "Test",
    "constructor": function() {
        Class.fn.constructor.call(this);
        
        this._s = 0;
        this._t = 0;
        this._tests = [];
        
        this.addTest(new TUint64());
        this.addTest(new TAbstractList());
        this.addTest(new TAbstractMap());
        this.addTest(new TAbstractOrder());
    },
    "destructor": function() {
        for (var i = 0; i < this._tests.length; ++i) {
            this._tests[i].destructor();
        }
        
        Class.fn.destructor.call(this);
    },
    "addTest": function(test) {
        test.on("start", this._onStart, this);
        test.on("end", this._onEnd, this);
        test.on("progress", this._onProgress, this);
        test.on("fail", this._onFail, this);
        
        this._tests.push(test);
    },
    "run": function() {
        log.info("Starting tests");
        for (var i = 0; i < this._tests.length; ++i) {
            this._tests[i].run();
        }
        log.info("Testing complete. " + this._s + "/" + this._t);
    },
    "_onStart": function(name) {
        log.info("Testing " + name);
    },
    "_onEnd": function(name, s, t) {
        log.info("Finished " + name + ". " + s + "/" + t);
        
        this._s += s;
        this._t += t;
    },
    "_onProgress": function(name, current, total) {
        
    },
    "_onFail": function(name, current, error) {
        log.warn("Test failed! Action " + current + ".");
        log.warn("Error message:" + error.message);
        log.warn("Error stack: \n" + error.stack);
    }
});

module.exports = Test;
