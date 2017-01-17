"use strict";
var Controller = require("./controller");

var Vocabulary = Controller.inherit({
    "className": "Vocabulary",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this._data = undefined;
        
        this.addHandler("get");
        this.addHandler("change");
        this.addHandler("clear");
    },
    "destructor": function() {
        if (this._data) {
            this._data.destructor();
        }
        
        Controller.fn.destructor.call(this);
    },
    "addElement": function(key, element) {
        this.trigger("newElement", key, element);
    },
    "removeElement": function(key) {
        this.trigger("removeElement", key);
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
    "_h_change": function(ev) {
        var key;
        var data = ev.getData();
        
        var erase = data.at("erase");
        var insert = data.at("insert");
        
        var eSize = erase.size();
        for (var i = 0; i < eSize; ++i) {
            key = erase.at(i);
            this.removeElement(key);
            this._data.erase(key);
        }
        
        var keys = insert.getKeys();
        for (var j = 0; j < keys.length; ++j) {
            key = kets[i];
            this._data.isert(key, insert.at(key).clone());
            this.newElement(key, this._data.at(key));
        }
    },
    "_h_clear": function(ev) {
        this.clear();
    },
    "_h_get": function(ev) {
        this.clear();
        
        var data = ev.getData();
        this._data = data.at("data").clone();
        var keys = this._data.getKeys();
        var size = keys.length;
        for (var i = 0; i < size; ++i) {
            var key = keys[i];
            this.addElement(key, this._data.at(key));
        }
    }
});

module.exports = Vocabulary;
