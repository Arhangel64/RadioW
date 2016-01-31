"use strict";
(function abstractorder_js() {
    var moduleName = "lib/wContainer/abstractorder";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    defineArray.push("lib/wContainer/abstractmap");
    defineArray.push("lib/wContainer/abstractlist");
    
    define(moduleName, defineArray, function abstractorder_module() {
        var Class = require("lib/utils/class");
        var AbstractMap = require("lib/wContainer/abstractmap");
        var AbstractList = require("lib/wContainer/abstractlist");
        
        var AbstractOrder = Class.inherit({
            "className": "AbstractOrder",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                if (!this.constructor.dataType) {
                    throw new Error("An attempt to instantiate an order without declared member type");
                }
                
                var Map = AbstractMap.template(this.constructor.dataType, this.constructor.iterator);
                var List = AbstractList.template(this.constructor.dataType);
                
                this._rmap = new Map();
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
                "constructor": function() {
                    AbstractOrder.fn.constructor.call(this);
                }
            });
            
            Order.dataType = data;
            
            return Order;
        }
        
        return AbstractOrder
    });
})();