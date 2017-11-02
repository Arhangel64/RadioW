"use strict";

var Model = require("./model");
var Page = require("./page");
var ModelString = require("./string");
var Vocabulary = require("../wType/vocabulary");
var Address = require("../wType/address");
var String = require("../wType/string");
var Event = require("../wType/event");
var AbstractMap = require("../wContainer/abstractmap");

var AddressMap = AbstractMap.template(Address, String);

var PageStorage = Model.inherit({
    "className": "PageStorage",
    "constructor": function(addr, rootPage, pr) {
        Model.fn.constructor.call(this, addr);
        
        this._urls = {};
        this._specialPages = {};
        this._rMap = new AddressMap(true);
        this._root = rootPage;
        this._pr = pr;
        
        this._initRootPage();
        this._initNotFoundPage();
        
        this.addHandler("getPageAddress");
        this.addHandler("getPageName");
    },
    "destructor": function() {
        this._rMap.destructor();
        
        Model.fn.destructor.call(this);
    },
    "getPageByUrl": function(url) {
        var addr = url.split("/");
        var page = this._root;
        for (var i = 0; i < addr.length; ++i) {
            if (addr[i] !== "") {
                page = page.getChildPage(addr[i]);
                if (page === undefined) {
                    return this._specialPages.notFound;
                }
            }
        }
        return page;
    },
    "hasPage": function(name) {
        return this.getPageByUrl(name) !== this._specialPages.notFound
    },
    "_h_getPageAddress": function(ev) {
        var data = ev.getData();
        
        var page = this.getPageByUrl(data.at("url").valueOf());
        
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
        var address = data.at("address");
        
        var evp = new Event(address["+"](new Address(["ping"])));
        this._dp.pass(evp);
        evp.destructor();
        
        var itr = this._rMap.find(address);
        var vc = new Vocabulary();
        if (itr["=="](this._rMap.end())) {
            vc.insert("name", new String("notFound"));
        } else {
            vc.insert("name", itr["*"]().second.clone());
        }
        this.response(vc, "pageName", ev);
    },
    "_initNotFoundPage": function() {
        var nf = new Page(this._address["+"](new Address(["special", "notFound"])), "Not found");
        this._specialPages["notFound"] = nf;
        this.addModel(nf);
        var msg = new ModelString(nf._address["+"](new Address(["errorMessage"])), "Error: page not found");
        nf.addItem(msg, 0, 0, 1, 1);
    },
    "_initRootPage": function() {
        this.addModel(this._root);
        
        this._rMap.insert(this._root.getAddress(), new String("/"));
        this._root.on("newPage", this._onNewPage, this);
        this._root.on("removePage", this._onRemovePage, this);
    },
    "_onNewPage": function(page) {
        var addr = page.urlAddress.join("/").slice(this._root.name.length);
        this.trigger("serviceMessage", "Adding new page: " + addr);
        this.trigger("serviceMessage", "Page address: " + page.getAddress().toString());
        
        if (this._urls[addr]) {
            throw new Error("An attempt to add page with an existing url");
        }
        this._urls[addr] = page;
        this._rMap.insert(page.getAddress(), new String(addr));
        page.setParentReporter(this._pr);
    },
    "_onRemovePage": function(page) {
        var addr = page.urlAddress.join("/").slice(this._root.name.length);
        this.trigger("serviceMessage", "Removing page: " + addr);
        this.trigger("serviceMessage", "Page address: " + page.getAddress().toString());
        
        if (!this._urls[addr]) {
            throw new Error("An attempt to remove a non existing page");
        }
        delete this._urls[addr];
        var itr = this._rMap.find(page.getAddress());
        this._rMap.erase(itr);
        page.unsetParentReporter();
    }
});

module.exports = PageStorage;
