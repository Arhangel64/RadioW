"use strict";

var TempPage = require("./tempPage");
var String = require("../lib/wModel/string");

var VCController = require("../lib/wController/vocabulary");

var Address = require("../lib/wType/address");

var SongPage = TempPage.inherit({
    "className": "SongPage",
    "constructor": function(address, name, id, proxySocket) {
        TempPage.fn.constructor.call(this, address, name);
        
        this._remoteId = id;
        this._proxySocket = proxySocket;
        this._ctrls = Object.create(null);
        
        var header = this._header = new String(this._address["+"](new Address(["header"])), "");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, TempPage.Aligment.CenterTop);
        
        var album = this._album = new String(this._address["+"](new Address(["album"])), "Album name");
        album.addProperty("fontFamily", "casualFont");
        this.addItem(album, 1, 0, 1, 1, TempPage.Aligment.CenterTop);
        
        var artist = this._artist = new String(this._address["+"](new Address(["artist"])), "Artist name");
        artist.addProperty("fontFamily", "casualFont");
        this.addItem(artist, 2, 0, 1, 1, TempPage.Aligment.CenterTop);
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
                this._album.set(el);
                break;
        }
    },
    "_onAlbumRemoveElement": function(key) {
        switch(key) {
            case "name":
                this._album.set("unknown");
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
    "_onSongNewElement": function(key, el) {
        switch(key) {
            case "name":
                this._header.set(el);
                break;
            case "album":
                var aVC = new VCController(new Address(["albums", el.toString()]));
                aVC.on("newElement", this._onAlbumNewElement, this);
                aVC.on("removeElement", this._onAlbumRemoveElement, this);
                this._ctrls.album = aVC;
                aVC.register(this._dp, this._proxySocket);
                aVC.subscribe();
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
    "_onSongRemoveElement": function(key) {
        switch(key) {
            case "name":
                this._header.set("");
                break;
            case "album":
                this._album.set("");
                this._ctrls.album.destructor();
                delete this._ctrls.album;
            case "artist":
                this._artist.set("");
                this._ctrls.artist.destructor();
                delete this._ctrls.artist;
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
        
        var songVC = new VCController(new Address(["songs", this._remoteId.toString()]));
        songVC.on("newElement", this._onSongNewElement, this);
        songVC.on("removeElement", this._onSongRemoveElement, this);
        this._ctrls.song = songVC;
        songVC.register(this._dp, this._proxySocket);
        songVC.subscribe();
    },
    "unsetParentReporter": function() {
        this._clearCtrls();
        
        TempPage.fn.unsetParentReporter.call(this);
    }
});

module.exports = SongPage;
