
"use strict";

var Page = require("../lib/wModel/page");
var String = require("../lib/wModel/string");
var ProxyCatModel = require("../lib/wModel/proxy/catalogue");
var ProxyVCModel = require("../lib/wModel/proxy/vocabulary");

var Address = require("../lib/wType/address");
var counter = 0;

var List = Page.inherit({
    "className": "MusicPage",
    "constructor": function(address, name, remoteAddress, socket) {
        Page.fn.constructor.call(this, address, name);
        
        var header = new String(this._address["+"](new Address(["header"])), name);
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, Page.Aligment.CenterTop);

        this._list = new ProxyCatModel(
            this._address["+"](new Address(["list"])), 
            remoteAddress, 
            {
                sorting: {ascending: true, field: "name"}
            }, 
            socket
        );
        this._list.className = "PanesList";
        this._list.setChildrenClass(ProxyVCModel);
        this.addItem(this._list, 1, 0, 1, 1, Page.Aligment.CenterTop);
    },
    "destructor": function() {
        this.removeItem(this._list);
        this._list.destructor();
        
        Page.fn.destructor.call(this);
    },
    "setParentReporter": function(pr) {
        Page.fn.setParentReporter.call(this, pr);
        
        this._pr.registerParent(this._list.getAddress(), this._list.reporterHandler);
        this._list.subscribe();
    },
    "unsetParentReporter": function() {
        this._pr.unregisterParent(this._list.getAddress());
        
        Page.fn.unsetParentReporter.call(this);
    }
});

module.exports = List;
