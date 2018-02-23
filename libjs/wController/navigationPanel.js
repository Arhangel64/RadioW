"use strict";
var List = require("./list");
var Link = require("./link");

var NavigationPanel = List.inherit({
    "className": "NavigationPanel",
    "constructor": function(addr) {
        List.fn.constructor.call(this, addr);
    },
    "addElement": function(element) {
        var address = element.at("address").clone();
        
        var ctrl = new Link(address);
        this.addController(ctrl);
        
        List.fn.addElement.call(this, element);
    },
    "clear": function() {
        List.fn.clear.call(this);
        this.clearChildren();
    }
});

module.exports = NavigationPanel;
