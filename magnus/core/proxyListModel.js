"use strict";

var PanesList = require("../lib/wModel/panesList");

var ListCtrl = require('../lib/wController/list');

var ProxyListModel = PanesList.inherit({
    "className": "ProxyListModel",
    "constructor": function(address, remoteAddress) {
        PanesList.fn.constructor.call(address);
        
        this.remoteList = new ListCtrl(remoteAddress);
    }
});

module.exports = ProxyListModel;
