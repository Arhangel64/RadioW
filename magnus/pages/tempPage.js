"use strict";

var Page = require("../lib/wModel/page");

var config = require("../config/config.json");

var TempPage = Page.inherit({
    "className": "TempPage",
    "constructor": function(address, name) {
        Page.fn.constructor.call(this, address, name);
        
        this._destructionTimeout = undefined;
    },
    "destructor": function() {
        if (this._destructionTimeout) {
            clearTimeout(this._destructionTimeout);
        }
        Page.fn.destructor.call(this);
    },
    "checkSubscribersAndDestroy": function() {
        if (this._subscribersCount === 0 && this._destructionTimeout === undefined) {
            this.trigger("serviceMessage", this._address.toString() + " has no more subscribers, destroying page");
            this._destructionTimeout = setTimeout(this.trigger.bind(this, "destroyMe"), config.modelDestructionTimeout);
        }
    },
    "_h_subscribe": function(ev) {
        Page.fn._h_subscribe.call(this, ev);
        
        if (this._destructionTimeout !== undefined) {
            clearTimeout(this._destructionTimeout);
            this._destructionTimeout = undefined;
        }
    },
    "_h_unsubscribe": function(ev) {
        Page.fn._h_unsubscribe.call(this, ev);
        
        this.checkSubscribersAndDestroy();
    },
    "_onSocketDisconnected": function(ev, socket) {
        Page.fn._onSocketDisconnected.call(this, ev, socket);
        
        this.checkSubscribersAndDestroy();
    }
});

module.exports = TempPage;
