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
        var View = require("views/view");
        
        Controller.initialize(["String", "List", "Vocabulary", "Page"]);
        View.initialize(["Label", "Page"]);
        
        var test = new Test();
        test.run();
        
        window.lorgar = new Lorgar();
    });
})();
