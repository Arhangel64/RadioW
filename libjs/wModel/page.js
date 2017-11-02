"use strict";

var Model = require("./model");
var AbstractMap = require("../wContainer/abstractmap");
var Vocabulary = require("../wType/vocabulary");
var Vector = require("../wType/vector");
var Address = require("../wType/address");
var String = require("../wType/string");
var UInt64 = require("../wType/uint64");

var ContentMap = AbstractMap.template(Address, Vocabulary);

var Page = Model.inherit({
    "className": "Page",
    "constructor": function(address, name) {
        Model.fn.constructor.call(this, address);
        
        this._data = new ContentMap(true);
        this.name = name.toLowerCase();
        this.urlAddress = [this.name];
        this._hasParentReporter = false;
        this._pr = undefined;
        this._childPages = {};
        
        this.addHandler("get");
        this.addHandler("ping");
        
        this.addProperty("backgroundColor", "mainColor");
        this.addProperty("color", "mainFontColor");
    },
    "destructor": function() {
        this._data.destructor();
        
        Model.fn.destructor.call(this);
    },
    "addItem": function(model, row, col, rowspan, colspan, aligment, viewType, viewOptions) {
        Model.fn.addModel.call(this, model);
        
        var vc = new Vocabulary();
        viewOptions = viewOptions || new Vocabulary();
        viewType = viewType || new UInt64(Page.getModelTypeId(model))
        
        vc.insert("type", new UInt64(Page.getModelTypeId(model)));
        vc.insert("row", new UInt64(row || 0));
        vc.insert("col", new UInt64(col || 0));
        vc.insert("rowspan", new UInt64(rowspan || 1));
        vc.insert("colspan", new UInt64(colspan || 1));
        vc.insert("aligment", new UInt64(aligment || Page.Aligment.LeftTop));
        vc.insert("viewOptions", viewOptions);
        vc.insert("viewType", viewType);
        
        this._data.insert(model.getAddress(), vc);
        
        var evc = vc.clone();
        evc.insert("address", model.getAddress());
        
        this.broadcast(evc, "addItem");
    },
    "addPage": function(page) {
        this.addModel(page);
        var addr = this.urlAddress.slice();
        addr.push(page.name);
        page.setUrlAddress(addr)
        page.on("newPage", this._onNewPage, this);
        page.on("removePage", this._onRemovePage, this);
        this._childPages[page.name] = page;
        
        this.trigger("newPage", page);
    },
    "clear": function() {
        this._data.clear();
        
        this.broadcast(new Vocabulary(), "clear");
    },
    "getChildPage": function(name) {
        return this._childPages[name];
    },
    "_h_get": function(ev) {
        var data = new Vocabulary();
        
        var vector = new Vector();
        var itr = this._data.begin();
        var end = this._data.end();
        
        for (; !itr["=="](end); itr["++"]()) {
            var pair = itr["*"]();
            var vc = pair.second.clone();
            
            vc.insert("address", pair.first.clone());
            vector.push(vc);
        }
        
        data.insert("name", new String(this.name));
        data.insert("data", vector);
        this.response(data, "get", ev);
    },
    "_h_ping": function(ev) {
        this.trigger("serviceMessage", "page " + this._address.toString() + " got pinged", 0);
    },
    "_h_subscribe": function(ev) {
        Model.fn._h_subscribe.call(this, ev);
        
        this._h_get(ev);
    },
    "_onNewPage": function(page) {
        this.trigger("newPage", page);
    },
    "_onRemovePage": function(page) {
        this.trigger("removePage", page);
    },
    "removeItem": function(model) {
        Model.fn.removeModel.call(this, model);
        var addr = model.getAddress();
        
        var itr = this._data.find(addr);
        this._data.erase(itr);
        
        var vc = new Vocabulary();
        vc.insert("address", addr);
        
        this.broadcast(vc, "removeItem");
    },
    "removePage": function(page) {
        Model.fn.removeModel.call(this, page);
        
        delete this._childPages[page.name];
        page.off("newPage", this._onNewPage, this);
        page.off("removePage", this._onRemovePage, this);
        
        this.trigger("removePage", page);
    },
    "setParentReporter": function(pr) {
        if (this._hasParentReporter) {
            throw new Error("An attempt to set parent reporter to page while another one already exists");
        }
        this._pr = pr;
        this._hasParentReporter = true;
    },
    "setUrlAddress": function(address) {
        this.urlAddress = address;
    },
    "unsetParentReporter": function() {
        if (!this._hasParentReporter) {
            throw new Error("An attempt to unset parent reporter from page which doesn't have one");
        }
        delete this._pr;
        this._hasParentReporter = false;
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
