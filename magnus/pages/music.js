"use strict";

var Page = require("../lib/wModel/page");
var String = require("../lib/wModel/string");
var PanesList = require("../lib/wModel/panesList");

var Address = require("../lib/wType/address");
var Vocabulary = require("../lib/wType/vocabulary");
var Boolean = require("../lib/wType/boolean");

var Link = require("../lib/wModel/link");

var List = require("./list");

var MusicPage = Page.inherit({
    "className": "MusicPage",
    "constructor": function(address, name) {
        Page.fn.constructor.call(this, address, name);
        
        this._dbConnected = false;
        
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
        
        Page.fn.destructor.call(this);
    },
    "_createLists": function(socket) {
        var ra = new Address(["artists"]);
        var pa = this._address["+"](ra);
        this._artists = new List(pa, "Artists", ra, socket);
        this._artistsLink = new Link(this._address["+"](new Address(["artistsLink"])), "Artists", pa.clone());
        this._artistsLink.label.addProperty("fontSize", "largeFontSize");
        this._artistsLink.label.addProperty("fontFamily", "largeFont");
        this._artistsLink.label.addProperty("color", "primaryFontColor");
        this._artistsLink.addProperty("backgroundColor", "primaryColor");
        
        
        var ral = new Address(["albums"]);
        var pal = this._address["+"](ral);
        this._albums = new List(pal, "Albums", ral, socket);
        this._albumsLink = new Link(this._address["+"](new Address(["albumsLink"])), "Albums", pal.clone());
        this._albumsLink.label.addProperty("fontSize", "largeFontSize");
        this._albumsLink.label.addProperty("fontFamily", "largeFont");
        this._albumsLink.label.addProperty("color", "primaryFontColor");
        this._albumsLink.addProperty("backgroundColor", "primaryColor");
        
        var rs = new Address(["songs"]);
        var ps = this._address["+"](rs);
        this._songs = new List(ps, "Songs", rs, socket);
        this._songsLink = new Link(this._address["+"](new Address(["songsLink"])), "Songs", ps.clone());
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
