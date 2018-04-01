"use strict";

var Controller = require("./controller");
var String = require("./string");

var Vocabulary = require("../wType/vocabulary");
var Address = require("../wType/address");

var AbstractMap = require("../wContainer/abstractmap");
var ContentMap = AbstractMap.template(Address, Object);

var Page = Controller.inherit({
    "className": "Page",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this.data = new ContentMap(false);
        
        this.addHandler("get");
        this.addHandler("addItem");
        this.addHandler("removeItem");
        this.addHandler("clear");
        
        this.elements = [];
    },
    "destructor": function() {
        this.data.destructor();
        
        Controller.fn.destructor.call(this);
    },
    "addItem": function(element) {
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
        this.data.insert(address, el);
        this.trigger("addItem", address, el);
    },
    "clear": function() {
        this.data.clear();
        this.trigger("clear");
        this.clearChildren();
    },
    "_h_clear": function(ev) {
        this.clear();
    },
    "_h_get": function(ev) {
        this.clear();
        
        var data = ev.getData().at("data");
        var size = data.length();
        for (var i = 0; i < size; ++i) {
            this.addItem(data.at(i).clone());
        }
    },
    "_h_addItem": function(ev) {
        var data = ev.getData().clone();
        
        this.addItem(data);
    },
    "_h_removeItem": function(ev) {
        var data = ev.getData();
        
        var address = data.at("address").clone();
        this.removeItem(address);
    },
    "removeItem": function(address) {
        var itr = this.data.find(address);
        var pair = itr["*"]();
        this.data.erase(itr);
        
        this.trigger("removeItem", pair.first);
        
        this.removeController(pair.second.controller);
        pair.second.controller.destructor();
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
