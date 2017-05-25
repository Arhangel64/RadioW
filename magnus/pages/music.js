"use strict";

var Page = require("../lib/wModel/page");
var String = require("../lib/wModel/string");
var PanesList = require("../lib/wModel/panesList");

var Address = require("../lib/wType/address");
var Vocabulary = require("../lib/wType/vocabulary");
var Boolean = require("../lib/wType/boolean");

var ProxyListModel = require("../core/proxyListModel");
var counter = 0;

var MusicPage = Page.inherit({
    "className": "MusicPage",
    "constructor": function(address, parentReporter) {
        Page.fn.constructor.call(this, address);
        
        this._dbConnected = false;
        this._pr = parentReporter;
        
        var header = new String(this._address["+"](new Address(["header"])), "Music");
        header.addProperty("fontFamily", "casualFont");
        //var hvo = new Vocabulary();
        this.addItem(header, 0, 0, 1, 1, Page.Aligment.CenterTop);
        
        this._errMessage = new String(this._address["+"](new Address(["message"])), "Database is not connected");
        this._errMessage.addProperty("fontFamily", "largeFont");
        this._errMessage.addProperty("fontSize", "largeFontSize");
        
        
        this.addItem(this._errMessage, 1, 0, 1, 1, Page.Aligment.CenterTop);
        //this.addItem(this._list, 1, 0, 1, 1, Page.Aligment.centerTop);
    },
    "destructor": function() {
        if (this._dbConnected) {
            this.removeItem(this._list);
            this._list.destructor();
        } else {
            this.removeItem(this._errMessage);
        }
        this._errMessage.destructor();
        
        Page.fn.destructor.call(this);
    },
    "addBand": function(name) {
        var c = counter++;
        c += "";
        var bandName = new String(this._address["+"](new Address([c])), name);
        bandName.addProperty("fontFamily", "casualFont");
        this._list.addItem(bandName);
    },
    "showError": function() {
        if (this._dbConnected) {
            this.removeItem(this._list);
            this._list.destructor();
            this.addItem(this._errMessage, 1, 0, 1, 1, Page.Aligment.CenterTop);
            this._dbConnected = false;
        }
    },
    "showBandList": function(perturaboSocket) {
        if (!this._dbConnected) {
            this.removeItem(this._errMessage);
            var ra = new Address(["artists"]);
            this._list = new ProxyListModel(this._address["+"](new Address(["list"])), ra, perturaboSocket);
            this.addItem(this._list, 1, 0, 1, 1, Page.Aligment.CenterTop);
            this._list.subscribe();
            this._dbConnected = true;
        }
    }
});

module.exports = MusicPage;
