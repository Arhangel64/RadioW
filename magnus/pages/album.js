"use strict";

var TempPage = require("./tempPage");
var String = require("../lib/wModel/string");

var VCController = require("../lib/wController/vocabulary");

var Address = require("../lib/wType/address");

var AlbumPage = TempPage.inherit({
    "className": "AlbumPage",
    "constructor": function(address, name, remoteAddress, proxySocket) {
        TempPage.fn.constructor.call(this, address, name);
        
        this._proxySocket = proxySocket;
        
        var ctrl = this._albumVC = new VCController(remoteAddress);
        ctrl.on("newElement", this._onAlbumNewElement, this);
        ctrl.on("removeElement", this._onAlbumRemoveElement, this);
        
        var header = this._header = new String(this._address["+"](new Address(["header"])), "");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, TempPage.Aligment.CenterTop);
        
        this._uncyclic.push(function() {
            ctrl.destructor();
            ctrl = undefined;
        });
    },
    "_onAlbumNewElement": function(key, el) {
        switch(key) {
            case "name":
                this._header.set(el);
                break;
        }
    },
    "_onAlbumRemoveElement": function(key) {
        switch(key) {
            case "name":
                this._header.set("");
                break;
        }
    },
    "register": function(dp, server) {
        TempPage.fn.register.call(this, dp, server);
        
        this._albumVC.register(dp, this._proxySocket);
    },
    "setParentReporter": function(pr) {
        TempPage.fn.setParentReporter.call(this, pr);
        
        this._albumVC.subscribe();
    },
    "unsetParentReporter": function() {
        TempPage.fn.unsetParentReporter.call(this);
        
        if (this._albumVC._subscribed) {
            this._albumVC.unsubscribe();
        }
        this._albumVC.unregister();
    }
});

module.exports = AlbumPage;
