"use strict";

var Model = require("./model");
var String = require("../wType/string");
var Address = require("../wType/address");
var Vocabulary = require("../wType/vocabulary");

var ModelString = Model.inherit({
    "className": "String",
    "constructor": function(address, string) {
        Model.fn.constructor.call(this, address);
        
        this._data = new String(string);
        
        this.addHandler("get");
    },
    "destructor": function() {
        this._data.destructor();
        
        Model.fn.destructor.call(this);
    },
    "_h_subscribe": function(ev) {
        Model.fn._h_subscribe.call(this, ev);
        
        this._h_get(ev);
    },
    "_h_get": function(ev) {
        var vc = new Vocabulary();
        
        vc.insert("data", this._data.clone());
        this.response(vc, "get", ev);
    },
    "set": function(value) {
        this._data.destructor();
        
        this._data = new String(value.toString());
        
        if (this._registered) {
            var vc = new Vocabulary();
            
            vc.insert("data", this._data.clone());
            this.broadcast(vc, "get");
        }
    }
});

module.exports = ModelString;
