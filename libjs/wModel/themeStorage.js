"use strict"

var Model = require("./model");

var Vocabulary = require("../wType/vocabulary");
var Address = require("../wType/address");
var String = require("../wType/string");
var Theme = require("./theme")

var ThemeStorage = Model.inherit({
    "className": "ThemeStorage",
    "constructor": function(address) {
        Model.fn.constructor.call(this, address);
        
        this._dtn = "budgie";
        this._data = new Vocabulary();
        
        this.addHandler("get");
        this._initThemes();
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
        vc.insert("default", new String(this._dtn));
        this.response(vc, "get", ev);
    },
    "_initThemes": function() {
        var budgie = new Theme(this._address["+"](new Address(["budgie"])), "budgie");
        this.insert(budgie);
    },
    "insert": function(theme) {
        this.addModel(theme);
        this._data.insert(theme.getName(), theme.getAddress());
        
        var vc = new Vocabulary();
        vc.insert("name", new String(theme.getName()));
        vc.insert("address", theme.getAddress());
        
        this.broadcast(vc, "insertion");
    }
});

module.exports = ThemeStorage;
