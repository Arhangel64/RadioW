"use strict";
(function main_js() {
    requirejs.onError = function(e) {
        throw e;
    }
    
    var defineArray = [];
    
    defineArray.push("lib/utils/subscribable");
    defineArray.push("lib/wType/string");
    defineArray.push("lib/wType/bytearray");
    
    require(defineArray, function main_module() {
        var Subscribable = require("lib/utils/subscribable");
        var String = require("lib/wType/string");
        var ByteArray = require("lib/wType/bytearray");
        
        var Some = Subscribable.inherit({});
        
        var some = new Some();
        some.on("hey", function() {
            var str = new String("hello world!");
            var ba = new ByteArray();
            
            str.serialize(ba);
            
            var str2 = new String();
            str2.deserialize(ba);
            
            console.log(str2.toString());
        });
        some.trigger("hey");
    });
})();