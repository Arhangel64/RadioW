"use strict";

var Proxy = require("./proxy");
var Vocabulary = require("../../wType/vocabulary");
var Ctrl = require("../../wController/vocabulary");

var List = Proxy.inherit({
    "className": "List",
    "constructor": function(address, controllerAddress, socket) {
        var controller = new Ctrl(controllerAddress);
        Proxy.fn.constructor.call(this, address, controller, socket);
        
        this.controller.on("get", this._onRemoteGet, this);
        this.controller.on("clear", this._onRemoteClear, this);
        this.controller.on("change", this._onRemoteChange, this);
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
    "_onRemoteChange": function(data) {
        if (this.ready) {
            this.broadcast(data, "change");
        }
    }
});

module.exports = List;
