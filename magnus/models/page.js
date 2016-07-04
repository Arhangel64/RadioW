"use strict";

var List = require("./list");
var Vocabulary = require("../lib/wType/vocabulary");
var Address = require("../lib/wType/address");
var String = require("../lib/wType/string");
var UInt64 = require("../lib/wType/uint64");

var Page = List.inherit({
    "className": "Page",
    "constructor": function(address) {
        List.fn.constructor.call(this, address);
    },
    "addItem": function(model, row, col, rowspan, colspan, aligment) {
        List.fn.addModel.call(this, model);
        
        var vc = new Vocabulary();
        vc.insert("type", new String(model.className));
        vc.insert("address", model.getAddress());
        vc.insert("row", new UInt64(row || 0));
        vc.insert("col", new UInt64(col || 0));
        vc.insert("rowspan", new UInt64(rowspan || 1));
        vc.insert("colspan", new UInt64(colspan || 1));
        vc.insert("aligment", new UInt64(aligment || 1));
        
        this.push(vc);
    }
});

Page.Aligment = {
    "LeftTop": 1,
    "LeftCenter": 4,
    "LeftBottom": 7,
    "CenterTop": 2,
    "CenterCenter": 5,
    "CenterBottom": 8,
    "RightTop": 3,
    "RightCenter": 6,
    "RightBottom": 9
};

module.exports = Page;