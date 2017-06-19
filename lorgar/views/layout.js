"use strict";
(function layout_js() {
    var moduleName = "views/layout";
    
    var defineArray = [];
    defineArray.push("views/view");
    defineArray.push("views/helpers/scrollable");
    
    define(moduleName, defineArray, function layout_module() {
        var View = require("views/view");
        var Scrollable = require("views/helpers/scrollable");
        
        var Layout = View.inherit({
            "className": "Layout",
            "constructor": function(controller, options) {
                var base = {
                    scrollable: Layout.Scroll.None
                };
                W.extend(base, options);
                var element = document.createElement("div");
                View.fn.constructor.call(this, controller, base, element);
                
                this._c = [];
                
                if (this._o.scrollable) {
                    this._scr = new Scrollable({
                        vertical: !!(this._o.scrollable & 1),
                        horizontal: !!(this._o.scrollable & 2),
                        virtual: !!(this._o.scrollable & 4)
                    }, this);
                }
            },
            "destructor": function() {
                this.clear();
                if (this._o.scrollable) {
                    this._scr.destructor();
                }
                
                View.fn.destructor.call(this);
            },
            "_addChild": function(child, aligment, index) {
                aligment = aligment || 1;
                var item = {
                    c: child,
                    a: aligment
                }
                if (index !== undefined) {
                    this._c.splice(index, 0, item);
                } else {
                    this._c.push(item);
                }
                child.remove();
                if (this._o.scrollable) {
                    this._scr.appendChild(child._e);
                } else {
                    this._e.appendChild(child._e);
                }
                child._p = this;
                child.on("changeLimits", this._onChildChangeLimits, this);
            },
            "append": function(child, aligment) {
                this._addChild(child, aligment)
                if (this._o.scrollable) {
                    this._scr.maximizeMinSize(child);
                    if (this._w !== undefined && this._h !== undefined) {
                        this._scr.setSize(this._w, this._h);
                    }
                }
                
                if (this._w !== undefined && this._h !== undefined) {
                    child.setSize(this._w, this._h);
                }
            },
            "clear": function() {
                while (this._c.length) {
                    var c = this._c[this._c.length - 1];
                    c.c.destructor();
                }
                if (this._o.scrollable) {
                    this._scr.setMinSize(0, 0);
                    this._scr.setSize(this._w, this._h);
                }
            },
            "_onChildChangeLimits": function(child) {
                var i;
                if (this._o.scrollable) {
                    var w = 0;
                    var h = 0;
                    for (i = 0; i < this._c.length; ++i) {
                        w = Math.max(this._c[i].c._o.minWidth, w);
                        h = Math.max(this._c[i].c._o.minHeight, h);
                    }
                    this._scr.setMinSize(w, h);
                    this._scr.setSize(this._w, this._h);
                    for (i = 0; i < this._c.length; ++i) {
                        this._positionElement(this._c[i]);
                    }
                } else {
                    for (i = 0; i < this._c.length; ++i) {
                        if (this._c[i].c === child) {
                            break;
                        }
                    }
                    if (i < this._c.length) {
                        this._positionElement(this._c[i]);
                    }
                }
            },
            "_positionElement": function(e) {
                var el = e.c;
                var a = e.a;
                var h = this._h;
                var w = this._w;
                if (this._o.scrollable) {
                    h = this._scr._h;
                    w = this._scr._w;
                }
                el.setSize(this._w, this._h);
                
                switch (a) {
                    case Layout.Aligment.LeftTop:
                        el.setTop(0);
                        el.setLeft(0);
                        break;
                    case Layout.Aligment.LeftCenter:
                        el.setTop((h - el._h) / 2);
                        el.setLeft(0);
                        break;
                    case Layout.Aligment.LeftBottom:
                        el.setTop(h - el._h);
                        el.setLeft(0);
                        break;
                    case Layout.Aligment.CenterTop:
                        el.setTop(0);
                        el.setLeft((w - el._w) / 2)
                        break;
                    case Layout.Aligment.CenterCenter:
                        el.setTop((h - el._h) / 2);
                        el.setLeft((w - el._w) / 2)
                        break;
                    case Layout.Aligment.CenterBottom:
                        el.setTop(h - el._h);
                        el.setLeft((w - el._w) / 2)
                        break;
                    case Layout.Aligment.RightTop:
                        el.setTop(0);
                        el.setLeft(w - el._w)
                        break;
                    case Layout.Aligment.RightCenter:
                        el.setTop((h - el._h) / 2);
                        el.setLeft(w - el._w)
                        break;
                    case Layout.Aligment.RightBottom:
                        el.setTop(h - el._h);
                        el.setLeft(w - el._w)
                        break;
                }
            },
            "removeChild": function(child) {
                var i;
                for (i = 0; i < this._c.length; ++i) {
                    if (this._c[i].c === child) {
                        break;
                    }
                }
                if (i !== this._c.length) {
                    this._removeChildByIndex(i);
                }
            },
            "_removeChildByIndex": function(i) {
                var child = this._c[i].c;
                this._c.splice(i, 1);
                child._p = undefined;
                
                if (this._o.scrollable) {
                    this._scr.removeChild(child._e);
                    var w = 0;
                    var h = 0;
                    for (var i = 0; i < this._c.length; ++i) {
                        w = Math.max(this._c[i].c._o.minWidth, w);
                        h = Math.max(this._c[i].c._o.minHeight, h);
                    }
                    this._scr.setMinSize(w, h);
                    this._scr.setSize(this._w, this._h);
                } else {
                    this._e.removeChild(child._e);
                }
                
                child.off("changeLimits", this._onChildChangeLimits, this);
            },
            "setSize": function(w, h) {
                View.fn.setSize.call(this, w, h);
                
                for (var i = 0; i < this._c.length; ++i) {
                    this._positionElement(this._c[i]);
                }
            }
        });
        
        Layout.Aligment = {
            "LeftTop": 1,
            "LeftCenter": 4,
            "LeftBottom": 7,
            "CenterTop": 2,
            "CenterCenter": 5,
            "CenterBottom": 8,
            "RightTop": 3,
            "RightCenter": 6,
            "RightBottom": 9
        };
        
        Layout.Scroll = {
            "None": 0,
            "Vertical": 1,
            "Horizontal": 2,
            "Both": 3,
            "Virtual": 4,
            "VirtualVertical": 5,
            "VirtualHorizontal": 6,
            "VirtualBoth": 7
        }
        
        return Layout;
    });
})();
