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
                this._props = [];
                
                this.addHandler("properties");
            },
            "destructor": function() {
                var i;
                var j;
                for (i = 0; i < this._views.length; ++i) {
                    for (j = 0; j < this._props.length; ++j) {
                        this._views[i].removeProperty(this._props[j]);
                    }
                }
                this.remove();
                this.unsubscribe();
                this.unregister();
                
                while (this._models.length) {
                    this._models[this._models.length - 1].destructor();
                }
                
                for (i = 0; i < this._handlers.length; ++i) {
                    this._handlers[i].destructor();
                }
                this._pairAddress.destructor();
                this._address.destructor();
                
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
                if (model._parent) {
                    throw new Error("Inserting model already has parent");
                } 
                model._parent = this;
                this._models.push(model);
                if (this._dp) {
                    model.register(this._dp, this._socket);
                }
                if (this._subscribed) {
                    model.subscribe();
                }
                this.trigger("newModel", model);
            },
            "addView": function(view) {
                this._views.push(view);
                
                for (var i = 0; i < this._props.length; ++i) {
                    view.addProperty(this._props[i]);
                }
            },
            "_h_properties": function(ev) {
                var i;
                var j;
                for (i = 0; i < this._views.length; ++i) {
                    for (j = 0; j < this._props.length; ++j) {
                        this._views[i].removeProperty(this._props[j]);
                    }
                }
                this._props = [];
                var data = ev.getData();
                var list = data.at("properties");
                var size = list.size();
                for (i = 0; i < size; ++i) {
                    var vc = list.at(i);
                    var pair = {p: vc.at("property").toString(), k: vc.at("key").toString()};
                    this._props.push(pair);
                }
                for (i = 0; i < this._views.length; ++i) {
                    for (j = 0; j < this._props.length; ++j) {
                        this._views[i].addProperty(this._props[j]);
                    }
                }
                
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
            "remove": function() {
                if (this._parent) {
                    this._parent.removeModel(this);
                }
            },
            "removeModel": function(model) {
                if (!(model instanceof Model)) {
                    throw new Error("An attempt to remove not a model from " + this.className);
                }
                var index = this._models.indexOf(model);
                if (index !== -1) {
                    if (this._subscribed) {
                        model.unsubscribe();
                    }
                    if (this._dp) {
                        model.unregister();
                    }
                    this._models.splice(index, 1);
                    delete model._parent;
                
                    this.trigger("removedModel", model);
                } else {
                    throw new Error("An attempt to remove not not existing model from " + this.className);
                }
            },
            "send": function(vc, handler) {
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
                    this.send(vc, "subscribe");
                    
                    for (var i = 0; i < this._models.length; ++i) {
                        this._models[i].subscribe();
                    }
                }
            },
            "unregister": function() {
                if (this._dp) {
                    var i;
                    for (i = 0; i < this._models.length; ++i) {
                        this._models[i].unregister();
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
                    this.send(vc, "unsubscribe");
                    
                    for (var i = 0; i < this._models.length; ++i) {
                        this._models[i].unsubscribe();
                    }
                }
            }
        });
        
        return Model;
    });
})();
