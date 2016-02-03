"use strict";
(function main_js() {
    requirejs.onError = function(e) {
        throw e;
    }
    
    var defineArray = [];
    defineArray.push("test/test");
    defineArray.push("core/lorgar");
    
    require(defineArray, function main_module() {
        var Test = require("test/test");
        var Lorgar = require("core/lorgar");
        
        var test = new Test();
        
        test.testMap();
        test.testList();
        test.testOrder();
        
        window.lorgar = new Lorgar();
    });
})();