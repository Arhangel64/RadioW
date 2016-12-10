"use strict";
var Controller = require("./controller");

var String = Controller.inherit({
    "className": "String",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this._data = "";
        
        this.addHandler("get");
    },
    "_h_get": function(ev) {
        var data = ev.getData();
        
        this._data = data.at("data").toString();
        
        for (var i = 0; i < this._views.length; ++i) {
            this._views[i].data(this._data);
        }
    }
});

module.exports = String;
