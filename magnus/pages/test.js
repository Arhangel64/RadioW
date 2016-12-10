"use strict";

var Page = require("../lib/wModel/page");
var String = require("../lib/wModel/string");

var Address = require("../lib/wType/address");

var TestPage = Page.inherit({
    "className": "TestPage",
    "constructor": function(address) {
        Page.fn.constructor.call(this, address);
        
        var header = new String(this._address["+"](new Address(["message"])), "This is a test page");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, Page.Aligment.CenterTop);
    }
});

module.exports = TestPage;
