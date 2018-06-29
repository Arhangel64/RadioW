"use strict";

var Model = require("./model");
var ModelString = require("./string");

var Vocabulary = require("../wType/vocabulary");
var Boolean = require("../wType/boolean");
var Address = require("../wType/address");
var String = require("../wType/string");

var Button = Model.inherit({
    "className": "Button",
    "constructor": function(address) {
        Model.fn.constructor.call(this, address);
        
        this._enabled = true;
        this._hasImage = false;
        this._hasLabel =false;
        this._imageName = undefined;
        this._label = undefined;
        
        this.addHandler("get");
        this.addHandler("activate");
    },
    "setImage": function(name) {
        if (this._hasImage) {
            if (this._imageName !== name) {
                this._image = name;
                var vc = new Vocabulary();
                vc.insert("image", new String(this._imageName));
                this.broadcast(vc, "changeImage");
            }
        } else {
            this._image = name;
            this._hasImage = true;
            var vc = new Vocabulary();
            vc.insert("image", new String(this._imageName));
            this.broadcast(vc, "setImage");
        }
    },
    "setEnabled": function(enabled) {
        if (enabled !== this._enabled) {
            this._enabled = enabled;
            var vc = new Vocabulary();
            vc.insert("enabled", new Boolean(this._enabled));
            this.broadcast(vc, "setEnabled");
        }
    },
    "setLabel": function(text) {
        if (this._hasLabel) {
            this._label.set(text);
        } else {
            this._label = new ModelString(this._address["+"](labelHop), text);
            this.addModel(this._label);
            var vc = new Vocabulary();
            vc.insert("hasLabel", new Boolean(true));
            vc.insert("label", this._label.getAddress());
            this.broadcast(vc, "setLabel");
            this._hasLabel = true;
        }
    },
    "_h_subscribe": function(ev) {
        Model.fn._h_subscribe.call(this, ev);
        
        this._h_get(ev);
    },
    "_h_get": function(ev) {
        var vc = new Vocabulary();
        vc.insert("hasImage", new Boolean(this._hasImage));
        if (this._hasImage) {
            vc.insert("image", new String(this._imageName));
        }
        vc.insert("hasLabel", new Boolean(this._hasLabel));
        if (this._hasLabel) {
            vc.insert("label", this._label.getAddress());
        }
        vc.insert("enabled", new Boolean(this._enabled));
        
        this.response(vc, "get", ev);
    },
    "_h_activate": function() {
        if (this._enabled) {
            this.trigger("activated");
        }
    }
});

var labelHop = new Address(["label"]);

module.exports = Button;
