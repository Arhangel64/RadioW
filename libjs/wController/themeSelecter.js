"use strict";
var Controller = require("./controller");

var ThemeSelecter = Controller.inherit({
    "className": "ThemeSelecter",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this._data = {};
        this._selected = undefined;
        
        this.addHandler("get");
        this.addHandler("insert");
    },
    "destructor": function() {
        for (var i = 0; i < this._views.length; ++i) {
            this._views[i].off("select", this.select, this);
        }
        
        Controller.fn.destructor.call(this);
    },
    "addView": function(view) {
        Controller.fn.addView.call(this, view);
        
        view.on("select", this.select, this);
    },
    "_h_get": function(ev) {
        var pairs = ev.getData();
        this.trigger("clear");
        
        var data = pairs.at("data");
        var keys = data.getKeys();
        for (var i = 0; i < keys.length; ++i) {
            this._data[keys[i]] = data.at(keys[i]).clone()
            this.trigger("newElement", {key: keys[i], value: this._data[keys[i]] });
        }
        this.select(pairs.at("default").toString());
    },
    "_h_insert": function(ev) {
        var data = ev.getData();
        
        var key = data.at().toString();
        var value = data.at().clone()
        this._data[key] = value;
        this.trigger("newElement", {key: key, value: value});
    },
    "select": function(key) {
        if (!this._data[key]) {
            throw new Error("No such key");
        }
        this._selected = key;
        this.trigger("selected", {key: key, value: this._data[key]});
    }
});

module.exports = ThemeSelecter;
