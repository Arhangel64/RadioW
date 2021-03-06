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
                
                controller.needData();
            },
            "destructor": function() {
                this._f.dontNeedData();
                
                View.fn.destructor.call(this);
            },
            "_onData": function() {
                if (this._f.hasData()) {
                    this._e.src = "data:" + this._f.getMimeType() + ";base64," + this._f.data.base64();
                }
            }
        });
        
        return Image;
    })
})();
