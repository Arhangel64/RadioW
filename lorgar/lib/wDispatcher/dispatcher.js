"use strict";
(function dispatcher_js() {
    var moduleName = "lib/wDispatcher/dispatcher"
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    defineArray.push("lib/wDispatcher/handler");
    defineArray.push("lib/wDispatcher/defaulthandler");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wContainer/abstractmap");
    defineArray.push("lib/wContainer/abstractorder");
    
    define(moduleName, defineArray, function dispatcher_module() {
        var Class = require("lib/utils/class");
        var Handler = require("lib/wDispatcher/handler");
        var DefaultHandler = require("lib/wDispatcher/defaulthandler");
        var Address = require("lib/wType/address");
        var AbstractMap = require("lib/wContainer/abstractmap");
        var AbstractOrder = require("lib/wContainer/abstractorder");
        
        var HandlerOrder = AbstractOrder.template(Handler);
        var DefaultHandlerOrder = AbstractOrder.template(DefaultHandler);
        var HandlerMap = AbstractMap.template(Address, HandlerOrder);
        
        var Dispatcher = Class.inherit({
            "className": "Dispatcher",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                this._handlers = new HandlerMap();
                this._defautHandlers = new DefaultHandlerOrder(false);
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
            },
            "unregisterHandler": function(handler) {
                var itr = this._handlers.find(handler.address);
                
                if (!itr["=="](this._handlers.end())) {
                    
                    var ord = itr["*"]().second;
                    ord.erase(handler);
                    
                } else {
                    throw new Error("Can't unregister hander");
                }
            },
            "registerDefautHandler": function(dh) {
                this._defautHandlers.push_back(dh);
            },
            "unregisterDefaultHandler": function(dh) {
                this._defautHandlers.erase(dh);
            },
            "pass": function(event) {
                var itr = this._handlers.find(event.getDestination());
                
                if (!itr["=="](this._handlers.end())) {
                    var ord = itr["*"]().second;
                    
                    var o_beg = ord.begin();
                    var o_end = ord.end();
                    
                    for (; !o_beg["=="](o_end); o_beg["++"]()) {
                        o_beg["*"]().pass(event);
                    }
                } else {
                    var dhitr = this._defautHandlers.begin();
                    var dhend = this._defautHandlers.end();
                    for (; !dhitr["=="](dhend); dhitr["++"]()) {
                        dhitr["*"]().call(event);
                    }
                }
            }
        });
        
        return Dispatcher;
    });
})();