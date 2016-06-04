"use strict";
(function page_js() {
    var moduleName = "views/page";
    
    var defineArray = [];
    defineArray.push("views/gridLayout");
    
    define(moduleName, defineArray, function page_module() {
        var GridLayout = require("views/gridLayout");
        
        var Page = GridLayout.inherit({
            "className": "Page",
            "constructor": function(options) {
                var base = {
                    
                };
                W.extend(base, options)
                GridLayout.fn.constructor.call(this, base);
            }
        });
        
        return Page;
    });
})();