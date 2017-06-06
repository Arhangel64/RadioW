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
            this.removePage(this._artists);
            this.removeItem(this._artistsLink);
            
            this._artists.destructor();
            this._artistsLink.destructor();
        } else {
            this.removeItem(this._errMessage);
        }
        this._errMessage.destructor();
        
        Page.fn.destructor.call(this);
    },
    "showError": function() {
        if (this._dbConnected) {
            if (!this._hasParentReporter) {
                throw new Error("Parent reporter is required in music page");
            }
            this.removePage(this._artists);
            this.removeItem(this._artistsLink);
            
            this._artists.destructor();
            this._artistsLink.destructor();
            
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
            
            var ra = new Address(["artists"]);
            var pa = this._address["+"](ra);
            this._artists = new List(pa, "Artists", ra, perturaboSocket);
            this._artistsLink = new Link(this._address["+"](new Address(["artistsLink"])), "Artists", pa.clone());
            this.addItem(this._artistsLink, 1, 0, 1, 1);
            this.addPage(this._artists);
            

            this._dbConnected = true;
        }
    }
});

module.exports = MusicPage;
