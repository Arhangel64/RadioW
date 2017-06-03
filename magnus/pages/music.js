"use strict";

var Page = require("../lib/wModel/page");
var String = require("../lib/wModel/string");
var PanesList = require("../lib/wModel/panesList");

var Address = require("../lib/wType/address");
var Vocabulary = require("../lib/wType/vocabulary");
var Boolean = require("../lib/wType/boolean");

var ProxyListModel = require("../lib/wModel/proxy/list");
var ProxyVCModel = require("../lib/wModel/proxy/vocabulary");
var counter = 0;

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
        //this.addItem(this._list, 1, 0, 1, 1, Page.Aligment.centerTop);
    },
    "destructor": function() {
        if (this._dbConnected && this._hasParentReporter) {
            this._pr.unregisterParent(this._list.getAddress());
            this.removeItem(this._list);
            this._list.destructor();
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
            this._pr.unregisterParent(this._list.getAddress());
            this.removeItem(this._list);
            this._list.destructor();
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
            this._list = new ProxyListModel(this._address["+"](new Address(["bands"])), ra, perturaboSocket);
            this._list.className = "PanesList";
            this._list.setChildrenClass(ProxyVCModel);
            this.addItem(this._list, 1, 0, 1, 1, Page.Aligment.CenterTop);
            this._pr.registerParent(this._list.getAddress(), this._list.reporterHandler);
            this._list.subscribe();
            this._dbConnected = true;
        }
    }
});

module.exports = MusicPage;
