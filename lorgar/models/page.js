"use strict";
(function page_js() {
    var moduleName = "models/page";
    
    var defineArray = [];
    defineArray.push("models/list");
    defineArray.push("models/string");
    defineArray.push("models/panesList");
    defineArray.push("views/string");
    defineArray.push("views/panesList");
    defineArray.push("lib/wType/vocabulary");
    defineArray.push("lib/wType/address");
    
    define(moduleName, defineArray, function page_module() {
        var List = require("models/list");
        var ModelString = require("models/string");
        var ModelPanesList = require("models/panesList");
        var ViewString = require("views/string");
        var ViewPanesList = require("views/panesList");
        var Vocabulary = require("lib/wType/vocabulary");
        var Address = require("lib/wType/address");
        
        var Page = List.inherit({
            "className": "Page",
            "constructor": function(addr) {
                List.fn.constructor.call(this, addr);
                
                this.on("clear", this._onClear, this);
                this.on("newElement", this._onNewElement, this);
                this.on("newModel", this._onNewModel, this)
            },
            "addView": function(view) {
                List.fn.addView.call(this, view);
                
                for (var i = 0; i < this._models.length; ++i) {
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
                    this._models[i].addView(elem)
                    view.append(elem, row, col, rowspan, colspan, aligment);
                }
            },
            "_onClear": function() {
                for (var i = 0; i < this._models.length; ++i) {
                    this._models[i].destructor();
                }
                for (i = 0; i < this._views.length; ++i) {
                    this._views[i].clear();
                }
            },
            "_onNewElement": function(vc) {
                var type = vc.at("type").valueOf();
                var address = vc.at("address").clone();
                
                
                if (!collection[type]) {
                    throw new Error("Unsupported view element on the page");
                }
                var Model = collection[type].model;
                var model = new Model(address);
                this.addModel(model);
            },
            "_onNewModel": function(model) {
                if (this._views.length) {
                    var index = this._models.indexOf(model)
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
                        model.addView(view)
                        this._views[i].append(view, row, col, rowspan, colspan, aligment);
                    }
                }
            }
        });
        
        var collection = {
            "String": {model: ModelString, view: ViewString},
            "PanesList": {model: ModelPanesList, view: ViewPanesList}
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
        
        return Page;
    });
})();