"use strict";

var Proxy = require("./proxy");
var Vocabulary = require("../../wType/vocabulary");
var Ctrl = require("../../wController/list");

var List = Proxy.inherit({
    "className": "List",
    "constructor": function(address, controllerAddress, socket) {
        var controller = new Ctrl(controllerAddress);
        Proxy.fn.constructor.call(this, address, controller, socket);
        
        this.controller.on("data", this._onRemoteData, this);
        this.controller.on("clear", this._onRemoteClear, this);
        this.controller.on("newElement", this._onRemoteNewElement, this);
    },
    "_h_subscribe": function(ev) {
        Proxy.fn._h_subscribe.call(this, ev);
        
        if (this.ready) {
            this._h_get(ev);
        }
    },
    "_onRemoteClear": function() {
        if (this.ready) {
            this.broadcast(new Vocabulary(), "clear");
        }
    },
    "_onRemoteData": function() {
        this.setReady(true);
        
        var vc = new Vocabulary();
        vc.insert("data", this.controller.data.clone())
        this.broadcast(vc, "get")
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
