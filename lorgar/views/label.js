"use strict";
(function view_label_js() {
    var moduleName = "views/label";
    
    var defineArray = [];
    defineArray.push("views/view");
    defineArray.push("lib/fonts/Liberation");
    
    define(moduleName, defineArray, function view_label_module() {
        var View = require("views/view");
        
        var Label = View.inherit({
            "className": "Label",
            "constructor": function(controller, options) {
                var base = {};
                W.extend(base, options)
                View.fn.constructor.call(this, controller, base);
                
                this._timeout = undefined;
                this._recalculationScheduled = false;
                
                this._e.innerText = this._f.data || "";
                if (this._f.data !== "") {
                    this._scheduleRecalculation();
                }
            },
            "destructor": function() {
                if (this._recalculationScheduled) {
                    clearTimeout(this._timeout);
                }
                
                View.fn.destructor.call(this);
            },
            "_onAddProperty": function(key, propertyName) {
                View.fn._onAddProperty.call(this, key, propertyName);
                
                if (sizeChangingProperties.indexOf(propertyName) !== -1) {
                    this._scheduleRecalculation();
                }
            },
            "_onData": function() {
                if (this._e.innerText !== this._f.data) {
                    this._e.innerText = this._f.data || "";
                    this._scheduleRecalculation();
                }
            },
            "_recalculateSize": function() {
                var fs = parseFloat(this._e.style.fontSize) || 16;
                var fontFamily = this._e.style.fontFamily || "Liberation";
                
                var h = fs + 2;
                var w = Label.calculateSingleString(fontFamily, fs, this._f.data || "");
                this.setConstSize(w, h);
                this._recalculationScheduled = false;
            },
            "_scheduleRecalculation": function() {
                if (!this._recalculationScheduled) {
                    this._timeout = setTimeout(this._recalculateSize.bind(this), 10);
                    this._recalculationScheduled = true;
                }
            }
        });
        
        var sizeChangingProperties = ["fontSize", "fontFamily", "whiteSpace"];
        
        Label.calculateSingleString = function(family, size, string) {
            var fontStorage = Label.fonts[family] || Label.fonts["Liberation"];
            
            var width = 0;
            var mul = size / fontStorage.unitsPerEm;
            var aw = fontStorage.advanceWidthArray;
            
            for (var i = 0; i < string.length; ++i) {
                var letter = string.charCodeAt(i);
                var l = aw[letter] || 1536;
                width += (l * mul);
            }
            
            return Math.ceil(width);
        };
        
        Label.fonts = {
            Liberation: require("lib/fonts/Liberation")
        };
        
        return Label;
    });
})();
