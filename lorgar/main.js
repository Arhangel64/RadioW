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
        
        var waiter = {
            views: false,
            controllers: false,
            check: function(key) {
                this[key] = true;
                this.launch()
            },
            launch: function() {
                if (this.views && this.controllers) {
                    window.lorgar = new Lorgar();
                }
            }
        }
        
        Controller.initialize(["String", "List", "Vocabulary", "Page", "PanesList"], waiter.check.bind(waiter, "controllers"));
        View.initialize(["Label", "Page", "PanesList"], waiter.check.bind(waiter, "views"));
        
        var test = new Test();
        test.run();
        
    });
})();
