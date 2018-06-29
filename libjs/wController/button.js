"use strict";

var Controller = require("./controller");
var String = require("./string");
var Vocabulary = require("../wType/vocabulary");

var Button = Controller.inherit({
    "className": "Button",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this.enabled = false;
        this.hasLabel = false;
        
        this.addHandler("get");
        this.addHandler("setLabel");
        this.addHandler("setImage");
        this.addHandler("setEnabled");
        this.addHandler("changeImage");
    },
    "destructor": function() {
        
        Controller.fn.destructor.call(this);
    },
    "activate": function() {
        if (this.enabled) {
            this.send(new Vocabulary, "activate");
        }
    },
    "_h_changeImage": function(ev) {
        
    },
    "_h_get": function(ev) {
        this._h_setLabel(ev);
        this._h_setImage(ev);
        this._h_setEnabled(ev);
    },
    "_h_setEnabled": function(ev) {
        var data = ev.getData();
        
        var enabled = data.at("enabled").valueOf();
        if (this.enabled !== enabled) {
            this.enabled = enabled;
            this.trigger("setEnabled", this.enabled);
        }
    },
    "_h_setLabel": function(ev) {
        var data = ev.getData();
        var hasLabel = data.at("hasLabel").valueOf();
        
        if (hasLabel !== this.hasLabel) {
            this.hasLabel = hasLabel;
            if (hasLabel) {
                this.label = new String(data.at("label").clone());
                this.addController(this.label);
                this.trigger("setLabel", true, this.label);
            } else {
                this.trigger("setLabel", false);
                this.removeController(this.label);
                this.label.destructor();
            }
        }
    },
    "_h_setImage": function(ev) {
        
    }
});

module.exports = Button;
