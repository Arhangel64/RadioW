"use strict";
(function navigationPanel_js() {
    var moduleName = "models/navigationPanel";
    
    var defineArray = [];
    defineArray.push("models/list");
    defineArray.push("models/link");
    defineArray.push("views/string")
    
    define(moduleName, defineArray, function navigationPanel_module() {
        var List = require("models/list");
        var ModelLink = require("models/link");
        var ViewLink = require("views/string");
        
        var NavigationPanel = List.inherit({
            "className": "NavigationPanel",
            "constructor": function(addr) {
                List.fn.constructor.call(this, addr);
                
                this.on("clear", this._onClear, this);
                this.on("newElement", this._onNewElement, this);
                this.on("newModel", this._onNewModel, this)
            },
            "addView": function(view) {
                List.fn.addView.call(this, view);
                
                for (var i = 0; i < this._models.length; ++i) {
                    var link = new ViewLink();
                    this._models[i].addView(link)
                    view.append(link, 0, view._c.length, 1, 1);
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
                
                var model = new ModelLink(address);
                this.addModel(model);
            },
            "_onNewModel": function(model) {
                if (this._views.length) {
                
                    for (var i = 0; i < this._views.length; ++i) {
                        var view = new ViewLink();
                        model.addView(view)
                        this._views[i].append(view, 0, this._views[i]._c.length, 1, 1);
                    }
                }
            }
        });
        
        return NavigationPanel;
    });
})();