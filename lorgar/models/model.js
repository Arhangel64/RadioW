"use strict";
(function model_js() {
    var moduleName = "models/model";
    
    var defineArray = [];
    defineArray.push("lib/utils/subscribable");
    defineArray.push("lib/wDispatcher/handler");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wType/string");
    defineArray.push("lib/wType/vocabulary");
    defineArray.push("lib/wType/event");
    
    define(moduleName, defineArray, function model_module() {
        var counter = 0;
        var Subscribable = require("lib/utils/subscribable");
        var Handler = require("lib/wDispatcher/handler");
        var Address = require("lib/wType/address");
        var String = require("lib/wType/string");
        var Vocabulary = require("lib/wType/vocabulary");
        var Event = require("lib/wType/event");
        
        var Model = Subscribable.inherit({
            "className": "Model",
            "constructor": function(addr) {
                Subscribable.fn.constructor.call(this);
                
                this._subscribed = false;
                this._pairAddress = addr;
                this._address = new Address([this.className.toString() + counter]);
                ++counter;
                
                this._views = [];
                this._handlers = [];
                this._models = [];
            },
            "destructor": function() {
                var i;
                this._unsubscribe();
                this._unregister();
                
                for (i = 0; i < this._models.length; ++i) {
                    this._models[i].destructor();
                }
                
                for (i = 0; i < this._handlers.length; ++i) {
                    this._handlers[i].destructor();
                }
                
                Subscribable.fn.destructor.call(this);
            },
            "addHandler": function(name) {
                if (!(this["_h_" + name] instanceof Function)) {
                    throw new Error("An attempt to create handler without a handling method");
                }
                
                var handler = new Handler(this._address["+"](new Address([name])), this, this["_h_" + name]);
                
                this._handlers.push(handler);
                if (this._dp) {
                    this._dp.registerHandler(handler);
                }
            },
            "addModel": function(model) {
                if (!(model instanceof Model)) {
                    throw new Error("An attempt to add not a model into " + this.className);
                }
                if (this._dp) {
                    model.register(this._dp, this._socket);
                }
            },
            "addView": function(view) {
                this._views.push(view);
            },
            "register": function(dp, socket) {
                var i;
                if (this._dp) {
                    throw new Error("Model is already registered in dispatcher");
                }
                this._dp = dp;
                this._socket = socket;
                
                for (i = 0; i < this._models.length; ++i) {
                    this._models.register(this._dp, this._socket);
                }
                
                for (i = 0; i < this._handlers.length; ++i) {
                    dp.registerHandler(this._handlers[i]);
                }
            },
            "send": function(handler, vc) {
                var addr = this._pairAddress["+"](new Address([handler]));
                var id = this._socket.getId().clone();
                
                vc.insert("source", this._address.clone());
                
                var ev = new Event(addr, vc);
                ev.setSenderId(id);
                this._socket.send(ev);
                ev.destructor();
            },
            "subscribe": function() {
                if (!this._subscribed) {
                    this._subscribed = true;
                    
                    var vc = new Vocabulary();
                    this.send("subscribe", vc);
                }
            },
            "unregister": function() {
                if (this._dp) {
                    var i;
                    for (i = 0; i < this._models.length; ++i) {
                        this._models.unregister();
                    }
                    for (i = 0; i < this._handlers.length; ++i) {
                        this._dp.unregisterHandler(this._handlers[i]);
                    }
                    delete this._dp;
                    delete this._socket;
                }
            },
            "unsubscribe": function() {
                if (this._subscribed) {
                    this._subscribed = false;
                    
                    var vc = new Vocabulary();
                    this.send("unsubscribe", vc);
                }
            }
        });
        
        return Model;
    });
})();