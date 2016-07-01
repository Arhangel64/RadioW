"use strict";
(function theme_js() {
    var moduleName = "models/theme";
    
    var defineArray = [];
    defineArray.push("models/model");
    
    define(moduleName, defineArray, function theme_module() {
        var Model = require("models/model");
        
        var Theme = Model.inherit({
            "className": "Theme",
            "constructor": function(addr) {
                Model.fn.constructor.call(this, addr);
                
                this._data = {};
                this._deferredReady = this._onReady.bind(this);
                
                this.addHandler("get");
            },
            "_h_get": function(ev) {
                var pairs = ev.getData();
                this.trigger("clear");
                
                var data = pairs.at("data");
                var keys = data.getKeys();
                for (var i = 0; i < keys.length; ++i) {
                    this._data[keys[i]] = data.at(keys[i]).valueOf()
                }
                
                setTimeout(this._deferredReady, 1);
            },
            "_onReady": function() {
                this.trigger("ready", this._data);
            }
        });
        
        return Theme;
    });
})();