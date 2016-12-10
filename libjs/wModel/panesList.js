"use strict";

var List = require("./list");
var Vocabulary = require("../wType/vocabulary");

var PanesList = List.inherit({
    "className": "PanesList",
    "constructor": function(address) {
        List.fn.constructor.call(this, address);
    },
    "addItem": function(model) {
        List.fn.addModel.call(this, model);
        
        var vc = new Vocabulary();
        vc.insert("address", model.getAddress());
        
        this.push(vc);
    }
});

module.exports = PanesList;
