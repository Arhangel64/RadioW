"use strict";
var Subscribable = require("../utils/subscribable");
var AbstcractMap = require("../wContainer/abstractmap");
var AbstractOrder = require("../wContainer/abstractorder");
var Address = require("../wType/address");
var Uint64 = require("../wType/uint64");
var Event = require("../wType/event");
var Vector = require("../wType/vector");
var Vocabulary = require("../wType/vocabulary");
var String = require("../wType/string");
var Handler = require("../wDispatcher/handler");

var Model = Subscribable.inherit({
    "className": "Model",
    "constructor": function(address) {
        Subscribable.fn.constructor.call(this);
        
        var SMap = AbstcractMap.template(Uint64, Model.addressOrder);
        
        this._registered = false;
        this._subscribers = new SMap(false);
        this._handlers = [];
        this._models = [];
        this._props = new Vector();
        this._address = address;
        this._subscribersCount = 0;
        
        this.addHandler("subscribe");
        this.addHandler("unsubscribe");
    },
    "destructor": function() {
        var i;
        if (this._registered) {
            this.unregister();
        }
        
        this._subscribers.destructor();
        
        for (i = 0; i < this,_models.length; ++i) {
            this._models[i].destructor();
        }
        
        for (i = 0; i < this._handlers.length; ++i) {
            this._handlers[i].destructor();
        }
        this._props.destructor();
        
        Subscribable.fn.destructor.call(this);
    },
    "addHandler": function(name) {
        if (!(this["_h_" + name] instanceof Function)) {
            throw new Error("An attempt to create handler without a handling method");
        }
        
        var handler = new Handler(this._address["+"](new Address([name])), this, this["_h_" + name]);
        
        this._addHandler(handler);
    },
    "_addHandler": function(handler) {
        this._handlers.push(handler);
        if (this._registered) {
            this._dp.registerHandler(handler);
        }
    },
    "addModel": function(model) {
        if (!(model instanceof Model)) {
            throw new Error("An attempt to add not a model into " + this.className);
        }
        this._models.push(model);
        model.on("serviceMessage", this._onModelServiceMessage, this);
        if (this._registered) {
            model.register(this._dp, this._server);
        }
    },
    "addProperty": function(property, key) {
        var vc = new Vocabulary();
        vc.insert("property", new String(property));
        vc.insert("key", new String(key));
        
        this._props.push(vc);
        
        if (this._registered) {
            var nvc = new Vocabulary();
            nvc.insert("properties", this._props.clone());
            this.broadcast(nvc, "properties");
        }
    },
    "broadcast": function(vc, handler) {
        var itr = this._subscribers.begin();
        var end = this._subscribers.end();
        vc.insert("source", this._address.clone());
        
        for (;!itr["=="](end); itr["++"]()) {
            var obj = itr["*"]();
            var order = obj.second;
            var socket = this._server.getConnection(obj.first);
            var oItr = order.begin();
            var oEnd = order.end();
            for (;!oItr["=="](oEnd); oItr["++"]()) {
                var addr = oItr["*"]()["+"](new Address([handler]));
                var ev = new Event(addr, vc.clone());
                ev.setSenderId(socket.getId().clone());
                socket.send(ev);
                ev.destructor();
            }
        }
        vc.destructor();
    },
    "getAddress": function() {
        return this._address.clone();
    },
    "getType": function() {
        var type = Model.ModelType[this.className];
        if (type === undefined) {
            throw new Error("Undefined ModelType");
        }
        return type;
    },
    "_h_subscribe": function(ev) {
        var id = ev.getSenderId();
        var source = ev.getData().at("source");
        
        var itr = this._subscribers.find(id);
        var ord;
        if (itr["=="](this._subscribers.end())) {
            ord = new Model.addressOrder(true);
            var socket = this._server.getConnection(id);
            socket.one("disconnected", this._onSocketDisconnected, this);
            this._subscribers.insert(id.clone(), ord);
        } else {
            ord = itr["*"]().second;
            var oItr = order.find(source);
            if (!oItr["=="](order.end())) {
                this.tigger("serviceMessage",   "id: " + id.toString() + ", " +
                            "source: " + source.toString() + " " +
                            "is trying to subscribe on model " + this._address.toString() + " " +
                            "but it's already subscribed", 1);
                return
            }
        }
        
        ord.push_back(source.clone());
        ++this._subscribersCount;
        this.trigger("serviceMessage", this._address.toString() + " has now " + this._subscribersCount + " subscribers", 0);
        
        var nvc = new Vocabulary();
        nvc.insert("properties", this._props.clone());
        
        this.response(nvc, "properties", ev);
    },
    "_h_unsubscribe": function(ev) {
        var id = ev.getSenderId();
        var source = ev.getData().at("source");
        
        var itr = this._subscribers.find(id);
        
        if (itr["=="](this._subscribers.end())) {
            this.trigger("serviceMessage", "id: " + id.toString() + ", " +
                        "source: " + source.toString() + " " +
                        "is trying to unsubscribe from model " + this._address.toString() + " " +
                        "but even this id is not registered in subscribers map", 1
                    );
            return
        }
        var ord = itr["*"]().second;
        var oItr = ord.find(source);
        if (oItr["=="](ord.end())) {
            this.trigger("serviceMessage", "id: " + id.toString() + ", " +
                        "source: " + source.toString() + " " +
                        "is trying to unsubscribe from model " + this._address.toString() + " " +
                        "but such address is not subscribed to this model", 1
                    );
            return
        }
        ord.erase(oItr["*"]());
        if (ord.size() === 0) {
            var socket = this._server.getConnection(itr["*"]().first);
            socket.off("disconnected", this._onSocketDisconnected, this);
            this._subscribers.erase(itr);
            ord.destructor();
        }
        
        --this._subscribersCount;
        this.trigger("serviceMessage", this._address.toString() + " has now " + this._subscribersCount + " subscribers", 0);
    },
    "_onModelServiceMessage": function(msg, severity) {
        this.trigger("serviceMessage", msg, severity);
    },
    "_onSocketDisconnected": function(ev, socket) {
        var id = socket.getId();
        var itr = this._subscribers.find(id);
        
        if (itr["=="](this._subscribers.end())) {
            this.trigger("serviceMessage", "id: " + id.toString() + ", " +
                        "after socket disconnected trying to remove subscriptions from model " +
                        "but id haven't been found in subscribers map", 1);
            return
        }
        var ord = itr["*"]().second;
        this._subscribersCount -= ord.size();
        this._subscribers.erase(itr);
        ord.destructor();
        this.trigger("serviceMessage", this._address.toString() + " has now " + this._subscribersCount + " subscribers", 0);
    },
    "register": function(dp, server) {
        if (this._registered) {
            throw new Error("Model " + this._address.toString() + " is already registered");
        }
        this._dp = dp;
        this._server = server;
        var i;
        
        for (i = 0; i < this._models.length; ++i) {
            this._models[i].register(dp, server);
        }
        
        for (i = 0; i < this._handlers.length; ++i) {
            dp.registerHandler(this._handlers[i]);
        }
        this._registered = true;
    },
    "_removeHandler": function(handler) {
        var index = this._handlers.indexOf(handler);
        if (index === -1) {
            throw new Error("An attempt to remove non existing handler");
        }
        this._handlers.splice(index, 1);
        if (this._registered) {
            this._dp.unregisterHandler(handler);
        }
    },
    "removeModel": function(model) {
        if (!(model instanceof Model)) {
            throw new Error("An attempt to remove not a model from " + this.className);
        }
        var index = this._models.indexOf(model);
        if (index === -1) {
            throw new Error("An attempt to remove non existing model from " + this.className);
        }
        this._models.splice(index, 1);
        if (this._registered) {
            model.unregister(this._dp, this._server);
        }
    },
    "response": function(vc, handler, src) {
        if (!this._registered) {
            throw new Error("An attempt to send a message from unregistered model " + this._address.toString());
        }
        var source = src.getData().at("source").clone();
        var id = src.getSenderId().clone();
        var addr = source["+"](new Address([handler]));
        vc.insert("source", this._address.clone());
        
        var ev = new Event(addr, vc);
        ev.setSenderId(id);
        var socket = this._server.getConnection(id);
        socket.send(ev);
        ev.destructor();
    },
    "unregister": function() {
        if (!this._registered) {
            throw new Error("Model " + this._address.toString() + " is not registered");
        }
        var i;
        
        for (i = 0; i < this._models.length; ++i) {
            this._models[i].unregister();
        }
        
        for (i = 0; i < this._handlers.length; ++i) {
            this._dp.unregisterHandler(this._handlers[i]);
        }
        
        var itr = this._subscribers.begin();
        var end = this._subscribers.end();
        
        for (;!itr["=="](end); itr["++"]()) {
            var socket = this._server.getConnection(itr["*"]().first);
            var ord = itr["*"]().second;
            order.destructor();
            socket.off("disconnected", this._onSocketDisconnected, this);
        }
        this._subscribers.clear();
        this._subscribersCount = 0;
        
        delete this._dp;
        delete this._server;
        
        this._registered = false;
    }
});

Model.getModelTypeId = function(model) {
    return this.ModelType[model.className];
}

Model.addressOrder = AbstractOrder.template(Address);
Model.ModelType = {
    String:     0,
    List:       1,
    Vocabulary: 2,
    
    Attributes: 50,
    
    GlobalControls: 100,
    Link: 101,
    Page: 102,
    PageStorage: 103,
    PanesList: 104,
    Theme: 105,
    ThemeStorage: 106
};

module.exports = Model;
