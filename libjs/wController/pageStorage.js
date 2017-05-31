"use strict";
var Controller = require("./controller");
var Vocabulary = require("../wType/vocabulary");
var String = require("../wType/string");

var PageStorage = Controller.inherit({
    "className": "PageStorage",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this.addHandler("pageAddress");
        this.addHandler("pageName");
    },
    "getPageAddress": function(url) {
        var vc = new Vocabulary();
        
        vc.insert("url", new String(url));
        this.send(vc, "getPageAddress");
    },
    "getPageName": function(address) {
        var vc = new Vocabulary();
        
        vc.insert("address", address.clone());
        this.send(vc, "getPageName");
    },
    "_h_pageAddress": function(ev) {
        var data = ev.getData();
        
        this.trigger("pageAddress", data.at("address").clone());
    },
    "_h_pageName": function(ev) {
        var data = ev.getData();
        
        this.trigger("pageName", data.at("name").toString());
    }
});

module.exports = PageStorage;
