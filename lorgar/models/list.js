"use strict";
(function list_js() {
    var moduleName = "models/list";
    
    var defineArray = [];
    defineArray.push("models/model");
    
    define(moduleName, defineArray, function list_module() {
        var Model = require("models/model");
        
        var List = Model.inherit({
            "className": "List",
            "constructor": function(addr) {
                Model.fn.constructor.call(this, addr);
                
                this._data = undefined;
                
                this.addHandler("get");
            },
            "destructor": function() {
                if (this._data) {
                    this._data.destructor();
                }
                
                Model.fn.destructor.call(this);
            },
            "_h_get": function(ev) {
                var data = ev.getData();
                
                this._data = data.at("data").clone();
                
                for (var i = 0; i < this._views.length; ++i) {
                    this._views[i].data(this._data);
                }
                
                this.trigger("got", this._data);
            },
            "_h_push": function(ev) {
                var data = ev.getData();
                
                var element = data.at("data").clone();
                this._data.push(element);
                
                for (var i = 0; i < this._views.length; ++i) {
                    this._views[i].push(element);
                }
            }
        });
        
        return List;
    });
})();