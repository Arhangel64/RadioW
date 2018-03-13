"use strict";
var counter = 0;
var Subscribable = require("../utils/subscribable");

var LocalModel = Subscribable.inherit({
    "className": "LocalModel",
    "constructor": function(properties) {
        Subscribable.fn.constructor.call(this);
        
        this.properties = [];
        this._controllers = [];
        
        if (properties) {
            for (var key in properties) {
                if (properties.hasOwnProperty(key)) {
                    var pair = {p: key, k: properties[key]};
                    this.properties.push(pair);
                }
            }
        }
    },
    "setData": function(data) {
        this.data = data;
        this.trigger("data");
    }
});

module.exports = LocalModel;
