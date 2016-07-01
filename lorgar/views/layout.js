"use strict";
(function layout_js() {
    var moduleName = "views/layout";
    
    var defineArray = [];
    defineArray.push("views/view");
    
    define(moduleName, defineArray, function layout_module() {
        var View = require("views/view");
        
        var Layout = View.inherit({
            "className": "Layout",
            "constructor": function(options) {
                var base = {};
                W.extend(base, options);
                var element = document.createElement("div");
                View.fn.constructor.call(this, base, element);
                
                this._c = [];
            },
            "destructor": function() {
                this.clear();
                
                View.fn.destructor.call(this);
            },
            "_addChild": function(child, aligment) {
                child.applyTheme(this._currentTheme);
                aligment = aligment || 1;
                this._c.push({
                    c: child,
                    a: aligment
                });
                child.remove();
                this._e.appendChild(child._e);
                child._p = this;
                child.on("changeLimits", this._onChildChangeLimits, this);
            },
            "append": function(child, aligment) {
                this._addChild(child, aligment)
                
                if (this._w !== undefined && this._h !== undefined) {
                    child.setSize(this._w, this._h);
                }
            },
            "applyTheme": function(theme) {
                View.fn.applyTheme.call(this, theme);
                
                for (var i = 0; i < this._c.length; ++i) {
                    this._c[i].c.applyTheme(theme);
                }
            },
            "clear": function() {
                while (this._c.length) {
                    var c = this._c.pop();
                    c.c.destructor();
                }
            },
            "_onChildChangeLimits": function(child) {
                for (var i = 0; i < this._c.length; ++i) {
                    if (this._c[i].c === child) {
                        break;
                    }
                }
                if (i < this._c.length) {
                    this._positionElement(this._c[i]);
                }
            },
            "_positionElement": function(e) {
                var el = e.c;
                var a = e.a;
                el.setSize(this._w, this._h);
                
                switch (a) {
                    case Layout.Aligment.LeftTop:
                        el._e.style.top = "0";
                        el._e.style.bottom = "";
                        el._e.style.left = "0";
                        el._e.style.right = "";
                        break;
                    case Layout.Aligment.LeftCenter:
                        el._e.style.top = ((this._h - el._h) / 2) + "px";
                        el._e.style.bottom = "";
                        el._e.style.left = "0";
                        el._e.style.right = "";
                        break;
                    case Layout.Aligment.LeftBottom:
                        el._e.style.top = "";
                        el._e.style.bottom = "0";
                        el._e.style.left = "0";
                        el._e.style.right = "";
                        break;
                    case Layout.Aligment.CenterTop:
                        el._e.style.top = "0";
                        el._e.style.bottom = "";
                        el._e.style.left = ((this._w - el._w) / 2) + "px";
                        el._e.style.right = "";
                        break;
                    case Layout.Aligment.CenterCenter:
                        el._e.style.top = ((this._h - el._h) / 2) + "px";
                        el._e.style.bottom = "";
                        el._e.style.left = ((this._w - el._w) / 2) + "px";
                        el._e.style.right = "";
                        break;
                    case Layout.Aligment.CenterBottom:
                        el._e.style.top = "";
                        el._e.style.bottom = "0";
                        el._e.style.left = ((this._w - el._w) / 2) + "px";
                        el._e.style.right = "";
                        break;
                    case Layout.Aligment.RightTop:
                        el._e.style.top = "0";
                        el._e.style.bottom = "";
                        el._e.style.left = "";
                        el._e.style.right = "0";
                        break;
                    case Layout.Aligment.RightCenter:
                        el._e.style.top = ((this._h - el._h) / 2) + "px";
                        el._e.style.bottom = "";
                        el._e.style.left = "";
                        el._e.style.right = "0";
                        break;
                    case Layout.Aligment.RightBottom:
                        el._e.style.top = "";
                        el._e.style.bottom = "0";
                        el._e.style.left = "";
                        el._e.style.right = "0";
                        break;
                }
            },
            "removeChild": function(child) {
                this._e.removeChild(child._e);
                this._c.splice(this._c.indexOf(child), 1);
                child._p = undefined;
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
        
        return Layout;
    });
})();