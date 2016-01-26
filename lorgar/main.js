"use strict";
(function main_js() {
    requirejs.onError = function(e) {
        throw e;
    }
    
    var defineArray = [];
    defineArray.push("test/test");
    
    require(defineArray, function main_module() {
        var Test = require("test/test");
        
        (new Test()).testMap();
    });
})();