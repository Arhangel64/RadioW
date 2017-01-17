"use strict";

var ModelVocabulary = require("./vocabulary");
var Vocabulary = require("../wType/vocabulary");
var String = require("../wType/string");
var Uint64 = require("../wType/uint64");

var Attributes = ModelVocabulary.inherit({
    "className": "Attributes",
    "constructor": function(addr) {
        ModelVocabulary.fn.constructor.call(this, addr);
        
        this._attributes = global.Object.create(null);
    },
    "addAttribute": function(key, model) {
        var old = this._attributes[key];
        if (old) {
            throw new Error("Attribute with key " + key + " already exists");
        }
        this._attributes[key] = model;
        this.addModel(model);
        
        var vc = new Vocabulary();
        vc.insert("name", new String(key));
        vc.insert("address", model.getAddress());
        vc.insert("type", new Uint64(model.getType()));
        this.insert(key, vc);
    },
    "removeAttribute": function(key) {
        var model = this._attributes[key];
        if (!model) {
            throw new Error("An attempt to access non existing Attribute");
        }
        delete this._attributes[key];
        this.erase(key);
        this.removeModel(model);
        model.destructor();
    },
    "setAttribute": function(key, value) {
        this._attributes[key].set(value);
    }
});

module.exports = Attributes;
