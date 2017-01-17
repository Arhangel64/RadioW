"use strict";
(function main_js() {
    requirejs.onError = function(e) {
        throw e;
    }
    
    var defineArray = [];
    defineArray.push("test/test");
    defineArray.push("core/lorgar");
    defineArray.push("lib/utils/globalMethods");
    
    require(defineArray, function main_module() {
        require("lib/utils/globalMethods");
        
        var Test = require("test/test");
        var Lorgar = require("core/lorgar");
        var Controller = require("lib/wController/controller");
        
        Controller.initialize(["String", "List", "Vocabulary"]);
        
        var test = new Test();
        test.run();
        
        window.lorgar = new Lorgar();
    });
})();
