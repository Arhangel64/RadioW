"use strict";
(function view_js() {
    var moduleName = "views/view";
    
    var defineArray = [];
    defineArray.push("lib/utils/subscribable");
    defineArray.push("views/helpers/draggable");
    
    define(moduleName, defineArray, function view_module() {
        var counter = 0;
        var Subscribable = require("lib/utils/subscribable");
        var Draggable = require("views/helpers/draggable");
        
        var View = Subscribable.inherit({
            "className": "View",
            "constructor": function View (controller, options, element) {
                Subscribable.fn.constructor.call(this);
                this._destroying = false;
                
                var base = {
                    minWidth: 0,
                    minHeight: 0,
                    maxWidth: Infinity,
                    maxHeight: Infinity,
                    draggable: false
                };
                W.extend(base, options);
                
                this._id = ++counter;
                this._o = base;
                this._f = controller;
                if (element) {
                    this._e = element;
                } else {
                    this._e = document.createElement("div");
                }
                this._p = undefined;
                this._w = undefined;
                this._h = undefined;
                this._x = 0;
                this._y = 0;
                
                this._initElement();
                
                if (this._o.draggable) {
                    this._initDraggable();
                }
                
                this._f.on("data", this._onData, this);
                this._f.on("clearProperties", this._onClearProperties, this);
                this._f.on("addProperty", this._onAddProperty, this);
                this._f.on("newController", this._onNewController, this);
                
                for (var i = 0; i < this._f._controllers.length; ++i) {
                    this._onNewController(this._f._controllers[i]);
                }
                this._onData(this._f);
                
                View.collection[this._id] = this;
                this._applyProperties();
            },
            "destructor": function() {
                this._destroying = true;
                this._f.off("data", this._onData, this);
                this._f.off("clearProperties", this._onClearProperties, this);
                this._f.off("addProperty", this._onAddProperty, this);
                this._f.off("newController", this._onNewController, this);
                
                this.remove()
                if (this._o.draggable) {
                    this._dg.destructor();
                }
                
                delete View.collection[this._id];
                
                Subscribable.fn.destructor.call(this);
            },
            "addClass": function(className) {
                var arr = this._e.className.split(" ");
                if (arr.indexOf(className) === -1) {
                    arr.push(className);
                    this._e.className = arr.join(" ");
                }
            },
            "_applyProperties": function() {
                for (var i = 0; i < this._f.properties.length; ++i) {
                    var prop = this._f.properties[i];
                    this._onAddProperty(prop.k, prop.p);
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
            "_initDraggable": function() {
                this._dg = new Draggable(this, {
                    snapDistance: this._o.snapDistance
                });
            },
            "_initElement": function() {
                this._e.style.position = "absolute";
                this._e.style.top = "0";
                this._e.style.left = "0";
                this._e.style.boxSizing = "border-box";
                this._e.style.overflow = "hidden";
                this._e.id = this._id;
            },
            "_onAddProperty": function(key, propertyName) {
                var value = View.theme[key];
                if (value) {
                    this._e.style[propertyName] = value;
                }
            },
            "_onClearProperties": function() {
//                 for (var key in this._e.style) {
//                     if (this._e.style.hasOwnProperty(key)) {
//                         delete this._e.style[key];
//                     }
//                 }
                this._initElement();
                this._e.style.left = this._x + "px";
                this._e.style.top = this._y + "px";
                this._e.style.width = this._w + "px";
                this._e.style.height = this._h + "px";
            },
            "_onData": function() {},
            "_onNewController": function() {},
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
            "_resetTheme": function() {
                this._onClearProperties();
                this._applyProperties();
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
            "_setLimits": function(minWidth, minHeight, maxWidth, maxHeight) {
                var needToTell = false;
                if (this._o.minWidth !== minWidth) {
                    needToTell = true;
                    this._o.minWidth = minWidth;
                }
                if (this._o.maxWidth !== maxWidth) {
                    needToTell = true;
                    this._o.maxWidth = maxWidth;
                }
                if (this._o.minHeight !== minHeight) {
                    needToTell = true;
                    this._o.minHeight = minHeight;
                }
                if (this._o.maxHeight !== maxHeight) {
                    needToTell = true;
                    this._o.maxHeight = maxHeight;
                }
                if (needToTell) {
                    this.trigger("changeLimits", this);
                    if (this._w !== undefined && this._h !== undefined) {
                        this.setSize(this._w, this._h);
                    }
                }
                
                return needToTell && this._events.changeLimits && this._events.changeLimits.length;     //to see if someone actually going to listen that event, if not - return result
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
        
        View.theme = Object.create(null);
        View.collection = Object.create(null);
        View.setTheme = function(theme) {
            for (var key in this.theme) {
                delete this.theme[key];
            }
            for (var key in theme) {
                if (theme.hasOwnProperty(key)) {
                    this.theme[key] = theme[key];
                }
            }
            
            for (var id in this.collection) {
                this.collection[id]._resetTheme();
            }
        }
        
        View.createByType = function(type, ctrl, opts) {
            var typeName = this.ReversedViewType[type];
            if (typeName === undefined) {
                throw new Error("Unknown ViewType: " + type);
            }
            var Type = this.constructors[typeName];
            if (Type === undefined) {
                throw new Error("Constructor is not loaded yet, something is wrong");
            }
            return new Type(ctrl, opts);
        }

        View.initialize = function(rc, cb) {
            var deps = [];
            var types = [];
            for (var key in this.ViewTypesPaths) {
                if (this.ViewTypesPaths.hasOwnProperty(key)) {
                    if (!rc || rc.indexOf(key) !== -1) {
                        deps.push(this.ViewTypesPaths[key]);
                        types.push(key);
                    }
                }
            }
            require(deps, function() {
                for (var i = 0; i < types.length; ++i) {
                    View.constructors[types[i]] = arguments[i];
                }
                cb();
            });
        }

        View.ViewType = {
            Label:          0,
           
            Image:          4,
            Button:         5,
            View:           6,
            
            Page:           102,
            PanesList:      104
        };

        View.ReversedViewType = {
            "0":        "Label",
           
            "4":        "Image",
            "5":        "Button",
            "6":        "View",
            
            "101":      "Nav",
            "102":      "Page",
            "104":      "PanesList"
        };

        View.ViewTypesPaths = {
            Label:          "views/label",
            Nav:            "views/nav",
            Page:           "views/page",
            PanesList:      "views/panesList",
            Image:          "views/image",
            Button:         "views/button"
        };

        View.constructors = {
            View: View
        };
        
        
        return View;
    });
})();
