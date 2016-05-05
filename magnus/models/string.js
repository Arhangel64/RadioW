"use strict";

var Model = require("./model");
var String = require("../lib/wType/string");
var Address = require("../lib/wType/address");
var Vocabulary = require("../lib/wType/vocabulary");

var ModelString = Model.inherit({
    "className": "String",
    "constructor": function(address, string) {
        Model.fn.constructor.call(this, address);
        
        this._data = new String(string);
        
        this.addHandler("get");
    },
    "_h_subscribe": function(ev) {
        Model.fn._h_subscribe.call(this, ev);
        
        this._h_get(ev);
    },
    "_h_get": function(ev) {
        var vc = new Vocabulary();
        
        vc.insert("data", this._data.clone());
        this.response(vc, "get", ev);
    }
});

module.exports = ModelString;