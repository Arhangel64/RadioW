"use strict";
(function nav_js() {
    var moduleName = "views/nav";
    
    var defineArray = [];
    defineArray.push("views/layout");
    defineArray.push("views/string");
    
    define(moduleName, defineArray, function nav_module() {
        var Layout = require("views/layout");
        var String = require("views/string");
        
        var Nav = Layout.inherit({
            "className": "Nav",
            "constructor": function(options) {
                var base = {
                    "font-size": 16,
                    "padding": 5
                };
                W.extend(base, options);
                Layout.fn.constructor.call(this, base);
                
                this._initProxy();
                
                this._data = "";
                this._label = new String();
                this._label.on("measuresChanged", this._onLabelMeasuresChanged, this);
                
                this.append(this._label, 5);
                this.addClass("hoverable");
                
                this._initHandlers();
            },
            "destructor": function() {
                this._e.removeEventListener("click", this._proxy.onClick, false);
                
                Layout.fn.destructor.call(this);
            },
            "data": function(data) {
                this._data = data;
                this._label.data(data);
            },
            "_initHandlers": function() {
                this._e.addEventListener("click", this._proxy.onClick, false);
            },
            "_initProxy": function() {
                this._proxy = {
                    onClick: this._onClick.bind(this)
                }
            },
            "_onClick": function(e) {
                this.trigger("activate");
            },
            "_onLabelMeasuresChanged": function() {
                var fs = this._label.getFontSize();
                var fontFamily = "Liberation";
                
                var h = fs + 2;
                var w = String.calculateSingleString(fontFamily, fs, this._data);
                this.setMaxSize(w + (this._o.padding * 2), this._o.maxHeight);
                this._label.setMaxSize(w, h);
            }
        });
        
        return Nav;
    });
})();