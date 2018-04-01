"use strict";
var Controller = require("./controller");
var Vector = require("../wType/vector");

var List = Controller.inherit({
    "className": "List",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this.data = new Vector();
        
        this.addHandler("get");
        this.addHandler("push");
        this.addHandler("clear");
    },
    "destructor": function() {
        this.data.destructor();
        
        Controller.fn.destructor.call(this);
    },
    "addElement": function(element) {
        this.data.push(element);
        this.trigger("newElement", element);
    },
    "clear": function() {
        this.data.clear();
        this.trigger("clear");
    },
    "_h_clear": function(ev) {
        this.clear();
    },
    "_h_get": function(ev) {
        this.clear();
        
        var data = ev.getData().at("data");
        var size = data.length();
        for (var i = 0; i < size; ++i) {
            this.addElement(data.at(i).clone());
        }
        this.initialized = true;
        this.trigger("data");
    },
    "_h_push": function(ev) {
        var data = ev.getData();
        
        var element = data.at("data").clone();
        this.addElement(element);
    }
});

module.exports = List;
