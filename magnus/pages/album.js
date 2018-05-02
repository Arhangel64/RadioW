"use strict";

var TempPage = require("./tempPage");
var String = require("../lib/wModel/string");
var ProxyCatModel = require("../lib/wModel/proxy/catalogue");
var PaneModel = require("../lib/wModel/proxy/pane");
var Image = require("../lib/wModel/image");
var Model = require("../lib/wModel/model");

var VCController = require("../lib/wController/vocabulary");

var Address = require("../lib/wType/address");
var Uint64 = require("../lib/wType/uint64");
var Vocabulary = require("../lib/wType/vocabulary");

var AlbumPage = TempPage.inherit({
    "className": "AlbumPage",
    "constructor": function(address, name, id, proxySocket) {
        TempPage.fn.constructor.call(this, address, name);
        
        this._remoteId = id;
        this._proxySocket = proxySocket;
        this._ctrls = Object.create(null);
    
        this._image = new Image(this._address["+"](new Address(["image"])), new Uint64(0));
        var imageOptions = new Vocabulary();
        imageOptions.insert("minWidth", new Uint64(200));
        imageOptions.insert("maxWidth", new Uint64(200));
        imageOptions.insert("minHeight", new Uint64(200));
        imageOptions.insert("maxHeight", new Uint64(200));
        this.addItem(this._image, 0, 0, 2, 1, TempPage.Aligment.CenterCenter, new Uint64(TempPage.getModelTypeId(this._image)), imageOptions);
        
        var header = this._header = new String(this._address["+"](new Address(["header"])), "");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 1, 1, 1, TempPage.Aligment.LeftTop);
        
        var artist = this._artist = new String(this._address["+"](new Address(["artist"])), "Artist name");
        artist.addProperty("fontFamily", "casualFont");
        this.addItem(artist, 1, 1, 1, 1, TempPage.Aligment.LeftTop);
        
        var spacer = new Model(this._address["+"](new Address(["spacer"])));
        this.addItem(spacer, 0, 2, 2, 1);
        
        this._songs = new ProxyCatModel(
            this._address["+"](new Address(["songs"])), 
            new Address(["songs"]), 
            {
                sorting: {ascending: true, field: "name"},
                filter: {album: id.clone()}
            }, 
            proxySocket
        );
        this._songs.className = "PanesList";
        var PaneClass = PaneModel.Songs;
        this._songs.setChildrenClass(PaneClass);
        this.addItem(this._songs, 2, 0, 1, 3, TempPage.Aligment.CenterTop);
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
    "_onAlbumNewElement": function(key, el) {
        switch(key) {
            case "name":
                this._header.set(el);
                break;
            case "image":
                this._image.set(el.clone());
                break;
            case "artist":
                var arVC = new VCController(new Address(["artists", el.toString()]));
                arVC.on("newElement", this._onArtistNewElement, this);
                arVC.on("removeElement", this._onArtistRemoveElement, this);
                this._ctrls.artist = arVC;
                arVC.register(this._dp, this._proxySocket);
                arVC.subscribe();
                break;
        }
    },
    "_onAlbumRemoveElement": function(key) {
        switch(key) {
            case "name":
                this._header.set("");
                break;
            case "image":
                this._image.set(new Uint64(0));
                break;
            case "artist":
                this._artist.set("");
                this._ctrls.artist.destructor();
                delete this._ctrls.artist;
        }
    },
    "_onArtistNewElement": function(key, el) {
        switch(key) {
            case "name":
                this._artist.set(el);
                break;
        }
    },
    "_onArtistRemoveElement": function(key) {
        switch(key) {
            case "name":
                this._artist.set("unknown");
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
        
        this._pr.registerParent(this._songs.getAddress(), this._songs.reporterHandler);
        this._songs.subscribe();
        
        var albumVC = new VCController(new Address(["albums", this._remoteId.toString()]));
        albumVC.on("newElement", this._onAlbumNewElement, this);
        albumVC.on("removeElement", this._onAlbumRemoveElement, this);
        this._ctrls.album = albumVC;
        albumVC.register(this._dp, this._proxySocket);
        albumVC.subscribe();
    },
    "unsetParentReporter": function() {
        this._pr.unregisterParent(this._songs.getAddress());
        
        this._clearCtrls();
        
        TempPage.fn.unsetParentReporter.call(this);
    }
});

module.exports = AlbumPage;
