"use strict";
(function string_js() {
    var moduleName = "models/string";
    
    var defineArray = [];
    defineArray.push("models/model");
    
    define(moduleName, defineArray, function string_module() {
        var Model = require("models/model");
        
        var ModelString = Model.inherit({
            "className": "String",
            "constructor": function(addr) {
                Model.fn.constructor.call(this, addr);
                
                this._data = "";
                
                this.addHandler("get");
            },
            "_h_get": function(ev) {
                var data = ev.getData();
                
                this._data = data.at("data").toString();
                this.trigger("got", this._data);
            }
        });
        
        return ModelString;
    });
})();