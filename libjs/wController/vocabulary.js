"use strict";
var Controller = require("./controller");
var WVocabulary = require("../wType/vocabulary");

var Vocabulary = Controller.inherit({
    "className": "Vocabulary",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this.data = new WVocabulary();
        
        this.addHandler("get");
        this.addHandler("change");
        this.addHandler("clear");
    },
    "destructor": function() {
        this.data.destructor();
        
        Controller.fn.destructor.call(this);
    },
    "addElement": function(key, element) {
        this.data.insert(key, element);
        this.trigger("newElement", key, element);
    },
    "removeElement": function(key) {
        this.data.erase(key);
        this.trigger("removeElement", key);
    },
    "clear": function() {
        this.data.clear();
        
        while (this._controllers.length) {
            var controller = this._controllers[this._controllers.length - 1]
            this._removeController(controller);
            controller.destructor();
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
            key = erase.at(i).toString();;
            this.removeElement(key);
        }
        
        var keys = insert.getKeys();
        for (var j = 0; j < keys.length; ++j) {
            key = keys[i];
            this.addElement(key, insert.at(key).clone());
        }
        this.trigger("change", data.clone());
    },
    "_h_clear": function(ev) {
        this.clear();
    },
    "_h_get": function(ev) {
        this.clear();
        
        var data = ev.getData().at("data");
        var keys = data.getKeys();
        for (var i = 0; i < keys.length; ++i) {
            var key = keys[i];
            this.addElement(key, data.at(key).clone());
        }
        this.trigger("data");
    }
});

module.exports = Vocabulary;
