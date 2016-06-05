"use strict";
(function navigationPanel_js() {
    var moduleName = "models/navigationPanel";
    
    var defineArray = [];
    defineArray.push("models/list");
    
    define(moduleName, defineArray, function navigationPanel_module() {
        var List = require("models/list");
        
        var NavigationPanel = List.inherit({
            "className": "NavigationPanel",
            "constructor": function(addr) {
                List.fn.constructor.call(this, addr);
            },
            "_h_get": function(ev) {
                List.fn._h_get.call(this, ev);
                
                
            },
            "_h_push": function(ev) {
                List.fn._h_push.call(this, ev);
                
                
            }
        });
        
        return NavigationPanel;
    });
})();