"use strict";

var Proxy = require("./proxy");
var Vocabulary = require("../../wType/vocabulary");
var Ctrl = require("../../wController/vocabulary");

var MVocabulary = Proxy.inherit({
    "className": "Vocabulary",
    "constructor": function(address, controllerAddress, socket) {
        var controller = new Ctrl(controllerAddress);
        Proxy.fn.constructor.call(this, address, controller, socket);
        
        this.controller.on("data", this._onRemoteData, this);
        this.controller.on("clear", this._onRemoteClear, this);
        this.controller.on("change", this._onRemoteChange, this);
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
        vc.insert("data", this._getAllData());
        this.broadcast(vc, "get")
    },
    "_onRemoteChange": function(data) {
        if (this.ready) {
            this.broadcast(data, "change");
        }
    }
});

module.exports = MVocabulary;
