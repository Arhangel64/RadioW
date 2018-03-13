"use strict";
(function() {
    var moduleName = "views/image";
    
    var deps = [];
    deps.push("views/view");
    
    define(moduleName, deps, function() {
        var View = require("views/view");
        
        var Image = View.inherit({
            "className": "Image",
            "constructor": function(controller, options) {
                var base = {};
                W.extend(base, options)
                var el = document.createElement("img");
                View.fn.constructor.call(this, controller, base, el);
                
                if (!controller.hasData()) {
                    controller.requestData();
                }
            },
            "_onData": function() {
                if (this._f.hasData()) {
                    this._e.src = "data:image/jpeg;base64," + this._f.data.base64();
                }
            }
        });
        
        return Image;
    })
})();
