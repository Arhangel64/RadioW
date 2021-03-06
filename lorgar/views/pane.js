"use strict";

(function() {
    var moduleName = "views/pane";
    
    var defineArray = [];
    defineArray.push("views/view");
    defineArray.push("views/layout");
    defineArray.push("views/label");
    defineArray.push("views/image");
    defineArray.push("lib/wController/localModel");
    
    define(moduleName, defineArray, function() {
        var View = require("views/view");
        var Layout = require("views/layout");
        var Label = require("views/label");
        var Image = require("views/image");
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
                
                var lm = this._labelModel = new LM({
                    fontFamily: "casualFont"
                });
                
                if (this._f.hasImage()) {
                    this._image = new Image(this._f.image);
                    this.append(this._image, Layout.Aligment.CenterCenter);
                }
                
                var name = this._f.data.at("name");
                this._labelModel.setData(name || "");
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
                switch (key) {
                    case "name":
                        this._labelModel.setData(value.toString());
                        break;
                    case "image":
                        this._image = new Image(this._f.image);
                        this.append(this._image, Layout.Aligment.LeftTop, 0);
                        break;
                }
            },
            "_onRemoveElement": function(key) {
                switch (key) {
                    case "name":
                        this._labelModel.setData("");
                        break;
                    case "image":
                        this._image.destructor();
                        break;
                }
            },
            "setSize": function(w, h) {
                Layout.fn.setSize.call(this, w, h);
                
                if (this._f.hasImage()) {
                    this._image.setSize(w, h);
                }
            }
        });
        
        return Pane;
    });
})();
