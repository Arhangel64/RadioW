"use strict";

var Controller = require("./controller");
var AbstractOrder = require("../wContainer/abstractorder");

var Uint64 = require("../wType/uint64");
var Vocabulary = require("../wType/vocabulary");
var Boolean = require("../wType/boolean");
var String = require("../wType/string");

var IdOrder = AbstractOrder.template(Uint64);

var Catalogue = Controller.inherit({
    "className": "Catalogue",
    "constructor": function(addr, options) {
        Controller.fn.constructor.call(this, addr);
        
        this._hasSorting = false;
        this._hasFilter = false;
        this.data = new IdOrder(true);
        
        if (options.sorting) {
            this._hasSorting = true;
            this._sorting = new Vocabulary();
            this._sorting.insert("ascending", new Boolean(options.sorting.ascending));
            this._sorting.insert("field", new String(options.sorting.field));
        }
        if (options.filter instanceof Vocabulary) {
            this._hasFilter = true;
            this._filter = options.filter.clone();
        }
        
        this.addHandler("get");
        this.addHandler("addElement");
        this.addHandler("removeElement");
    },
    "destructor": function() {
        this.data.destructor();
        
        if (this._hasSorting) {
            this._sorting.destructor();
        }
        if (this._hasFilter) {
            this._filter.destructor();
        }
        
        Controller.fn.destructor.call(this);
    },
    "addElement": function(element, before) {
        if (before === undefined) {
            this.data.push_back(element);
        } else {
            this.data.insert(element, before);
        }
        
        this.trigger("addElement", element, before);
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
    "_createSubscriptionVC": function() {
        var vc = Controller.fn._createSubscriptionVC.call(this);
        var p = new Vocabulary();
        
        if (this._hasSorting) {
            p.insert("sorting", this._sorting.clone());
        }
        if (this._hasFilter) {
            p.insert("filter", this._filter.clone());
        }
        
        vc.insert("params", p);
        return vc;
    },
    "_h_addElement": function(ev) {
        var data = ev.getData();
        var id = data.at("id").clone();
        var before = undefined;
        if (data.has("before")) {
            before = data.at("before").clone();
        }
        
        this.addElement(id, before);
    },
    "_h_get": function(ev) {
        this.clear();
        
        var data = ev.getData().at("data");
        var size = data.size();
        for (var i = 0; i < size; ++i) {
            this.addElement(data.at(i).clone());
        }
        this.initialized = true;
        this.trigger("data");
    },
    "_h_removeElement": function(ev) {
        var data = ev.getData();
        
        this.removeElement(data.at("id").clone());
    },
    "removeElement": function(id) {
        this.data.erase(id);
        this.trigger("removeElement", id);
    }
});

module.exports = Catalogue;
