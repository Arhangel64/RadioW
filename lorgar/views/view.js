"use strict";
(function view_js() {
    var moduleName = "views/view";
    
    var defineArray = [];
    defineArray.push("lib/utils/subscribable");
    defineArray.push("views/helpers/draggable");
    
    define(moduleName, defineArray, function view_module() {
        var Subscribable = require("lib/utils/subscribable");
        var Draggable = require("views/helpers/draggable");
        
        var View = Subscribable.inherit({
            "className": "View",
            "constructor": function(options, element) {
                Subscribable.fn.constructor.call(this);
                
                var base = {
                    minWidth: 0,
                    minHeight: 0,
                    maxWidth: Infinity,
                    maxHeight: Infinity,
                    draggable: false
                };
                W.extend(base, options);
                
                this._o = base;
                if (element) {
                    this._e = element;
                } else {
                    this._e = document.createElement("div");
                }
                this._styleProperties = [];
                this._p = undefined;
                this._w = undefined;
                this._h = undefined;
                this._currentTheme = {};
                this._x = 0;
                this._y = 0;
                
                this._initElement();
                if (this._o.draggable) {
                    this._initDraggable();
                }
            },
            "destructor": function() {
                this.remove()
                if (this._o.draggable) {
                    this._dg.destructor();
                }
                
                Subscribable.fn.destructor.call(this);
            },
            "addClass": function(className) {
                var arr = this._e.className.split(" ");
                if (arr.indexOf(className) === -1) {
                    arr.push(className);
                    this._e.className = arr.join(" ");
                }
            },
            "addProperty": function(pair) {
                this._styleProperties.push(pair);
                this._applyProp(pair);
            },
            "_applyProp": function(pair) {
                var value = this._currentTheme[pair.k];
                if (value) {
                    this._e.style[pair.p] = value;
                }
            },
            "applyTheme": function(theme) {
                this._currentTheme = theme;
                for (var i = 0; i < this._styleProperties.length; ++i) {
                    this._applyProp(this._styleProperties[i]);
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
            "_initDraggable": function() {
                this._dg = new Draggable(this, {});
            },
            "_initElement": function() {
                this._e.style.position = "absolute";
                this._e.style.top = "0";
                this._e.style.left = "0";
                this._e.style.boxSizing = "border-box";
                this._e.style.overflow = "hidden";
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
            "removeProperty": function(pair) {
                var index = this._styleProperties.indexOf(pair);
                if (index !== -1) {
                    this._styleProperties.splice(index, 1);
                    this._e.style[pair.p] = "";
                    this.applyTheme(this._currentTheme);
                }
                
            },
            "setConstSize": function(w, h) {
                this._o.maxWidth = w;
                this._o.maxHeight = h;
                this._o.minWidth = w;
                this._o.minHeight = h;
                
                if (this._w !== undefined && this._h !== undefined) {
                    this.setSize(this._w, this._h);
                }
                
                this.trigger("changeLimits", this);
            },
            "setLeft": function(l) {
                this._x = l;
                this._e.style.left = this._x + "px";
            },
            "setMaxSize": function(w, h) {
                this._o.maxWidth = w;
                this._o.maxHeight = h;
                
                if (this._w !== undefined && this._h !== undefined) {
                    this.setSize(this._w, this._h);
                }
                
                this.trigger("changeLimits", this);
            },
            "setMinSize": function(w, h) {
                this._o.minWidth = w;
                this._o.minHeight = h;
                
                if (this._w !== undefined && this._h !== undefined) {
                    this.setSize(this._w, this._h);
                }
                
                this.trigger("changeLimits", this);
            },
            "setSize": function(w, h) {
                this._w = this.constrainWidth(w);
                this._h = this.constrainHeight(h);
                
                this._e.style.width = this._w + "px";
                this._e.style.height = this._h + "px";
                
                this.trigger("resize", this._w, this._h);
            },
            "setTop": function(t) {
                this._y = t;
                this._e.style.top = this._y + "px";
            },
            "trySize": function(w, h) {
                return !(w < this._o.minWidth || h < this._o.minHeight || w > this._o.maxWidth || h > this._o.maxHeight)
            }
        });
        
        return View;
    });
})();