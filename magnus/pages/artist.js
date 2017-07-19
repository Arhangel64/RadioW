"use strict";

var TempPage = require("./tempPage");
var String = require("../lib/wModel/string");

var VCController = require("../lib/wController/vocabulary");

var Address = require("../lib/wType/address");

var ArtistPage = TempPage.inherit({
    "className": "ArtistPage",
    "constructor": function(address, name, remoteAddress, proxySocket) {
        TempPage.fn.constructor.call(this, address, name);
        
        this._proxySocket = proxySocket;
        
        var ctrl = this._artistVC = new VCController(remoteAddress);
        ctrl.on("newElement", this._onArtistNewElement, this);
        ctrl.on("removeElement", this._onArtistRemoveElement, this);
        
        var header = this._header = new String(this._address["+"](new Address(["header"])), "");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, TempPage.Aligment.CenterTop);
        
        this._uncyclic.push(function() {
            ctrl.destructor();
            ctrl = undefined;
        });
    },
    "_onArtistNewElement": function(key, el) {
        switch(key) {
            case "name":
                this._header.set(el);
                break;
        }
    },
    "_onArtistRemoveElement": function(key) {
        switch(key) {
            case "name":
                this._header.set("");
                break;
        }
    },
    "register": function(dp, server) {
        TempPage.fn.register.call(this, dp, server);
        
        this._artistVC.register(dp, this._proxySocket);
    },
    "setParentReporter": function(pr) {
        TempPage.fn.setParentReporter.call(this, pr);
        
        this._artistVC.subscribe();
    },
    "unregister": function() {
        TempPage.fn.unregister.call(this);
        
        if (this._artistVC._subscribed) {
            this._artistVC.unsubscribe();
        }
        this._artistVC.unregister();
    }
});

module.exports = ArtistPage;
