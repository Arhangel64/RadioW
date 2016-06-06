"use strict";
(function link_js() {
    var moduleName = "models/link";
    
    var defineArray = [];
    defineArray.push("models/model");
    
    define(moduleName, defineArray, function link_module() {
        var Model = require("models/model");
        
        var Link = Model.inherit({
            "className": "Link",
            "constructor": function(addr) {
                Model.fn.constructor.call(this, addr);
                
                this._targetAddress = undefined;
                this._text = undefined;
                
                this.addHandler("get");
            },
            "_h_get": function(ev) {
                var data = ev.getData();
                
                this._text = data.at("text").toString();
                this._targetAddress = data.at("targetAddress").clone();
                
                for (var i = 0; i < this._views.length; ++i) {
                    this._views[i].data(this._text);
                }
            }
        });
        
        return Link;
    });
})();