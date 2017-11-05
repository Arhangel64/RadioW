"use strict";

var TempPage = require("./tempPage");
var String = require("../lib/wModel/string");
var ProxyCatModel = require("../lib/wModel/proxy/catalogue");
var PaneModel = require("../lib/wModel/proxy/pane");

var VCController = require("../lib/wController/vocabulary");

var Address = require("../lib/wType/address");

var ArtistPage = TempPage.inherit({
    "className": "ArtistPage",
    "constructor": function(address, name, id, proxySocket) {
        TempPage.fn.constructor.call(this, address, name);
        
        this._remoteId = id;
        this._proxySocket = proxySocket;
        this._ctrls = Object.create(null);
        
        var header = this._header = new String(this._address["+"](new Address(["header"])), "");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, TempPage.Aligment.CenterTop);
        
        this._albums = new ProxyCatModel(
            this._address["+"](new Address(["albums"])), 
            new Address(["albums"]), 
            {
                sorting: {ascending: true, field: "name"},
                filter: {artist: id.clone()}
            }, 
            proxySocket
        );
        this._albums.className = "PanesList";
        var PaneClass = PaneModel.Albums;
        this._albums.setChildrenClass(PaneClass);
        this.addItem(this._albums, 1, 0, 1, 1, TempPage.Aligment.CenterTop);
        
        this._songs = new ProxyCatModel(
            this._address["+"](new Address(["songs"])), 
            new Address(["songs"]), 
            {
                sorting: {ascending: true, field: "name"},
                filter: {artist: id.clone()}
            }, 
            proxySocket
        );
        this._songs.className = "PanesList";
        var PaneClass = PaneModel.Songs;
        this._songs.setChildrenClass(PaneClass);
        this.addItem(this._songs, 2, 0, 1, 1, TempPage.Aligment.CenterTop);
    },
    "destructor": function() {
        this._clearCtrls();
        
        TempPage.fn.destructor.call(this);
    },
    "_clearCtrls": function() {
        for (var key in this._ctrls) {
            this._ctrls[key].destructor();
        }
        
        this._ctrls = Object.create(null);
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
        
        for (var key in this._ctrls) {
            this._ctrls[key].register(dp, this._proxySocket);
        }
    },
    "setParentReporter": function(pr) {
        TempPage.fn.setParentReporter.call(this, pr);
        
        this._pr.registerParent(this._albums.getAddress(), this._albums.reporterHandler);
        this._pr.registerParent(this._songs.getAddress(), this._songs.reporterHandler);
        
        this._albums.subscribe()
        this._songs.subscribe();
        
        var artistVC = new VCController(new Address(["artists", this._remoteId.toString()]));
        artistVC.on("newElement", this._onArtistNewElement, this);
        artistVC.on("removeElement", this._onArtistRemoveElement, this);
        this._ctrls.artist = artistVC;
        artistVC.register(this._dp, this._proxySocket);
        artistVC.subscribe();
    },
    "unsetParentReporter": function() {
        this._pr.unregisterParent(this._albums.getAddress());
        this._pr.unregisterParent(this._songs.getAddress());
        
        this._clearCtrls();
        
        TempPage.fn.unsetParentReporter.call(this);
    }
});

module.exports = ArtistPage;
