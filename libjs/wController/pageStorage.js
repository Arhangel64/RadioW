"use strict";
var Controller = require("./controller");
var Vocabulary = require("../wType/vocabulary");
var String = require("../wType/string");

var PageStorage = Controller.inherit({
    "className": "PageStorage",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this.addHandler("pageAddress");
    },
    "getPageAddress": function(url) {
        var vc = new Vocabulary();
        
        vc.insert("url", new String(url));
        this.send(vc, "getPageAddress");
    },
    "_h_pageAddress": function(ev) {
        var data = ev.getData();
        
        this.trigger("pageAddress", data.at("address").clone());
    }
});

module.exports = PageStorage;
