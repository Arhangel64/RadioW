"use strict";
var Class = require("../utils/class");
var Handler = require("./handler");
var DefaultHandler = require("./defaulthandler");
var Address = require("../wType/address");
var AbstractMap = require("../wContainer/abstractmap");
var AbstractOrder = require("../wContainer/abstractorder");

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
            
            if (ord.size() === 0) {
                this._handlers.erase(itr);
            }
            
        } else {
            throw new Error("Can't unregister hander");
        }
    },
    "registerDefaultHandler": function(dh) {
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

module.exports = Dispatcher;
