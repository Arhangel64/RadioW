"use strict";

var Class = require("../utils/subscribable");

var Test = Class.inherit({
    "className": "Test",
    "constructor": function(name) {
        Class.fn.constructor.call(this);
        
        this._name = name;
        this._actions = [];
    },
    "run": function() {
        this.trigger("start", this._name);
        var succsess = this._actions.length;
        for (var i = 0; i < this._actions.length; ++i) {
            this.trigger("progress", i + 1, this._actions.length);
            try {
                this._actions[i].call(this);
            } catch (e) {
                this.trigger("fail", i + 1, e);
                --succsess;
            }
        }
        this.trigger("end", succsess, this._actions.length);
    }
});

module.exports = Test;