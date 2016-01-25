"use strict";
(function main_js() {
    requirejs.onError = function(e) {
        throw e;
    }
    
    var defineArray = [];
    defineArray.push("lib/wType/string");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wSocket/socket");
    defineArray.push("lib/wContainers/abstractmap")
    
    require(defineArray, function main_module() {
        var String = require("lib/wType/string");
        var Address = require("lib/wType/address");
        var AbstractMap = require("lib/wContainers/abstractmap");
        
        var Socket = require("lib/wSocket/socket");
        
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
    });
})();