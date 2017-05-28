"use strict";

var Model = require("../model");
var Handler = require("../../wDispatcher/handler");

var Proxy = Model.inherit({
    "className": "Proxy",
    "constructor": function(address, controller, socket) {      //les't pretend - this class is abstract
        Model.fn.constructor.call(this, address);
        
        this._socket = socket;
        this.ready = false;
        this.controller = controller;
        this.childrenPossible = false;
        this._destroyOnLastUnsubscription = false;
        this._childClass = undefined;
        this._waitingEvents = [];
        
        this.addHandler("get");
        
        this._uncyclic.push(function() {
            controller.destructor();
        });
    },
    "destructor": function() {
        for (var i = 0; i < this._waitingEvents.length; ++i) {
            this._waitingEvents[i].destructor();
        }
        Model.fn.destructor.call(this);
    },
    "checkSubscribersAndDestroy": function() {
        if (this._subscribersCount === 0) {
            this.trigger("serviceMessage", this._address.toString() + " has no more subscribers, destroing model");
            setTimeout(this.destructor.bind(this), 1);
        }
    },
    "dispatchWaitingEvents": function() {
        for (var i = 0; i < this._waitingEvents.length; ++i) {
            var ev = this._waitingEvents[i];
            var cmd =  "_h_" + ev.getDestination().back().toString();
            this[cmd](ev);
        }
        this._waitingEvents = [];
    },
    "_h_get": function(ev) {
        if (this.ready) {
            var vc = new Vocabulary();
            
            vc.insert("data", this.controller.data.clone());
            this.response(vc, "get", ev);
        } else {
            this._waitingEvents.push(ev);
        }
    },
    "_h_unsubscribe": function(ev) {
        Model.fn._h_unsubscribe.call(this, ev);
        
        if (this._destroyOnLastUnsubscription) {
            this.checkSubscribersAndDestroy()
        }
    },
    "_subscribeMember": function(ev) {
        if (!this.childrenPossible) {
            return;
        }
        var dest = ev.getDestination();
        var lastHops = dest["<<"](this._address.size());
        
        if (lastHops.size() === 2) {
            var command = lastHops.back().toString();
            var id = lastHops[">>"](1);
            if (command === "subscribe" || command === "get") {
                var child = new this._childClass(this._address["+"](id), this._controller._pairAddress["+"](id), this._socket);
                this.addModel(child);
                child._destroyOnLastUnsubscription = true;
                child["_h_" + command](ev);
                if (command === "get") {
                    child.on("ready", child.checkSubscribersAndDestroy, child);
                }
            } else {
                this.trigger("serviceMessage", "Proxy model got a strange event: " + ev.toString());
            }
        } else {
            this.trigger("serviceMessage", "Proxy model got a strange event: " + ev.toString());
        }
    },
    "register": function(dp, server) {
        Model.fn.register.call(this, dp, server);
        
        this.controller.register(dp, this._socket);
    },
    "unregister": function() {
        Model.fn.unregister.call(this);
        
        this.controller.unregister();
    },
    "setChildrenClass": function(Class) {
        if (this.childrenPossible) {
            this.trigger("serviceMessage", "An attempt to set another class for children in Proxy", 1);
        }
        if (!Class instanceof Proxy) {
            this.trigger("serviceMessage", "An attempt to set not inherited chidren class from Proxy to a Proxy", 2)
        }
        this.childrenPossible = true;
        this._childClass = Class;
    },
    "setReady": function(bool) {
        bool = !!bool;
        if (bool !== this.ready) {
            this.ready = bool;
            if (bool) {
                this.dispatchWaitingEvents();
                this.trigger("ready");
            } else {
                //todo do I realy need to trigger smth here?
            }
        }
    },
    "subscribe": function() {
        this.controller.subscribe();
    },
    "unsetChildrenClass": function() {
        if (!this.childrenPossible) {
            this.trigger("serviceMessage", "An attempt to unset children class in Proxy, which dont have it", 1);
        } else {
            delete this._childClass;
            this.childrenPossible = false;
        }
    },
    "unsubscribe": function() {
        this.controller.unsubscribe();
        this.setReady(false);
    }
});

Proxy.onChildReady = function(ev) {
    this._h_get(ev);
    this.checkSubscribersAndDestroy();
}

module.exports = Proxy;

