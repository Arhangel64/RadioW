"use strict";
(function dispatcher_js() {
    var moduleName = "lib/wDispatcher/dispatcher"
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    defineArray.push("lib/wDispatcher/handler");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wContainers/abstractmap");
    
    define(moduleName, defineArray, function dispatcher_module() {
        var Class = require("lib/utils/class");
        var Handler = require("lib/wDispatcher/handler");
        var Address = require("lib/wType/address");
        var AbstractMap = require("lib/wContainers/abstractmap");
        var HandlerMap = AbstractMap.template(Address, Array);
        
        var Dispatcher = Class.inherit({
            "className": "Dispatcher",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                this._handlers = new HandlerMap();
            },
            "destructor": function() {
                this._handlers.destructor();
                
                Class.fn.destructor.call(this);
            }
        });
        
        return Dispatcher;
    });
})();