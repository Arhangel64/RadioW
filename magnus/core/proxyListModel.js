"use strict";

var Model = require("../lib/wModel/model");

var ListCtrl = require("../lib/wController/list");

var Vocabulary = require("../lib/wType/vocabulary");

var ProxyListModel = Model.inherit({
    "className": "PanesList",
    "constructor": function(address, remoteAddress, socket) {
        Model.fn.constructor.call(this, address);
        
        this._socket = socket;
        this._subscribed = false;
        var rl = this.remoteList = new ListCtrl(remoteAddress);
        rl.on("clear", this._onRemoteClear, this);
        rl.on("newElement", this._onRemoteNewElement, this);
        
        this.addHandler("get");
        
        this._uncyclic.push(function() {
            rl.destructor();
        });
    },
    "destructor": function() {
        Model.fn.destructor.call(this);
    },
    "_h_subscribe": function(ev) {
        Model.fn._h_subscribe.call(this, ev);
        
        this._h_get(ev);
    },
    "_h_get": function(ev) {
        var vc = new Vocabulary();
        
        vc.insert("data", this.remoteList.data.clone());
        this.response(vc, "get", ev);
    },
    "register": function(dp, server) {
        Model.fn.register.call(this, dp, server);
        
        this.remoteList.register(dp, this._socket);
    },
    "unregister": function() {
        Model.fn.unregister.call(this);
        
        this.remoteList.unregister();
    },
    "_onRemoteClear": function() {
        this.broadcast(new Vocabulary(), "clear");
    },
    "_onRemoteNewElement": function(obj) {
        
        var vc = new Vocabulary();
        vc.insert("data", obj.clone());
        
        this.broadcast(vc, "push");
    },
    "subscribe": function() {
        if (!this._subscribed) {
            this._subscribed = true;
            this.remoteList.subscribe();
        }
    }
});

module.exports = ProxyListModel;
