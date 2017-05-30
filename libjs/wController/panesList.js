"use strict";
var List = require("./list");
var Vocabulary = require("./vocabulary");

var Address = require("../wType/address");

var PanesList = List.inherit({
    "className": "PanesList",
    "constructor": function PanesListModel(addr) {
        List.fn.constructor.call(this, addr);
    },
    "addElement": function(element) {
        var controller = new Vocabulary(this._pairAddress["+"](new Address([element.toString()])));
        this.addController(controller);
        
        List.fn.addElement.call(this, element);
    }
});

module.exports = PanesList;
