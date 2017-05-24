"use strict";

var PanesList = require("../lib/wModel/panesList");

var ListCtrl = require('../lib/wController/list');

var ProxyListModel = PanesList.inherit({
    "className": "ProxyListModel",
    "constructor": function(address, remoteAddress, socket) {
        PanesList.fn.constructor.call(address);
        
        this._socket = socket;
        var rl = this.remoteList = new ListCtrl(remoteAddress);
        
        this._uncyclic.push(function() {
            rl.destructor();
        })
    },
    "destructor": function() {
        PanesList.fn.destructor.call(this);
    },
    "register": function(dp, server) {
        PanesList.fn.register.call(this, dp, server);
        
        this.remoteList.register(dp, this._socket);
    },
    "unregister": function() {
        PanesList.fn.unregister.call(this);
        
        this.remoteList.unregister();
    }
});

module.exports = ProxyListModel;
