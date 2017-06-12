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
        var controller = new Vocabulary(this._pairAddress["+"](new Address([element.toString()])));
        this.addController(controller);
        
        List.fn.addElement.call(this, element);
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
                if (needStart) {
                    if (s > os) {
                        for (var i = os; i < s; ++i) {
                            var ctrl = this._controllers[i];
                            if (ctrl._subscribed) {
                                ctrl.unsubscribe();
                            }
                        }
                    } else {
                        for (var i = s; i < os; ++i) {
                            var ctrl = this._controllers[i];
                            if (!ctrl._subscribed) {
                                ctrl.subscribe();
                            }
                        }
                    }
                }
                if (needEnd) {
                    var ce = Math.min(this._controllers.length, e);
                    var coe = Math.min(this._controllers.length, oe);
                    if (ce > coe) {
                        for (var i = coe; i < ce; ++i) {
                            var ctrl = this._controllers[i];
                            if (!ctrl._subscribed) {
                                ctrl.subscribe();
                            }
                        }
                    } else if (ce < coe) {
                        for (var i = ce; i < coe; ++i) {
                            var ctrl = this._controllers[i];
                            if (ctrl._subscribed) {
                                ctrl.unsubscribe();
                            }
                        }
                    }
                }
            }
        }
    },
    "_subscribeChildController": function(index) {
        if (index >= this._subscriptionStart && index < this._subscriptionEnd) {
            List.fn._subscribeChildController.call(this, index);
        }
    },
    "_unsubscribeChildController": function(index) {
        if (index >= this._subscriptionStart && index < this._subscriptionEnd) {
            List.fn._unsubscribeChildController.call(this, index);
        }
    }
});

module.exports = PanesList;
