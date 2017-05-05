"use strict";

var Model = require("./model");
var ModelString = require("./string")

var String = require("../wType/string");

var Vocabulary = require("../wType/vocabulary");
var Address = require("../wType/address");

var Link = Model.inherit({
    "className": "Link",
    "constructor": function(addr, text, tAddress) {
        Model.fn.constructor.call(this, addr);
        
        this.addHandler("get");
        
        this._targetAddress = tAddress;
        
        var hop = new Address(["label"]);
        this.label = new ModelString(addr["+"](hop), text);
        this.addModel(this.label);
        
        hop.destructor();
    },
    "destructor": function() {
        this._targetAddress.destructor();
        
        Model.fn.destructor.call(this);
    },
    "_h_get": function(ev) {
        var vc = new Vocabulary();
        
        vc.insert("targetAddress", this._targetAddress.clone());
        this.response(vc, "get", ev);
    },
    "_h_subscribe": function(ev) {
        Model.fn._h_subscribe.call(this, ev);
        
        this._h_get(ev);
    }
});

module.exports = Link;
