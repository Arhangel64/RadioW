"use strict";

var Page = require("../lib/wModel/page");
var String = require("../lib/wModel/string");

var Address = require("../lib/wType/address");

var HomePage = Page.inherit({
    "className": "HomePage",
    "constructor": function(address, name) {
        Page.fn.constructor.call(this, address, name);
        
        var header = new String(this._address["+"](new Address(["message"])), "This is the root page");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, Page.Aligment.CenterTop);
    }
});

module.exports = HomePage;
