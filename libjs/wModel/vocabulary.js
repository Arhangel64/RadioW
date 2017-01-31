"use strict";

var Model = require("./model");
var Vector = require("../wType/vector");
var Vocabulary = require("../wType/vocabulary");
var Object = require("../wType/object")
var String = require("../wType/string");

var ModelVocabulary = Model.inherit({
    "className": "Vocabulary",
    "constructor": function(address) {
        Model.fn.constructor.call(this, address);
        
        this._data = new Vocabulary();
        
        this.addHandler("get");
    },
    "destructor": function() {
        this._data.destructor();
        
        Model.fn.destructor.call(this);
    },
    "clear": function() {
        this._data.clear();
        
        if (this._regestered) {
            this.broadcast(new Vocabulary(), "clear");
        }
    },
    "erase": function(key) {
        this._data.erase(key);
        
        if (this._registered) {
            var vc = new Vocabulary();
            var insert = new Vocabulary();
            var erase = new Vector();
            
            erase.push(new String(key));
            vc.insert("insert", insert);
            vc.insert("erase", erase);
            
            this.broadcast(vc, "change");
        }
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
    "insert": function(key, value) {
        if (this._registered) {
            var vc = new Vocabulary();
            var insert = new Vocabulary();
            var erase = new Vector();
            
            if (this._data.has(key)) {
                erase.push(new String(key));
            }
            this._data.insert(key, value);
            insert.insert(key, value.clone());
            
            vc.insert("insert", insert);
            vc.insert("erase", erase);
            
            this.broadcast(vc, "change");
            
        } else {
            this._data.insert(key, value);
        }
    }
});

module.exports = ModelVocabulary;
