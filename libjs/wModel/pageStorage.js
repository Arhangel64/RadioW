"use strict";

var Model = require("./model");
var Page = require("./page");
var ModelString = require("./string");
var Vocabulary = require("../wType/vocabulary");
var Address = require("../wType/address");
var String = require("../wType/string");
var AbstractMap = require("../wContainer/abstractmap");

var AddressMap = AbstractMap.template(Address, String);

var PageStorage = Model.inherit({
    "className": "PageStorage",
    "constructor": function(addr) {
        Model.fn.constructor.call(this, addr);
        
        this._urls = {};
        this._specialPages = {};
        this._rMap = new AddressMap(true);
        
        this._initNotFoundPage();
        
        this.addHandler("getPageAddress");
        this.addHandler("getPageName");
    },
    "destructor": function() {
        this._rMap.destructor();
        
        Model.fn.destructor.call(this);
    },
    "addPage": function(page, urls) {
        if (!(urls instanceof Array)) {
            throw new Error("To add page you need to pass an array of urls");
        }
        for (var i = 0; i < urls.length; ++i) {
            if (this._urls[urls[i]]) {
                throw new Error("An attempt to add page with an existing url");
            }
            
            this._urls[urls[i]] = page;
        }
        this._rMap.insert(page.getAddress(), new String(urls[0]));
        this.addModel(page)
    },
    "hasPage": function(name) {
        return this._urls[name] !== undefined;
    },
    "_h_getPageAddress": function(ev) {
        var data = ev.getData();
        
        var page = this._urls[data.at("url").valueOf()];
        
        var vc = new Vocabulary();
        if (page) {
            vc.insert("address", page.getAddress());
        } else {
            vc.insert("address", this._specialPages.notFound.getAddress());
        }
        
        this.response(vc, "pageAddress", ev);
    },
    "_h_getPageName": function(ev) {
        var data = ev.getData();
        
        var itr = this._rMap.find(data.at("address"));
        var vc = new Vocabulary();
        if (itr["=="](this._rMap.end())) {
            vc.insert("name", new String("notFound"));
        } else {
            vc.insert("name", itr["*"]().second.clone());
        }
        this.response(vc, "pageName", ev);
    },
    "_initNotFoundPage": function() {
        var nf = new Page(this._address["+"](new Address(["special", "notFound"])));
        this._specialPages["notFound"] = nf;
        this.addModel(nf);
        var msg = new ModelString(nf._address["+"](new Address(["errorMessage"])), "Error: page not found");
        nf.addItem(msg, 0, 0, 1, 1);
    }
});

module.exports = PageStorage;
