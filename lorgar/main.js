"use strict";
(function main_js() {
    requirejs.onError = function(e) {
        throw e;
    }
    
    var defineArray = [];
    
    defineArray.push("lib/utils/subscribable");
    defineArray.push("lib/wType/string");
    defineArray.push("lib/wType/vocabulary");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wType/boolean");
    defineArray.push("lib/wType/event");
    
    defineArray.push("lib/wSocket/socket");
    
    require(defineArray, function main_module() {
        var Subscribable = require("lib/utils/subscribable");
        var String = require("lib/wType/string");
        var Vocabulary = require("lib/wType/vocabulary");
        var Address = require("lib/wType/address");
        var Boolean = require("lib/wType/boolean");
        var Event = require("lib/wType/event");
        
        var Socket = require("lib/wSocket/socket");
        
        var Some = Subscribable.inherit({
            "className": "Some",
            "constructor": function() {
                Subscribable.fn.constructor.call(this);
                
                this._socket = new Socket("Lorgar");
                this._socket.on("connected", function() {
                    console.log("Yaaaahooo!");
                });
            },
            "launch": function() {
                this._socket.open("localhost", 8080);
            }
        });
        
        var some = new Some();
        window.some = some;
    });
})();