"use strict";
(function panesList_js() {
    var moduleName = "views/panesList";
    
    var defineArray = [];
    defineArray.push("views/layout");
    
    define(moduleName, defineArray, function panesList_module() {
        var Layout = require("views/layout");
        
        var PanesList = Layout.inherit({
            "className": "PanesList",
            "constructor": function PanesListView(options) {
                var base = {
                    
                };
                W.extend(base, options);
                Layout.fn.constructor.call(this, base);
            },
            "destructor": function() {
                Layout.fn.destructor.call(this);
            }
        });
        
        return PanesList;
    });
})();