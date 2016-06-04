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
            }
        });
        
        return NavigationPanel;
    });
})();