"use strict";

(function() {
    var moduleName = "views/pane";
    
    var defineArray = [];
    defineArray.push("views/view");
    defineArray.push("views/layout");
    defineArray.push("views/label");
    defineArray.push("lib/wController/localModel");
    
    define(moduleName, defineArray, function() {
        var View = require("views/view");
        var Layout = require("views/layout");
        var Label = require("views/label");
        var LM = require("lib/wController/localModel");
        
        var Pane = Layout.inherit({
            "constructor": function PaneView (controller, options) {
                var base = {
                    
                };
                W.extend(base, options);
                Layout.fn.constructor.call(this, controller, options);
                
                this._initProxy();
                this.addClass("hoverable");
                this._e.addEventListener("click", this._proxy.onClick, false);
                
                var lm = this._labelModel = new LM();
                var name = this._f.data.at("name");
                this._labelModel.data = name || "";
                this._labelView = new Label(this._labelModel);
                this._labelView.on("changeLimits", this._onLabelChangeLimits, this);
                this.append(this._labelView, Layout.Aligment.CenterCenter);
                
                this._f.on("newElement", this._onNewElement, this);
                this._f.on("removeElement", this._onRemoveElement, this);
                
                this._uncyclic.push(function() {
                    lm.destructor();
                });
            },
            "destructor": function() {
                this._e.removeEventListener("click", this._proxy.onClick, false);
                
                Layout.fn.destructor.call(this);
            },
            "_applyProperties": function() {
                this._onAddProperty("secondaryColor", "background");
                
                Layout.fn._applyProperties.call(this);
            },
            "_initProxy": function() {
                this._proxy = {
                    onClick: this._onClick.bind(this)
                };
            },
            "_onClick": function() {
                if (this._f.data.at("hasPageLink").valueOf() === true) {
                    this.trigger("activate", this._f.data.at("pageLink").clone());
                }
            },
            "_onLabelChangeLimits": function(label) {
                this.setMinSize(label._o.minWidth, this._h);
            },
            "_onNewElement": function(key, value) {
                if (key === "name") {
                    this._labelModel.data = value.toString();
                    this._labelModel.trigger("data");
                }
            },
            "_onRemoveElement": function(key) {
                if (key === "name") {
                    this._labelModel.data = "";
                    this._labelModel.trigger("data");
                }
            }
        });
        
        return Pane;
    });
})();
