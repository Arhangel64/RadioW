"use strict";
(function dispatcher_js() {
    var moduleName = "lib/wDispatcher/dispatcher"
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    defineArray.push("lib/wDispatcher/handler");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wContainer/abstractmap");
    defineArray.push("lib/wContainer/abstractorder");
    
    define(moduleName, defineArray, function dispatcher_module() {
        var Class = require("lib/utils/class");
        var Handler = require("lib/wDispatcher/handler");
        var Address = require("lib/wType/address");
        var AbstractMap = require("lib/wContainer/abstractmap");
        var AbstractOrder = require("lib/wContainer/abstractorder");
        
        var HandlerOrder = AbstractOrder.template(Handler);
        var HandlerMap = AbstractMap.template(Address, HandlerOrder);
        
        var Dispatcher = Class.inherit({
            "className": "Dispatcher",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                this._handlers = new HandlerMap();
                this._defautHandlers;
            },
            "destructor": function() {
                this._handlers.destructor();
                
                Class.fn.destructor.call(this);
            },
            "registerHandler": function(handler) {
                var itr = this._handlers.find(handler.address);
                var order;
                
                
                if (itr["=="](this._handlers.end())) {
                    order = new HandlerOrder(false);
                    this._handlers.insert(handler.address.clone(), order);
                } else {
                    order = itr["*"]();
                }
                
                order.push_back(handler);
            }
        });
        
        return Dispatcher;
    });
})();