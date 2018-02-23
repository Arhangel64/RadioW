"use strict";
var List = require("./list");
var Vocabulary = require("./vocabulary");

var Address = require("../wType/address");

var PanesList = List.inherit({
    "className": "PanesList",
    "constructor": function PanesListModel(addr) {
        List.fn.constructor.call(this, addr);
        
        this._subscriptionStart = 0;
        this._subscriptionEnd = Infinity;
    },
    "addElement": function(element) {
        var size = this.data.size();
        List.fn.addElement.call(this, element);
        
        if (size >= this._subscriptionStart && size < this._subscriptionEnd) {
            var controller = new Vocabulary(this._pairAddress["+"](new Address([element.toString()])));
            this.addController(controller);
        }
    },
    "clear": function() {
        List.fn.clear.call(this);
        this.clearChildren();
    },
    "setSubscriptionRange": function(s, e) {
        var needStart = s !== this._subscriptionStart;
        var needEnd = e !== this._subscriptionEnd;
        if (needStart || needEnd) {
            var os = this._subscriptionStart;
            var oe = this._subscriptionEnd;
            this._subscriptionStart = s;
            this._subscriptionEnd = e;
            if (this._subscribed) {
                this.trigger("rangeStart");
                if (needStart) {
                    if (s > os) {
                        var limit = Math.min(s - os, this._controllers.length);
                        for (var i = 0; i < limit; ++i) {
                            var ctrl = this._controllers[0];
                            this._removeControllerByIndex(0);
                            ctrl.destructor();
                        }
                    } else {
                        var limit = Math.min(os, e) - s;
                        for (var i = 0; i < limit; ++i) {
                            var ctrl = new Vocabulary(this._pairAddress["+"](new Address([this.data.at(i + s).toString()])));
                            this.addController(ctrl, i);
                        }
                    }
                }
                if (needEnd) {
                    var ce = Math.min(this.data.size(), e);
                    var coe = Math.min(this.data.size(), oe);
                    if (ce > coe) {
                        var start = Math.max(s, oe);
                        var amount = ce - start;    //it can be negative, it's fine
                        for (var i = 0; i < amount; ++i) {
                            var ctrl = new Vocabulary(this._pairAddress["+"](new Address([this.data.at(start + i).toString()])));
                            this.addController(ctrl);
                        }
                    } else if (ce < coe) {
                        var amount = Math.min(coe - ce, coe - os);
                        for (var i = 0; i < amount; ++i) {
                            var index = this._controllers.length - 1;
                            var ctrl = this._controllers[index];
                            this._removeControllerByIndex(index);
                            ctrl.destructor();
                        }
                    }
                }
                this.trigger("rangeEnd");
            }
        }
    }
});

module.exports = PanesList;
