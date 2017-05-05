"use strict";
var List = require("./list");
var String = require("./string");
var PanesList = require("./panesList");

var ViewPanesList = require("../../views/panesList");

var Vocabulary = require("../wType/vocabulary");
var Address = require("../wType/address");

var Page = List.inherit({
    "className": "Page",
    "constructor": function(addr) {
        List.fn.constructor.call(this, addr);
        
        this.elements = [];
    },
    "addElement": function(element) {
        List.fn.addElement.call(this, element);
        
        var type = element.at("type").valueOf();
        var address = element.at("address").clone();
        var col = element.at("col").valueOf();
        var row = element.at("row").valueOf();
        var colspan = element.at("colspan").valueOf();
        var rowspan = element.at("rowspan").valueOf();
        var aligment = element.at("aligment").valueOf();
        var viewType = element.at("viewType").valueOf();
        var opts = Page.deserializeOptions(element.at("viewOptions"));
        
        var controller = Page.createByType(type, address);
        this.addController(controller);
        
        var el = {
            type: type,
            col: col,
            row: row,
            colspan: colspan,
            rowspan: rowspan,
            aligment: aligment,
            viewType: viewType,
            viewOptions: opts,
            controller: controller
        }
        this.elements.push(el);
        this.trigger("newPageElement", el);
    },
    "clear": function() {
        this.elements = [];
        
        List.fn.clear.call(this);
    }
});


Page.deserializeOptions = function(vc) {
    var opts = Object.create(null);
    var keys = vc.getKeys();
    
    for (var i = 0; i < keys.length; ++i) {
        var value = vc.at(keys[i]);
        
        if (value instanceof Vocabulary) {
            value = this.deserializeOptions(value);
        } else if(value instanceof Address) {
            value = value.clone();
        } else {
            value = value.valueOf();
        }
        opts[keys[i]] = value;
    }
    
    return opts;
}

module.exports = Page;
