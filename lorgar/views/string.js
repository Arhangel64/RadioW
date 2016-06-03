"use strict";
(function view_string_js() {
    var moduleName = "views/string";
    
    var defineArray = [];
    defineArray.push("views/view");
    
    define(moduleName, defineArray, function view_string_module() {
        var View = require("views/view");
        
        var ViewString = View.inherit({
            "className": "String",
            "constructor": function(options) {
                var base = {
                    maxHeight: 20
                };
                W.extend(base, options)
                
                View.fn.constructor.call(this, base);
            },
            "data": function(data) {
                this._e.innerText = data;
            }
        });
        
        return ViewString;
    });
})();