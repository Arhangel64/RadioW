"use strict";
(function map_js() {
    var moduleName = "lib/wContainers/map";
    
    var defineArray = [];
    defineArray.push("lib/js_bintrees/rbtree");
    
    define(moduleName, defineArray,function map_module() {
        var RBTree = require("lib/js_bintrees/rbtree");
        var Map = RBTree = inherit({
            "className": "Map",
            "constructor": function() {
                
            }
        });
        
        return Map;
    });
})();