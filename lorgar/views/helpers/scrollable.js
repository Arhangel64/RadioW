"use strict";
(function scrollable_js() {
    var moduleName = "views/helpers/scrollable";
    
    var defineArray = [];
    defineArray.push("views/view");
    
    define(moduleName, defineArray, function scrollable_module() {
        var View = require("views/view");
        
        var Scrollable = View.inherit({
            "className": "Scrollable",
            "constructor": function Scrollable(options, layout) {
                var base = {
                    vertical: false,
                    horizontal: false,
                    draggable: true
                };
                W.extend(base, options);
                View.fn.constructor.call(this, base);
                
                this._l = layout;
                this._l.on("resize", this.setSize, this);
                this._l._e.appendChild(this._e);
                
                this.on("dragStart", this._onDragStart, this);
                this.on("drag", this._onDrag, this);
                this.on("dragEnd", this._onDragEnd, this);
            },
            "destructor": function() {
                this._l._e.removeChild(this._e);
                
                View.fn.destructor.call(this);
            },
            "appendChild": function(e) {
                this._e.appendChild(e);
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
                console.log("end")
            },
            "_onDragStart": function(pos) {
                this._sX = this._x;
                this._sY = this._y;
                console.log("start");
            },
            "removeChild": function(e) {
                this._e.removeChild(e);
            }
        });
        
        return Scrollable;
    });
})();