"use strict";
(function test_js() {
    var moduleName = "test/test";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    defineArray.push("lib/wType/string");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wContainer/abstractmap")
    
    define(moduleName, defineArray, function test_module() {
        var Class = require("lib/utils/class");
        var String = require("lib/wType/string");
        var Address = require("lib/wType/address");
        var AbstractMap = require("lib/wContainer/abstractmap");
        
        var Test = Class.inherit({
            "className": "Test",
            "constructor": function() {
                Class.fn.constructor.call(this);
            },
            "testMap": function() {
                var Map = AbstractMap.template(Address, String);
                var map = window.map = new Map();
                
                var noEl = map.find(new Address(["addr1", "hop1"]));
                if (noEl !== map.end()) {
                    console.log("error: problem with end!");
                }
                
                map.insert(new Address(["addr1", "hop1"]), new String("hello"));
                map.insert(new Address(["addr2", "hop2"]), new String("world"));
                
                if (map.size() !== 2) {
                    console.log("error: problem with insertion!");
                }
                
                var itr = map.find(new Address(["addr1", "hop1"]));
                
                if (itr === map.end()) {
                    console.log("error: problem with finding!");
                }
                
                console.log(itr.data().second.toString());
                itr.next();
                console.log(itr.data().second.toString());
                
                map.erase(itr);
                
                itr = map.find(new Address(["addr2", "hop2"]));
                
                if (itr !== map.end()) {
                    console.log("error: problem with erasing!");
                }
                
                map.clear();
                
                if (map.size() > 0) {
                    console.log("error: problem with clearing!");
                }
                map.destructor();
            }
        });
        
        return Test;
    });
})();