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
            "constructor": function(controller, options) {
                var base = {
                    
                };
                W.extend(base, options);
                Layout.fn.constructor.call(this, controller, options);
                
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
