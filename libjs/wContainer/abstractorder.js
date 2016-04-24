"use strict";
var Class = require("../utils/class");
var AbstractMap = require("./abstractmap");
var AbstractList = require("./abstractlist");

var AbstractOrder = Class.inherit({
    "className": "AbstractOrder",
    "constructor": function(owning) {
        Class.fn.constructor.call(this);
        
        if (!this.constructor.dataType) {
            throw new Error("An attempt to instantiate an order without declared member type");
        }
        
        var Map = AbstractMap.template(this.constructor.dataType, this.constructor.iterator);
        var List = AbstractList.template(this.constructor.dataType);
        
        this._owning = owning !== false;
        
        this._rmap = new Map(this._owning);
        this._order = new List(false);
    },
    "destructor": function() {
        this._rmap.destructor();
        this._order.destructor();
        
        Class.fn.destructor.call(this);
    },
    "begin": function() {
        return this._order.begin();
    },
    "clear": function() {
        this._rmap.clear();
        this._order.clear();
    },
    "end": function() {
        return this._order.end();
    },
    "erase": function(element) {
        var itr = this._rmap.find(element);
        
        if (itr["=="](this._rmap.end())) {
            throw new Error("An attempt to erase non existing element from an order");
        }
        
        var pair = itr["*"]();
        
        this._order.erase(pair.second);
        this._rmap.erase(itr);
    },
    "find": function(data) {
        var itr = this._rmap.find(data);
        
        if (itr["=="](this._rmap.end())) {
            return this.end();
        }
        return itr["*"]().second;
    },
    "push_back": function(data) {
        this._order.push_back(data);
        var itr = this._order.end();
        itr["--"]();
        
        this._rmap.insert(data, itr);
    },
    "size": function() {
        return this._order.size();
    }
});

AbstractOrder.dataType = undefined;
AbstractOrder.iterator = AbstractList.iterator;

AbstractOrder.template = function(data) {
    if (!(data instanceof Function)) {
        throw new Error("Wrong argument to template an order");
    }
    
    var Order = AbstractOrder.inherit({
        "className": "Order",
        "constructor": function(owning) {
            AbstractOrder.fn.constructor.call(this, owning);
        }
    });
    
    Order.dataType = data;
    
    return Order;
}

module.exports = AbstractOrder