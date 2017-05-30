"use strict";
var Controller = require("./controller");

var String = Controller.inherit({
    "className": "String",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this.data = "";
        
        this.addHandler("get");
    },
    "_h_get": function(ev) {
        var data = ev.getData();
        
        this.data = data.at("data").toString();
        
        this.trigger("data");
    }
});

module.exports = String;
