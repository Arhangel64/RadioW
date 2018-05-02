"use strict";
(function nav_js() {
    var moduleName = "views/nav";
    
    var defineArray = [];
    defineArray.push("views/layout");
    defineArray.push("views/label");
    
    define(moduleName, defineArray, function nav_module() {
        var Layout = require("views/layout");
        var Label = require("views/label");
        
        var Nav = Layout.inherit({
            "className": "Nav",
            "constructor": function(controller, options) {
                var base = {
                    "padding": 5
                };
                W.extend(base, options);
                Layout.fn.constructor.call(this, controller, base);
                
                this._initProxy();
                
                this._label = new Label(this._f.label);
                this._label.on("changeLimits", this._onChangeLimits, this);
                this._onChangeLimits();
                
                this.append(this._label, Layout.Aligment.CenterCenter);
                this.addClass("hoverable");
                
                this._e.addEventListener("click", this._proxy.onClick, false);
            },
            "destructor": function() {
                this._e.removeEventListener("click", this._proxy.onClick, false);
                
                Layout.fn.destructor.call(this);
            },
            "_initProxy": function() {
                this._proxy = {
                    onClick: this._onClick.bind(this)
                }
            },
            "_onChangeLimits": function() {
                this._o.minWidth = this._label._o.minWidth + this._o.padding * 2;
                this._o.maxWidth = this._label._o.maxWidth + this._o.padding * 2;
                this._o.minHeight = this._label._o.minHeight + this._o.padding * 2;
                this._o.maxHeight = this._label._o.maxHeight + this._o.padding * 2;
                
                this.trigger("changeLimits", this);
            },
            "_onClick": function(e) {
                lorgar.changePage(this._f.targetAddress);
            }
        });
        
        return Nav;
    });
})();
