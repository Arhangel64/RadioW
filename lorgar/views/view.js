"use strict";
(function view_js() {
    var moduleName = "views/view";
    
    var defineArray = [];
    defineArray.push("lib/utils/subscribable");
    
    define(moduleName, defineArray, function view_module() {
        var Subscribable = require("lib/utils/subscribable");
        
        var View = Subscribable.inherit({
            "className": "View",
            "constructor": function(options, element) {
                Subscribable.fn.constructor.call(this);
                
                var base = {};
                W.extend(base, options);
                
                this._o = base;
                if (element) {
                    this._e = element;
                } else {
                    this._e = document.createElement("div");
                }
                this._c = [];
                this._p = undefined;
            },
            "destructor": function() {
                this.remove()
                
                var c = this._c.slice();
                for (var i = 0; i < c.length; ++i) {
                    c[i].destructor();
                }
                
                Subscribable.fn.destructor.call(this);
            },
            "append": function(child) {
                this._c.push(child);
                this._e.appendChild(child._e);
                child._p = this;
            },
            "data": function(data) {
                
            },
            "remove": function() {
                if (this._p) {
                    this._p.removeChild(this);
                }
            },
            "removeChild": function(child) {
                this._e.removeChild(child._e);
                this._c.splice(this._c.indexOf(child), 1);
                child._p = undefined;
            }
        });
        
        return View;
    });
})();