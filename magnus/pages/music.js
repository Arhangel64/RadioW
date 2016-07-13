"use strict";

var Page = require("../models/page");
var String = require("../models/string");
var PanesList = require("../models/panesList");

var Address = require("../lib/wType/address");
var counter = 0;

var MusicPage = Page.inherit({
    "className": "MusicPage",
    "constructor": function(address) {
        Page.fn.constructor.call(this, address);
        
        var header = new String(this._address["+"](new Address(["message"])), "Music");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, Page.Aligment.CenterTop);
        
        this._list = new PanesList(this._address["+"](new Address(["list"])));
        this.addItem(this._list, 1, 0, 1, 1, Page.Aligment.centerTop);
    },
    "addBand": function(name) {
        var c = counter++;
        c += "";
        var bandName = new String(this._address["+"](new Address([c])), name);
        this._list.addItem(bandName);
    }
});

module.exports = MusicPage;