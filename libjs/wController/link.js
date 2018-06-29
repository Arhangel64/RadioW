"use strict";

var Controller = require("./controller");
var String = require("./string");

var Address = require("../wType/address");

var Link = Controller.inherit({
    "className": "Link",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this.targetAddress = new Address([]);
        this.label = new String(addr['+'](hop));
        
        this.addController(this.label);
        
        this.addHandler("get");
    },
    "destructor": function() {
        this.targetAddress.destructor();
        
        Controller.fn.destructor.call(this);
    },
    "_h_get": function(ev) {
        var data = ev.getData();

        this.targetAddress = data.at("targetAddress").clone();
        
        this.trigger("data", this.targetAddress);
    }
});

var hop = new Address(["label"]);

module.exports = Link;
