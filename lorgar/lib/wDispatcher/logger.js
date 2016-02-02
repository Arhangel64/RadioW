"use strict";
(function logger_js() {
    var moduleName = "lib/wDispatcher/logger";
    
    var defineArray = [];
    defineArray.push("lib/wDispatcher/defaulthandler");
    
    define(moduleName, defineArray, function logger_module() {
        var DefaultHandler = require("lib/wDispatcher/defaulthandler");
        
        var Logger = DefaultHandler.inherit({
            "className": "Logger",
            "constructor": function() {
                DefaultHandler.fn.constructor.call(this);
            },
            "call": function(event) {
                console.log("Event went to default handler");
                console.log("Destination: " + event.getDestination().toString());
                console.log("Data: " + event.getData().toString());
            }
        });
        
        return Logger;
    });
})();