"use strict";

var Page = require("../models/page");
var String = require("../models/string");

var Address = require("../lib/wType/address");

var MusicPage = Page.inherit({
    "className": "MusicPage",
    "constructor": function(address) {
        Page.fn.constructor.call(this, address);
        
        var header = new String(this._address["+"](new Address(["message"])), "Music");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, Page.CenterTop);
    }
});

module.exports = MusicPage;