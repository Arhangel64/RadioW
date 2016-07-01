"use strict";
(function themeSelecter_js() {
    var moduleName = "models/themeSelecter";
    
    var defineArray = [];
    defineArray.push("models/model");
    
    define(moduleName, defineArray, function themeSelecter_module() {
        var Model = require("models/model");
        
        var ThemeSelecter = Model.inherit({
            "className": "ThemeSelecter",
            "constructor": function(addr) {
                Model.fn.constructor.call(this, addr);
                
                this._data = {};
                this._selected = undefined;
                
                this.addHandler("get");
                this.addHandler("insert");
            },
            "destructor": function() {
                for (var i = 0; i < this._c.length; ++i) {
                    this._c[i].off("select", this.select, this);
                }
                
                Model.fn.destructor.call(this);
            },
            "addView": function(view) {
                Model.fn.addView.call(this, view);
                
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
        
        return ThemeSelecter;
    });
})();