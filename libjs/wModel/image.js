"use strict";

var Model = require("./model");
var Uint64 = require("../wType/uint64");
var Address = require("../wType/address");
var Vocabulary = require("../wType/vocabulary");

var ModelImage = Model.inherit({
    "className": "Image",
    "constructor": function(address, uint64) {
        Model.fn.constructor.call(this, address);
        
        this._data = uint64;
        
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
    "set": function(uint64) {
        this._data.destructor();
        
        this._data = uint64;
        
        if (this._registered) {
            var vc = new Vocabulary();
            
            vc.insert("data", this._data.clone());
            this.broadcast(vc, "get");
        }
    }
});

module.exports = ModelImage;
