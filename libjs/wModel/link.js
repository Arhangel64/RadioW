"use strict";

var Model = require("./model");
var String = require("../wType/string");
var Vocabulary = require("../wType/vocabulary");

var Link = Model.inherit({
    "className": "Link",
    "constructor": function(addr, text, tAddress) {
        Model.fn.constructor.call(this, addr);
        
        this.addHandler("get");
        
        this._targetAddress = tAddress;
        this._text = new String(text);
    },
    "_h_get": function(ev) {
        var vc = new Vocabulary();
        
        vc.insert("targetAddress", this._targetAddress.clone());
        vc.insert("text", this._text.clone());
        this.response(vc, "get", ev);
    },
    "_h_subscribe": function(ev) {
        Model.fn._h_subscribe.call(this, ev);
        
        this._h_get(ev);
    }
});

module.exports = Link;
