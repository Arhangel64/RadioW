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
        
        while (this._controllers.length) {
            var controller = this._controllers[this._controllers.length - 1]
            this._removeController(controller);
            controller.destructor();
        }
        
        this.trigger("clear");
    },
    "_h_clear": function(ev) {
        this.clear();
    },
    "_h_get": function(ev) {
        this.clear();
        
        var data = ev.getData().at("data");
        var size = data.size();
        for (var i = 0; i < size; ++i) {
            this.addElement(data.at(i).clone());
        }
    },
    "_h_push": function(ev) {
        var data = ev.getData();
        
        var element = data.at("data").clone();
        this.addElement(element);
    }
});

module.exports = List;
