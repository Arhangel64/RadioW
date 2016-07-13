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
                    "padding": 5
                };
                W.extend(base, options);
                Layout.fn.constructor.call(this, base);
                
                this._initProxy();
                
                this._data = "";
                this._label = new String();
                
                this.append(this._label, Layout.Aligment.CenterCenter);
                this.addClass("hoverable");
                
                this._initHandlers();
            },
            "destructor": function() {
                this._e.removeEventListener("click", this._proxy.onClick, false);
                
                Layout.fn.destructor.call(this);
            },
            "_applyProp": function(pair) {
                var counter = 0;
                var value = this._currentTheme[pair.k];
                if (value) {
                    if (this._e.style[pair.p] !== value) {
                        this._e.style[pair.p] = value;
                        if (pair.p === "fontSize" || pair.p === "fontFamily") {
                            return true
                        }
                    }
                }
                return false;
            },
            "applyTheme": function(theme) {
                this._currentTheme = theme;
                var counter = 0;;
                
                for (var i = 0; i < this._styleProperties.length; ++i) {
                    if (this._applyProp(this._styleProperties[i])){
                        ++counter;
                    }
                }
                if (counter > 0) {
                    this.trigger("measuresChanged");
                }
            },
            "data": function(data) {
                this._data = data;
                this._label.data(data);
                this.trigger("measuresChanged");
            },
            "_initHandlers": function() {
                this._e.addEventListener("click", this._proxy.onClick, false);
                this.on("measuresChanged", this._onMeasuresChanged, this);
            },
            "_initProxy": function() {
                this._proxy = {
                    onClick: this._onClick.bind(this)
                }
            },
            "_onClick": function(e) {
                this.trigger("activate");
            },
            "_onMeasuresChanged": function() {
                var fs = parseFloat(this._e.style.fontSize) || 16;
                var fontFamily = this._e.style.fontFamily || "Liberation";
                
                var h = fs + 2;
                var w = String.calculateSingleString(fontFamily, fs, this._data);
                this.setMaxSize(w + (this._o.padding * 2), this._o.maxHeight);
                this._label.setMaxSize(w, h);
            }
        });
        
        return Nav;
    });
})();