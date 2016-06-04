"use strict";
var Subscribable = require("../lib/utils/subscribable");
var AbstcractMap = require("../lib/wContainer/abstractmap");
var AbstractOrder = require("../lib/wContainer/abstractorder");
var Address = require("../lib/wType/address");
var Uint64 = require("../lib/wType/uint64");
var Event = require("../lib/wType/event");
var Handler = require("../lib/wDispatcher/handler");
var log = require("../lib/log")(module);

var Model = Subscribable.inherit({
    "className": "Model",
    "constructor": function(address) {
        Subscribable.fn.constructor.call(this);
        
        var SMap = AbstcractMap.template(Uint64, Object);
        
        this._subscribers = new SMap(false);
        this._handlers = [];
        this._models = [];
        this._address = address;
        this._subscribersCount = 0;
        
        this.addHandler("subscribe");
        this.addHandler("unsubscribe");
    },
    "destructor": function() {
        var i;
        this.unregister();
        
        var itr = this._subscribers.begin();
        var end = this._subscribers.end();
        
        for (;!itr["=="](end); itr["++"]()) {
            var obj = itr["*"]().second;
            obj.order.destructor();
            obj.socket.off("disconnected", this._onSocketDisconnected, this);
        }
        
        this._subscribers.destructor();
        
        for (i = 0; i < this,_models.length; ++i) {
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
        this._models.push(model);
        if (this._dp) {
            model.register(this._dp, this._socket);
        }
    },
    "broadcast": function(vc, handler) {
        var itr = this._subscribers.begin();
        var end = this._subscribers.end();
        vc.insert("source", this._address.clone());
        
        for (;!itr["=="](end); itr["++"]()) {
            var obj = itr["*"]().second;
            var order = obj.order;
            var oItr = order.begin();
            var oEnd = order.end();
            for (;!oItr["=="](oEnd); oItr["++"]()) {
                var addr = oItr["*"]()["+"](new Address([handler]));
                var ev = new Event(addr, vc.clone());
                ev.setSenderId(obj.socket.getId());
                obj.socket.send(ev);
                ev.destructor();
            }
        }
        vc.destructor();
    },
    "getAddress": function() {
        return this._address.clone();
    },
    "register": function(dp) {
        if (this._dp) {
            throw new Error("Model is already registered in dispatcher");
        }
        this._dp = dp;
        var i;
        
        for (i = 0; i < this._models.length; ++i) {
            this._models[i].register(dp);
        }
        
        for (i = 0; i < this._handlers.length; ++i) {
            dp.registerHandler(this._handlers[i]);
        }
    },
    "response": function(vc, handler, src) {
        var source = src.getData().at("source").clone();
        var id = src.getSenderId().clone();
        var addr = source["+"](new Address([handler]));
        vc.insert("source", this._address.clone());
        
        var ev = new Event(addr, vc);
        ev.setSenderId(id);
        var socket = global.magnus.server.getConnection(id);
        socket.send(ev);
        ev.destructor();
    },
    "_h_subscribe": function(ev) {
        var id = ev.getSenderId();
        var source = ev.getData().at("source");
        
        var itr = this._subscribers.find(id);
        var ord;
        if (itr["=="](this._subscribers.end())) {
            ord = new Model.addressOrder(true);
            var socket = global.magnus.server.getConnection(id);
            socket.one("disconnected", this._onSocketDisconnected, this);
            this._subscribers.insert(id.clone(), {order: ord, socket: socket});
        } else {
            ord = itr["*"]().second.order;
            var oItr = order.find(source);
            if (!oItr["=="](order.end())) {
                log.warn(   "id: " + id.toString() + ", " +
                            "source: " + source.toString() + " " +
                            "is trying to subscribe on model " + this._address.toString() + " " +
                            "but it's already subscribed" );
                return
            }
        }
        
        ord.push_back(source.clone());
        ++this._subscribersCount;
        log.info(this._address.toString() + " has now " + this._subscribersCount + " subscribers");
    },
    "_h_unsubscribe": function(ev) {
        var id = ev.getSenderId();
        var source = ev.getData().at("source");
        
        this._unsubscribe(id, source);
    },
    "_onSocketDisconnected": function(ev, socket) {
        var id = socket.getId();
        var itr = this._subscribers.find(id);
        
        if (itr["=="](this._subscribers.end())) {
            log.warn(   "id: " + id.toString() + ", " +
                        "after socket disconnected trying to remove subscriptions from model " +
                        "but id haven't been found in subscribers map" );
            return
        }
        var ord = itr["*"]().second.order;
        this._subscribersCount -= ord.size();
        this._subscribers.erase(itr);
        ord.destructor();
        log.info(this._address.toString() + " has now " + this._subscribersCount + " subscribers");
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
        }
    },
    "_unsubscribe": function(id, source) {
        var itr = this._subscribers.find(id);
        
        if (itr["=="](this._subscribers.end())) {
            log.warn(   "id: " + id.toString() + ", " +
                        "source: " + source.toString() + " " +
                        "is trying to unsubscribe from model " + this._address.toString() + " " +
                        "but even this id is not registered in subscribers map"
                    );
            return
        }
        var ord = itr["*"]().second.order;
        var oItr = ord.find(source);
        if (oItr["=="](ord.end())) {
            log.warn(   "id: " + id.toString() + ", " +
                        "source: " + source.toString() + " " +
                        "is trying to unsubscribe from model " + this._address.toString() + " " +
                        "but such address is not subscribed to this model"
                    );
            return
        }
        ord.erase(oItr);
        if (ord.size() === 0) {
            var socket = itr["*"]().second.socket;
            socket.off("disconnected", this._onSocketDisconnected, this);
            this._subscribers.erase(itr);
            ord.destructor();
        }
        
        --this._subscribersCount;
        log.info(this._address.toString() + " has now " + this._subscribersCount + " subscribers");
    }
});

Model.addressOrder = AbstractOrder.template(Address);

module.exports = Model;