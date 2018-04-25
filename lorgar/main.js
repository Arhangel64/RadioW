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
                    
                    window.registerForeignController = window.lorgar.registerForeignController.bind(window.lorgar);
                    window.unregisterForeignController = window.lorgar.unregisterForeignController.bind(window.lorgar);
                    window.subscribeForeignController = window.lorgar.subscribeForeignController.bind(window.lorgar);
                    window.unsubscribeForeignController = window.lorgar.unsubscribeForeignController.bind(window.lorgar);
                }
            }
        }
        
        Controller.initialize(["String", "List", "Vocabulary", "Page", "PanesList", "Link", "Image"], waiter.check.bind(waiter, "controllers"));
        View.initialize(["Label", "Page", "PanesList", "Nav", "Image"], waiter.check.bind(waiter, "views"));
        
        var test = new Test();
        test.run();
        
    });
    
})();
