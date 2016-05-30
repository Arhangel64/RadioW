"use strict";
(function layout_js() {
    var moduleName = "views/layout";
    
    var defineArray = [];
    defineArray.push("views/view");
    
    define(moduleName, defineArray, function layout_module() {
        var View = require("views/view");
        
        var Layout = View.inherit({
            "className": "Layout",
            "constructor": function(options, element) {
                var base = {};
                W.extend(base, options);
                View.fn.constructor.call(this, base, element);
                
                this._c = [];
            },
            "destructor": function() {
                var c = this._c.slice();
                for (var i = 0; i < c.length; ++i) {
                    c[i].destructor();
                }
                
                View.fn.destructor.call(this);
            },
            "append": function(child) {
                this._c.push(child);
                this._e.appendChild(child._e);
                child._p = this;
                if (this._w !== undefined && this._h !== undefined) {
                    child.setSize(this._w, this._h);
                }
            },
            "removeChild": function(child) {
                this._e.removeChild(child._e);
                this._c.splice(this._c.indexOf(child), 1);
                child._p = undefined;
            },
            "setSize": function(w, h) {
                View.fn.setSize.call(this, w, h);
                
                for (var i = 0; i < this._c.length; ++i) {
                    this._c[i].setSize(this._w, this._h);
                }
            }
        });
        
        return Layout;
    });
})();