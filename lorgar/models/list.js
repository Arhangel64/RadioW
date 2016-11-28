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
                this.addHandler("push");
            },
            "destructor": function() {
                if (this._data) {
                    this._data.destructor();
                }
                
                Model.fn.destructor.call(this);
            },
            "_h_get": function(ev) {
                var data = ev.getData();
                if (this._data) {
                    this._data.destructor();
                }
                this.trigger("clear");
                
                this._data = data.at("data").clone();
                var size = this._data.size();
                for (var i = 0; i < size; ++i) {
                    this.trigger("newElement", this._data.at(i).clone());
                }
            },
            "_h_push": function(ev) {
                var data = ev.getData();
                
                var element = data.at("data").clone();
                this._data.push(element);
                this.trigger("newElement", element.clone());
            }
        });
        
        return List;
    });
})();
