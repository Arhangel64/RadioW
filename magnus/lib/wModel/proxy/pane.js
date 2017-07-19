"use strict";

var MVocabulary = require("./vocabulary");

var Address = require("../../wType/address");
var Boolean = require("../../wType/boolean");

var Pane = MVocabulary.inherit({
    "className": "Pane",
    "constructor": function(address, controllerAddress, socket) {
        MVocabulary.fn.constructor.call(this, address, controllerAddress, socket);
        
        if (this.constructor.pageAddress) {
            this.hasPageLink = true;
            
            var id = address.back();
            this._pageLink = this.constructor.pageAddress["+"](new Address([id.toString()]));
        }
    },
    "_getAllData": function() {
        var vc = this.controller.data.clone();
        
        vc.insert("hasPageLink", new Boolean(this.hasPageLink));
        if (this.hasPageLink) {
            vc.insert("pageLink", this._pageLink.clone());
        }
        
        return vc;
    }
});

module.exports = Pane;
