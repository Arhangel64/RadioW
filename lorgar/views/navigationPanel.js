"use strict";
(function navigationPanel_js() {
    var moduleName = "views/navigationPanel";
    
    var defineArray = [];
    defineArray.push("views/gridLayout");
    
    define(moduleName, defineArray, function navigationPanel_module() {
        var GridLayout = require("views/gridLayout");
        
        var NavigationPanel = GridLayout.inherit({
            "className": "NavigationPanel",
            "constructor": function(options) {
                var base = {
                    minHeight: 100,
                    maxHeight: 100
                };
                W.extend(base, options)
                GridLayout.fn.constructor.call(this, base);
                
                this._e.className += "primary";
            }
        });
        
        return NavigationPanel;
    });
})();