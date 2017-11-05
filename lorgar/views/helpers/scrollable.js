"use strict";
(function scrollable_js() {
    var moduleName = "views/helpers/scrollable";
    
    var defineArray = [];
    defineArray.push("views/view");
    defineArray.push("lib/wController/localModel");
    
    define(moduleName, defineArray, function scrollable_module() {
        var View = require("views/view");
        var LocalModel = require("lib/wController/localModel");
        
        var Scrollable = View.inherit({
            "className": "Scrollable",
            "constructor": function Scrollable(options, layout) {
                var base = {
                    draggable: true,
                    snapDistance: 5,
                    vertical: false,
                    horizontal: false,
                    virtual: false
                };
                var lm = new LocalModel();
                W.extend(base, options);
                View.fn.constructor.call(this, lm, base);
                
                this._l = layout;
                this._l._e.appendChild(this._e);
                this._horizontalBar = document.createElement("div");
                this._verticalBar = document.createElement("div");
                this._horizontalBarShown = false;
                this._verticalBarShown = false;
                
                this._initProxy();
                this._initBars();
                
                this._e.addEventListener("mousewheel", this._proxy.onMouseWheel, false);
                
                if (this._o.draggable) {
                    this.on("dragStart", this._onDragStart, this);
                    this.on("drag", this._onDrag, this);
                    this.on("dragEnd", this._onDragEnd, this);
                }
                
                this._uncyclic.push(function() {
                    lm.destructor();
                })
            },
            "destructor": function() {
                this._e.removeEventListener("mousewheel", this._proxy.onMouseWheel, false);
                this._l._e.removeChild(this._e);
                if (this._horizontalBarShown) {
                    this._l._e.removeChild(this._horizontalBar);
                }
                if (this._verticalBarShown) {
                    this._l._e.removeChild(this._verticalBar);
                }
                
                View.fn.destructor.call(this);
            },
            "appendChild": function(e) {
                this._e.appendChild(e);
            },
            "_initBars": function() {
                this._horizontalBar.style.position = "absolute";
                this._horizontalBar.style.left = "0";
                this._horizontalBar.style.bottom = "0";
                this._horizontalBar.style.height = "10px";
                this._horizontalBar.style.width = "0";
                this._horizontalBar.style.zIndex = "2";
                this._horizontalBar.style.backgroundColor = Scrollable.theme.primaryColor || "#ff0000";
                
                this._verticalBar.style.position = "absolute";
                this._verticalBar.style.right = "0";
                this._verticalBar.style.top = "0";
                this._verticalBar.style.height = "0";
                this._verticalBar.style.width = "10px";
                this._verticalBar.style.zIndex = "2";
                this._verticalBar.style.backgroundColor = Scrollable.theme.primaryColor || "#ff0000";
            },
            "_initProxy": function() {
                this._proxy = {
                    onMouseWheel: this._onMouseWheel.bind(this)
                };
            },
            "maximizeMinSize": function(child) {
                var width = Math.max(this._o.minWidth, child._o.minWidth);
                var height = Math.max(this._o.minHeight, child._o.minHeight);
                
                this.setMinSize(width, height);
            },
            "_onDrag": function(pos) {
                var oX = this._o.horizontal ? pos.x : 0;
                var oY = this._o.vertical ? pos.y : 0;
                
                var aX = this._sX + oX;
                var aY = this._sY + oY;
                
                var cX = Math.max(Math.min(0, aX), this._l._w - this._w);
                var cY = Math.max(Math.min(0, aY), this._l._h - this._h);
                
                this.setTop(cY);
                this.setLeft(cX);
            },
            "_onDragEnd": function(pos) {
                //console.log("end")
            },
            "_onDragStart": function(pos) {
                this._sX = this._x;
                this._sY = this._y;
                //console.log("start");
            },
            "_onMouseWheel": function(e) {
                var dX = this._o.horizontal ? e.deltaX : 0;
                var dY = this._o.vertical ? e.deltaY : 0;
                
                var aX = this._x + dX;
                var aY = this._y - dY;
                
                var cX = Math.max(Math.min(0, aX), this._l._w - this._w);
                var cY = Math.max(Math.min(0, aY), this._l._h - this._h);
                
                this.setTop(cY);
                this.setLeft(cX);
            },
            "removeChild": function(e) {
                this._e.removeChild(e);
            },
            "_resetTheme": function() {
                View.fn._resetTheme.call(this);
                
                this._horizontalBar.style.backgroundColor = Scrollable.theme.primaryColor || "#ff0000";
                this._verticalBar.style.backgroundColor = Scrollable.theme.primaryColor || "#ff0000";
            },
            "setLeft": function(x) {
                if (this._x !== x) {
                    if (this._o.virtual) {
                        this._x = x;
                        this._e.style.left = "0px";
                    } else {
                        View.fn.setLeft.call(this, x);
                    }
                    if (x === 0) {
                        this.trigger("scrollLeft", 0);
                    } else {
                        this.trigger("scrollLeft", -x);
                    }
                    
                    this._horizontalBar.style.top = this._x / this._w * this._l._w + "px";
                }
            },
            "setSize": function(w, h) {
                if (this._o.virtual) {
                    this._w = this.constrainWidth(w);
                    this._h = this.constrainHeight(h);
                    
                    this._e.style.width = w + "px";
                    this._e.style.height = h + "px";
                    
                    this.trigger("resize", this._w, this._h);
                } else {
                    View.fn.setSize.call(this, w, h);
                }
                
                if (this._w > this._l._w) {
                    var width = this._l._w / this._w * this._l._w;
                    var wOffset = this._x / this._w * this._l._w;
                    
                    this._horizontalBar.style.width = width + "px";
                    this._horizontalBar.style.left = wOffset + "px";
                    
                    if (!this._horizontalBarShown) {
                        this._l._e.appendChild(this._horizontalBar);
                        this._horizontalBarShown = true;
                    }
                } else if (this._horizontalBarShown) {
                    this._l._e.removeChild(this._horizontalBar);
                    this._horizontalBarShown = false;
                }
                
                if (this._h > this._l._h) {
                    var height = this._l._h / this._h * this._l._h;
                    var hOffset = -this._y / this._h * this._l._h;
                    
                    this._verticalBar.style.height = height + "px";
                    this._verticalBar.style.top = hOffset + "px";
                    
                    if (!this._verticalBarShown) {
                        this._l._e.appendChild(this._verticalBar);
                        this._verticalBarShown = true;
                    }
                } else if (this._verticalBarShown) {
                    this._l._e.removeChild(this._verticalBar);
                    this._verticalBarShown = false;
                }
            },
            "setTop": function(y) {
                if (this._y !== y) {
                    if (this._o.virtual) {
                        this._y = y;
                        this._e.style.top = "0px";
                    } else {
                        View.fn.setTop.call(this, y);
                    }
                    if (y === 0) {
                        this.trigger("scrollTop", 0);
                    } else {
                        this.trigger("scrollTop", -y);
                    }
                    
                    
                    this._verticalBar.style.top = -this._y / this._h * this._l._h + "px";
                }
            }
        });
        
        return Scrollable;
    });
})();
