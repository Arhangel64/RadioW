"use strict";

var Page = require("../lib/wModel/page");
var String = require("../lib/wModel/string");
var Button = require("../lib/wModel/button");

var Address = require("../lib/wType/address");

var TestPage = Page.inherit({
    "className": "TestPage",
    "constructor": function(address, name) {
        Page.fn.constructor.call(this, address, name);
        
        var header = new String(this._address["+"](new Address(["message"])), "This is a test page");
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, Page.Aligment.CenterTop);
        
        this._button = new Button(this._address["+"](new Address(["testButton"])));
        this._button.setLabel("Push me");
        this._button.on("activated", this._onActivate, this);
        this.addItem(this._button, 1, 0, 1, 1, Page.Aligment.CenterTop);
    },
    "_onActivate": function() {
        this.trigger("serviceMessage", "Button works!");
        this._button.setEnabled(false);
        setTimeout(this._button.setEnabled.bind(this._button, true), 3000);
    }
});

module.exports = TestPage;
