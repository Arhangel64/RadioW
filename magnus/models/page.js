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
    "addItem": function(model, row, col, rowspan, colspan) {
        List.fn.addModel.call(this, model);
        
        var vc = new Vocabulary();
        vc.insert("type", new String(model.className));
        vc.insert("address", model.getAddress());
        vc.insert("row", new UInt64(row));
        vc.insert("col", new UInt64(col));
        vc.insert("rowspan", new UInt64(rowspan));
        vc.insert("colspan", new UInt64(colspan));
        
        this.push(vc);
    }
});

module.exports = Page;