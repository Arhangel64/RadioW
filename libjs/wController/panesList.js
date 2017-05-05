"use strict";
var List = require("./list");

var String = require("./string");

var PanesList = List.inherit({
    "className": "PanesList",
    "constructor": function PanesListModel(addr) {
        List.fn.constructor.call(this, addr);
    },
    "addElement": function(element) {
        var address = element.at("address").clone();
        
        var controller = new String(address);
        this.addController(controller);
        
        List.fn.addElement.call(this, element);
    }
});

module.exports = PanesList;
