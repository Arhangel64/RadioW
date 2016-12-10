"use strict";
var List = require("./list");
var String = require("./string");
var PanesList = require("./panesList");

var ViewString = require("../../views/string");
var ViewPanesList = require("../../views/panesList");

var Vocabulary = require("../wType/vocabulary");
var Address = require("../wType/address");

var Page = List.inherit({
    "className": "Page",
    "constructor": function(addr) {
        List.fn.constructor.call(this, addr);
    },
    "addController": function(ctrl) {
        List.fn.addController.call(this, ctrl);
        
        if (this._views.length) {
            var index = this._controllers.indexOf(ctrl);
            var vc = this._data.at(index);
        
            var col = vc.at("col").valueOf();
            var row = vc.at("row").valueOf();
            var colspan = vc.at("colspan").valueOf();
            var rowspan = vc.at("rowspan").valueOf();
            var aligment = vc.at("aligment").valueOf();
            var type = vc.at("type").valueOf();
            var opts = Page.deserializeOptions(vc.at("viewOptions"));
            
            if (!collection[type]) {
                throw new Error("Unsupported view element on the page");
            }
            var View = collection[type].view;
        
            for (var i = 0; i < this._views.length; ++i) {
                var view = new View(opts);
                ctrl.addView(view)
                this._views[i].append(view, row, col, rowspan, colspan, aligment);
            }
        }
    },
    "addElement": function(element) {
        var type = element.at("type").valueOf();
        var address = element.at("address").clone();
        
        
        if (!collection[type]) {
            throw new Error("Unsupported view element on the page");
        }
        var Controller = collection[type].controller;
        var controller = new Controller(address);
        this.addController(controller);
        
        List.fn.addElement.call(this, element);
    },
    "addView": function(view) {
        List.fn.addView.call(this, view);
        
        for (var i = 0; i < this._controllers.length; ++i) {
            var vc = this._data.at(i);
            
            var col = vc.at("col").valueOf();
            var row = vc.at("row").valueOf();
            var colspan = vc.at("colspan").valueOf();
            var rowspan = vc.at("rowspan").valueOf();
            var aligment = vc.at("aligment").valueOf();
            var type = vc.at("type").valueOf();
            var opts = Page.deserializeOptions(vc.at("viewOptions"));
            
            if (!collection[type]) {
                throw new Error("Unsupported view element on the page");
            }
            var Element = collection[type].view;
            var elem = new Element(opts);
            this._controllers[i].addView(elem)
            view.append(elem, row, col, rowspan, colspan, aligment);
        }
    }
});

var collection = {
    "String": {controller: String, view: ViewString},
    "PanesList": {controller: PanesList, view: ViewPanesList}
};


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
        opts[key[i]] = value;
    }
    
    return opts;
}

module.exports = Page;
