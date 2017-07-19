"use strict";

var TempPage = require("./tempPage");
var String = require("../lib/wModel/string");

var VCController = require("../lib/wController/vocabulary");

var Address = require("../lib/wType/address");

var SongPage = TempPage.inherit({
    "className": "SongPage",
    "constructor": function(address, name, remoteAddress, proxySocket) {
        TempPage.fn.constructor.call(this, address, name);
        
        this._proxySocket = proxySocket;
        
        var ctrl = this._songVC = new VCController(remoteAddress);
        ctrl.on("newElement", this._onSongNewElement, this);
        ctrl.on("removeElement", this._onSongRemoveElement, this);
        
        var header = this._header = new String(this._address["+"](new Address(["header"])), "");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, TempPage.Aligment.CenterTop);
        
        this._uncyclic.push(function() {
            ctrl.destructor();
            ctrl = undefined;
        });
    },
    "_onSongNewElement": function(key, el) {
        switch(key) {
            case "name":
                this._header.set(el);
                break;
        }
    },
    "_onSongRemoveElement": function(key) {
        switch(key) {
            case "name":
                this._header.set("");
                break;
        }
    },
    "register": function(dp, server) {
        TempPage.fn.register.call(this, dp, server);
        
        this._songVC.register(dp, this._proxySocket);
    },
    "setParentReporter": function(pr) {
        TempPage.fn.setParentReporter.call(this, pr);
        
        this._songVC.subscribe();
    },
    "unsetParentReporter": function() {
        TempPage.fn.unsetParentReporter.call(this);
        
        if (this._songVC._subscribed) {
            this._songVC.unsubscribe();
        }
        this._songVC.unregister();
    }
});

module.exports = SongPage;
