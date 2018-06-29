"use strict";
(function() {
    var moduleName = "views/button";
    
    var deps = [];
    deps.push("views/layout");
    deps.push("views/label");
    
    define(moduleName, deps, function() {
        var Layout = require("views/layout");
        var Label = require("views/label");
        
        var Button = Layout.inherit({
            "className": "Button",
            "constructor": function(controller, options) {
                var base = {
                    padding: 5
                };
                W.extend(base, options)
                Layout.fn.constructor.call(this, controller, base);
                
                this.addClass("hoverable");
                this._enabled = true;
                this._hasLabel = false;
                this._e.addEventListener("click", this._onClick.bind(this), false);
                
                controller.on("setEnabled", this._onSetEnabled, this);
                controller.on("setLabel", this._onSetLabel, this);
            },
            "destructor": function() {
                this._f.off("setEnabled", this._onSetEnabled, this);
                this._f.off("setLabel", this._onSetLabel, this);
                
                Layout.fn.destructor.call(this);
            },
            "append": function(child, aligment, index) {
                this._updateLimits();
                
                Layout.fn.append.call(this, child, aligment, index);
            },
            "_onChildChangeLimits": function(child) {
                this._updateLimits();
                
                Layout.fn._onChildChangeLimits.call(this, child);
            },
            "_onClick": function() {
                if (this._enabled) {
                    this._f.activate();
                }
            },
            "_onSetEnabled": function(enabled) {
                if (this._enabled !== enabled) {
                    this._enabled = enabled;
                    if (this._enabled) {
                        this.addClass("hoverable");
                        this.removeClass("disabled");
                    } else {
                        this.removeClass("hoverable");
                        this.addClass("disabled");
                    }
                }
            },
            "_onSetLabel": function(hasLabel, label) {
                if (this._hasLabel !== hasLabel) {
                    this._hasLabel = hasLabel;
                    if (this._hasLabel) {
                        this._label = new Label(label);
                        this.append(this._label, Layout.Aligment.CenterCenter);
                    } else {
                        this._label.destructor();
                        delete this._label();
                    }
                }
            },
            "_updateLimits": function() {
                var minWidth = this._o.padding * 2;
                var maxWidth = this._o.padding * 2;
                var minHeight = this._o.padding * 2;
                var maxHeight = this._o.padding * 2;
                
                if (this._hasLabel) {
                    minWidth += this._label._o.minWidth;
                    minHeight += this._label._o.minHeight;
                    maxWidth += this._label._o.maxWidth;
                    maxHeight += this._label._o.maxHeight;
                }
                
                this._setLimits(minWidth, minHeight, maxWidth, maxHeight);
            }
        });
        
        return Button;
    })
})();

