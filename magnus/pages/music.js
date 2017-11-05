"use strict";

var Page = require("../lib/wModel/page");
var String = require("../lib/wModel/string");
var PanesList = require("../lib/wModel/panesList");

var Address = require("../lib/wType/address");
var Vocabulary = require("../lib/wType/vocabulary");
var Boolean = require("../lib/wType/boolean");

var Link = require("../lib/wModel/link");

var List = require("./list");
var Artist = require("./artist");
var Album = require("./album");
var Song = require("./song");

var PaneModel = require("../lib/wModel/proxy/pane");

var MusicPage = Page.inherit({
    "className": "MusicPage",
    "constructor": function(address, name) {
        Page.fn.constructor.call(this, address, name);
        
        this._dbConnected = false;
        this._addresses = Object.create(null);
        
        this._createAddresses();
        
        var header = new String(this._address["+"](new Address(["header"])), "Music");
        header.addProperty("fontFamily", "casualFont");
        //var hvo = new Vocabulary();
        this.addItem(header, 0, 0, 1, 1, Page.Aligment.CenterTop);
        
        this._errMessage = new String(this._address["+"](new Address(["message"])), "Database is not connected");
        this._errMessage.addProperty("fontFamily", "largeFont");
        this._errMessage.addProperty("fontSize", "largeFontSize");
        
        this.addItem(this._errMessage, 1, 0, 1, 1, Page.Aligment.CenterTop);
    },
    "destructor": function() {
        if (this._dbConnected && this._hasParentReporter) {
            this._destroyLists();
        } else {
            this.removeItem(this._errMessage);
        }
        this._errMessage.destructor();
        
        for (var tag in this._addresses) {
            var group = this._addresses[tag];
            for (var name in group) {
                group[name].destructor();
            }
        }
        
        Page.fn.destructor.call(this);
    },
    "_createAddresses": function() {
        var ra = new Address(["artists"]);
        var ral = new Address(["albums"]);
        var rs = new Address(["songs"]);
        
        var artists = Object.create(null);
        var albums = Object.create(null);
        var songs = Object.create(null);
        
        artists.remote = ra.clone();
        artists.local = this._address["+"](ra);
        
        albums.remote = ral.clone();
        albums.local = this._address["+"](ral);
        
        songs.remote = rs.clone();
        songs.local = this._address["+"](rs);
        
        this._addresses.artists = artists;
        this._addresses.albums = albums;
        this._addresses.songs = songs;
        
        var PaneArtist = PaneModel.Artists;
        if (!PaneArtist) {
            PaneArtist = PaneModel.inherit({});
            PaneModel.Artists = PaneArtist;
        } else {
            PaneArtist.pageAddress.destructor()
        }
        PaneArtist.pageAddress = artists.local.clone();
        
        var PaneAlbum = PaneModel.Albums;
        if (!PaneAlbum) {
            PaneAlbum = PaneModel.inherit({});
            PaneModel.Albums = PaneAlbum;
        } else {
            PaneAlbum.pageAddress.destructor()
        }
        PaneAlbum.pageAddress = albums.local.clone();
        
        var PaneSongs = PaneModel.Songs;
        if (!PaneSongs) {
            PaneSongs = PaneModel.inherit({});
            PaneModel.Songs = PaneSongs;
        } else {
            PaneSongs.pageAddress.destructor()
        }
        PaneSongs.pageAddress = songs.local.clone();
        
        ra.destructor();
        ral.destructor();
        rs.destructor();
    },
    "_createLists": function(socket) {
        this._artists = new List(
            this._addresses.artists.local.clone(), 
            "Artists", 
            this._addresses.artists.remote.clone(), 
            socket, 
            Artist
        );
        this._artistsLink = new Link(this._address["+"](new Address(["artistsLink"])), "Artists", this._addresses.artists.local.clone());
        this._artistsLink.label.addProperty("fontSize", "largeFontSize");
        this._artistsLink.label.addProperty("fontFamily", "largeFont");
        this._artistsLink.label.addProperty("color", "primaryFontColor");
        this._artistsLink.addProperty("backgroundColor", "primaryColor");
        
        this._albums = new List(
            this._addresses.albums.local.clone(), 
            "Albums", 
            this._addresses.albums.remote.clone(), 
            socket, 
            Album
        );
        this._albumsLink = new Link(this._address["+"](new Address(["albumsLink"])), "Albums", this._addresses.albums.local.clone());
        this._albumsLink.label.addProperty("fontSize", "largeFontSize");
        this._albumsLink.label.addProperty("fontFamily", "largeFont");
        this._albumsLink.label.addProperty("color", "primaryFontColor");
        this._albumsLink.addProperty("backgroundColor", "primaryColor");
        
        this._songs = new List(
            this._addresses.songs.local.clone(), 
            "Songs", 
            this._addresses.songs.remote.clone(), 
            socket, 
            Song
        );
        this._songsLink = new Link(this._address["+"](new Address(["songsLink"])), "Songs", this._addresses.songs.local.clone());
        this._songsLink.label.addProperty("fontSize", "largeFontSize");
        this._songsLink.label.addProperty("fontFamily", "largeFont");
        this._songsLink.label.addProperty("color", "primaryFontColor");
        this._songsLink.addProperty("backgroundColor", "primaryColor");
        
        this.addItem(this._artistsLink, 1, 0, 1, 1);
        this.addItem(this._albumsLink, 2, 0, 1, 1);
        this.addItem(this._songsLink, 3, 0, 1, 1);
        
        this.addPage(this._artists);
        this.addPage(this._albums);
        this.addPage(this._songs);
    },
    "_destroyLists": function() {
        this.removePage(this._artists);
        this.removePage(this._albums);
        this.removePage(this._songs);
        
        this.removeItem(this._artistsLink);
        this.removeItem(this._albumsLink);
        this.removeItem(this._songsLink);
        
        this._artists.destructor();
        this._albums.destructor();
        this._songs.destructor();
        
        this._artistsLink.destructor();
        this._albumsLink.destructor();
        this._songsLink.destructor();
    },
    "showError": function() {
        if (this._dbConnected) {
            if (!this._hasParentReporter) {
                throw new Error("Parent reporter is required in music page");
            }
            this._destroyLists()
            this.addItem(this._errMessage, 1, 0, 1, 1, Page.Aligment.CenterTop);
            this._dbConnected = false;
        }
    },
    "showBandList": function(perturaboSocket) {
        if (!this._hasParentReporter) {
            throw new Error("Parent reporter is required in music page");
        }
        if (!this._dbConnected) {
            this.removeItem(this._errMessage);
            this._createLists(perturaboSocket);
            this._dbConnected = true;
        }
    }
});

module.exports = MusicPage;
