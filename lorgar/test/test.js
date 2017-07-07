"use strict";
(function test_js() {
    var moduleName = "test/test";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    defineArray.push("lib/wTest/abstractmap");
    defineArray.push("lib/wTest/abstractlist");
    defineArray.push("lib/wTest/abstractorder");
    
    define(moduleName, defineArray, function test_module() {
        var Class = require("lib/utils/class");
        var TAbstractMap = require("lib/wTest/abstractmap");
        var TAbstractList = require("lib/wTest/abstractlist");
        var TAbstractOrder = require("lib/wTest/abstractorder");
        
        var Test = Class.inherit({
            "className": "Test",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                this._s = 0;
                this._t = 0;
                this._tests = [];
                
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
                console.info("Starting tests");
                for (var i = 0; i < this._tests.length; ++i) {
                    this._tests[i].run();
                }
                console.info("Testing complete. " + this._s + "/" + this._t);
            },
            "_onStart": function(name) {
                console.info("Testing " + name);
            },
            "_onEnd": function(name, s, t) {
                console.info("Finished " + name + ". " + s + "/" + t);
                
                this._s += s;
                this._t += t;
            },
            "_onProgress": function(name, current, total) {
                
            },
            "_onFail": function(name, current, error) {
                console.warn("Test failed! Action " + current + ".");
                console.warn("Error message:" + error.message);
                console.warn("Error stack: \n" + error.stack);
            }
        });
        
        return Test;
    });
})();
