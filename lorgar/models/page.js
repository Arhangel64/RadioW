"use strict";
(function page_js() {
    var moduleName = "models/page";
    
    var defineArray = [];
    defineArray.push("models/list");
    
    defineArray.push("models/string");
    
    defineArray.push("views/string");
    
    define(moduleName, defineArray, function page_module() {
        var List = require("models/list");
        
        var ModelString = require("models/string");
        
        var ViewString = require("views/string");
        
        var Page = List.inherit({
            "className": "Page",
            "constructor": function(addr) {
                List.fn.constructor.call(this, addr);
            },
            "addView": function(view) {
                List.fn.addView.call(this, view);
                
                if (this._data) {
                    var size = this._data.size();
                    for (var i = 0; i < size; ++i) {
                        var vc = this._data.at(i);
                        this._addItem(vc, view);
                    }
                }
            },
            "_addItem": function(vc, myView) {
                var col = vc.at("col").valueOf();
                var row = vc.at("row").valueOf();
                var colspan = vc.at("colspan").valueOf();
                var rowspan = vc.at("rowspan").valueOf();
                var type = vc.at("type").valueOf();
                var address = vc.at("address").clone();
                var View;
                var model;
                var view;
                
                switch (type) {
                    case "String":
                        model = new ModelString(address);
                        View = ViewString;
                        
                        break;
                    default:
                        console.warn("Unsupported view element on the page");
                }
                
                
                if (!myView) {
                    for (var i = 0; i < this._views.length; ++i) {
                        view = new View()
                        this._views[i].append(view, row, col, rowspan, colspan);
                        model.addView(view);
                    }
                } else {
                    view = new View()
                    myView.append(view, row, col, rowspan, colspan);
                    model.addView(view);
                }
                
                this.addModel(model);
            },
            "_h_get": function(ev) {
                List.fn._h_get.call(this, ev);
                
                var size = this._data.size();
                for (var i = 0; i < size; ++i) {
                    var vc = this._data.at(i);
                    this._addItem(vc);
                }
            }
        });
        
        return Page;
    });
})();