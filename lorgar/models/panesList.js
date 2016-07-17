"use strict";
(function panesList_js() {
    var moduleName = "models/panesList";
    
    var defineArray = [];
    defineArray.push("models/list");
    
    defineArray.push("models/string");
    defineArray.push("views/string");
    
    define(moduleName, defineArray, function panesList() {
        var List = require("models/list");
        
        var ModelString = require("models/string");
        var ViewString = require("views/string");
        
        var PanesList = List.inherit({
            "className": "PanesList",
            "constructor": function PanesListModel(addr) {
                List.fn.constructor.call(this, addr);
                
                this.on("clear", this._onClear, this);
                this.on("newElement", this._onNewElement, this);
                this.on("newModel", this._onNewModel, this)
            },
            "addView": function(view) {
                List.fn.addView.call(this, view);
                
                for (var i = 0; i < this._models.length; ++i) {
                    var el = new ViewString();
                    this._models[i].addView(el)
                    view.append(el);
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
                var address = vc.at("address").clone();
                
                var model = new ModelString(address);
                this.addModel(model);
            },
            "_onNewModel": function(model) {
                if (this._views.length) {
                    var index = this._models.indexOf(model)
                
                    for (var i = 0; i < this._views.length; ++i) {
                        var view = new ViewString({singleLine: true});
                        model.addView(view)
                        this._views[i].append(view);
                    }
                }
            }
        });
        
        return PanesList;
    });
})();