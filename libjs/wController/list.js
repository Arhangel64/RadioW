"use strict";
var Controller = require("./controller");

var List = Controller.inherit({
    "className": "List",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this._data = undefined;
        
        this.addHandler("get");
        this.addHandler("push");
    },
    "destructor": function() {
        if (this._data) {
            this._data.destructor();
        }
        
        Controller.fn.destructor.call(this);
    },
    "addElement": function(element) {
        this.trigger("newElement", element);
    },
    "clear": function() {
        if (this._data) {
            this._data.destructor();
        }
        
        while (this._controllers.length) {
            var controller = this._controllers[this._controllers.length - 1]
            this._removeController(controller);
            controller.destructor();
        }
        for (var i = 0; i < this._views.length; ++i) {
            this._views[i].clear();
        }
        
        this.trigger("clear");
    },
    "_h_get": function(ev) {
        this.clear();
        
        var data = ev.getData();
        this._data = data.at("data").clone();
        var size = this._data.size();
        for (var i = 0; i < size; ++i) {
            this.addElement(this._data.at(i));
        }
    },
    "_h_push": function(ev) {
        var data = ev.getData();
        
        var element = data.at("data").clone();
        this._data.push(element);
        this.addElement(element);
    }
});

module.exports = List;
