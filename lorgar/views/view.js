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
                
                var base = {
                    minWidth: 0,
                    minHeight: 0,
                    maxWidth: Infinity,
                    maxHeight: Infinity
                };
                W.extend(base, options);
                
                this._o = base;
                if (element) {
                    this._e = element;
                } else {
                    this._e = document.createElement("div");
                }
                this._p = undefined;
                this._w = undefined;
                this._h = undefined;
                
                this._initElement();
            },
            "destructor": function() {
                this.remove()
                
                Subscribable.fn.destructor.call(this);
            },
            "addClass": function(className) {
                var arr = this._e.className.split(" ");
                if (arr.indexOf(className) === -1) {
                    arr.push(className);
                    this._e.className = arr.join(" ");
                }
            },
            "constrainHeight": function(h) {
                h = Math.max(h, this._o.minHeight);
                h = Math.min(h, this._o.maxHeight);
                
                return h;
            },
            "constrainWidth": function(w) {
                w = Math.max(w, this._o.minWidth);
                w = Math.min(w, this._o.maxWidth);
                
                return w;
            },
            "data": function(data) {
                
            },
            "_initElement": function() {
                this._e.style.position = "absolute";
                this._e.style.top = "0";
                this._e.style.left = "0";
                this._e.style.boxSizing = "border-box";
            },
            "remove": function() {
                if (this._p) {
                    this._p.removeChild(this);
                }
            },
            "removeClass": function(className) {
                var arr = this._e.className.split(" ");
                var index = arr.indexOf(className)
                var toJoin = false;
                while (index !== -1) {
                    arr.splice(index, 1);
                    index = arr.indexOf(className)
                    toJoin = true;
                }
                if (toJoin) {
                    this._e.className = arr.join(" ");
                }
            },
            "setSize": function(w, h) {
                this._w = this.constrainWidth(w);
                this._h = this.constrainHeight(h);
                
                this._e.style.width = this._w + "px";
                this._e.style.height = this._h + "px";
            },
            "trySize": function(w, h) {
                return !(w < this._o.minWidth || h < this._o.minHeight || w > this._o.maxWidth || h > this._o.maxHeight)
            }
        });
        
        return View;
    });
})();