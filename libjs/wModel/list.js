"use strict";

var Model = require("./model");
var Vector = require("../wType/vector");
var Vocabulary = require("../wType/vocabulary");
var Object = require("../wType/object")

var List = Model.inherit({
    "className": "List",
    "constructor": function(address) {
        Model.fn.constructor.call(this, address);
        
        this._data = new Vector();
        
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
    "push": function(obj) {
        if (!(obj instanceof Object)) {
            throw new Error("An attempt to push into list unserializable value");
        }
        this._data.push(obj);
        
        var vc = new Vocabulary();
        vc.insert("data", obj.clone());
        
        this.broadcast(vc, "push");
    }
});

module.exports = List;
