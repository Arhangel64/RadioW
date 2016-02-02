"use strict";
(function defaulthandler_js() {
    var moduleName = "lib/wDispatcher/defaulthandler";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    
    define(moduleName, defineArray, function defaulthandler_module() {
        var Class = require("lib/utils/class");
        var id = 0;
        
        var DefaultHandler = Class.inherit({
            "className": "DefaultHandler",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                this._id = id++;
            },
            "==": function(other) {
                if (!(other instanceof Handler)) {
                    throw new Error("Can compare only Handler with Handler");
                }
                return this._id === other._id;
            },
            ">": function(other) {
                if (!(other instanceof Handler)) {
                    throw new Error("Can compare only Handler with Handler");
                }
                return this._id > other._id;
            },
            "<": function(other) {
                if (!(other instanceof Handler)) {
                    throw new Error("Can compare only Handler with Handler");
                }
                return this._id < other._id;
            },
            "call": function(event) {
                throw new Error("Attempt to call pure abstract default handler");
            }
        });
        
        return DefaultHandler;
    });
})();