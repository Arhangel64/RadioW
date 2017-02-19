"use strict";

var Page = require("../lib/wModel/page");
var String = require("../lib/wModel/string");
var PanesList = require("../lib/wModel/panesList");

var Address = require("../lib/wType/address");
var Vocabulary = require("../lib/wType/vocabulary");
var Boolean = require("../lib/wType/boolean");
var counter = 0;

var MusicPage = Page.inherit({
    "className": "MusicPage",
    "constructor": function(address) {
        Page.fn.constructor.call(this, address);
        
        this._dbConnected = false;
        
        var header = new String(this._address["+"](new Address(["header"])), "Music");
        header.addProperty("fontFamily", "casualFont");
        var hvo = new Vocabulary();
        hvo.insert("singleLine", new Boolean(true));
        this.addItem(header, 0, 0, 1, 1, Page.Aligment.CenterTop, hvo);
        
        this._list = new PanesList(this._address["+"](new Address(["list"])));
        this._errMessage = new String(this._address["+"](new Address(["message"])), "Database is not connected");
        this._errMessage.addProperty("fontFamily", "largeFont");
        this._errMessage.addProperty("fontSize", "largeFontSize");
        this._errMessageOptions = new Vocabulary();
        this._errMessageOptions.insert("singleLine", new Boolean(true));
        this.addItem(this._errMessage, 1, 0, 1, 1, Page.Aligment.CenterTop, this._errMessageOptions);
        
        
        //this.addItem(this._list, 1, 0, 1, 1, Page.Aligment.centerTop);
    },
    "addBand": function(name) {
        var c = counter++;
        c += "";
        var bandName = new String(this._address["+"](new Address([c])), name);
        bandName.addProperty("fontFamily", "casualFont");
        this._list.addItem(bandName);
    }
});

module.exports = MusicPage;
