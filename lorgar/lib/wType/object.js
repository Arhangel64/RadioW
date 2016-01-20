"use strict";
(function object_js() {
    var moduleName = "lib/wType/object";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    
    define(moduleName, defineArray, function object_module() {
        var Class = require("lib/utils/class");
        
        var Object = Class.inherit({
            "className": "Object",
            "constructor": function() {
                
            },
            "getType": function() {
                var type = Object.objectType[this.className];
                
                if (type === undefined) {
                    throw new Error("Undefined type of object");
                }
                
                return type;
            }
        });
        
        Object.objectType = {
            "String"    :   0,
            "Vocabulary":   1,
            "Uint64"    :   2,
            "Address"   :   3,
            "Boolean"   :   4,
            "Event"     :   5
        }
        
        return Object;
    });
})();