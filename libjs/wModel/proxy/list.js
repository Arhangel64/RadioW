"use strict";

var Proxy = require("./proxy");
var Vocabulary = require("../../wType/vocabulary");
var Ctrl = require("../../wController/list");

var List = Proxy.inherit({
    "className": "List",
    "constructor": function(address, controllerAddress, socket) {
        var controller = new Ctrl(controllerAddress);
        Proxy.fn.constructor.call(this, address, controller, socket);
        
        this.controller.on("get", this._onRemoteGet, this);
        this.controller.on("clear", this._onRemoteClear, this);
        this.controller.on("newElement", this._onRemoteNewElement, this);
    },
    "_h_subscribe": function(ev) {
        Proxy.fn._h_subscribe.call(this, ev);
        
        this._h_get(ev);
    },
    "_onRemoteClear": function() {
        if (this.ready) {
            this.broadcast(new Vocabulary(), "clear");
        }
    },
    "_onRemoteGet": function() {
        this.setReady(true);
    },
    "_onRemoteNewElement": function(obj) {
        if (this.ready) {
            var vc = new Vocabulary();
            vc.insert("data", obj.clone());
            
            this.broadcast(vc, "push");
        }
    }
});

module.exports = List;
