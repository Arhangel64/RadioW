"use strict";
var DefaultHandler = require("./defaulthandler");
        
var Logger = DefaultHandler.inherit({
    "className": "Logger",
    "constructor": function() {
        DefaultHandler.fn.constructor.call(this);
    },
    "call": function(event) {
        console.log("Event went to default handler");
        console.log("Destination: " + event.getDestination().toString());
        console.log("Data: " + event.getData().toString());
        
        return false;
    }
});

module.exports = Logger;
