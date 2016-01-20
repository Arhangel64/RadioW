"use strict";
(function main_js() {
    requirejs.onError = function(e) {
        throw e;
    }
    
    var defineArray = [];
    
    defineArray.push("lib/utils/subscribable");
    defineArray.push("lib/wType/string");
    defineArray.push("lib/wType/uint64");
    defineArray.push("lib/wType/bytearray");
    defineArray.push("lib/wType/vocabulary");
    
    require(defineArray, function main_module() {
        var Subscribable = require("lib/utils/subscribable");
        var String = require("lib/wType/string");
        var Uint64 = require("lib/wType/uint64");
        var ByteArray = require("lib/wType/bytearray");
        var Vocabulary = require("lib/wType/vocabulary");
        
        var Some = Subscribable.inherit({});
        
        var some = new Some();
        some.on("hey", function() {
            var str = new String("hello world!");
            var int = new Uint64(64);
            var vc1 = new Vocabulary();
            var vc2 = new Vocabulary();
            
            vc1.insert("vc1", vc2);
            vc2.insert(str, int);
            
            var ba = new ByteArray();
            
            ba["<<"](str);
            ba["<<"](int);
            ba["<<"](vc1);
            
            var str2 = ba[">>"]();
            var int2 = ba[">>"]();
            var vcd = ba[">>"]();
            
            console.log(str2);
            console.log(int2);
            console.log(vcd.at("vc1").at(str2).toString());
        });
        some.trigger("hey");
    });
})();