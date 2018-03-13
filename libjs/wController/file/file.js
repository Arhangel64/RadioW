"use strict";

var Controller = require("../controller");
var WVocabulary = require("../../wType/vocabulary");

var File = Controller.inherit({
    "className": "File",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this._hasData = false;
        this._hasSize = false;
        this.data = null;
        this._size = null;
        
        this.addHandler("get");
        this.addHandler("getAdditional");
    },
    "destructor": function() {
        if (this._hasData) {
            this.data.destructor();
        }
        if (this._hasSize) {
            this._size.destructor();
        }
        Controller.fn.destructor.call(this);
    },
    "requestData": function() {
        var vc = new WVocabulary();
        
        this.send(vc, "get");
    },
    "hasData": function() {
        return this._hasData;
    },
    "_h_get": function(ev) {
        var dt = ev.getData();
        
        var ac = this._getAdditional(dt.at("additional"));
        if (ac) {
            this.trigger("additionalChange")
        }
        
        this._hasData = true;
        this.data = dt.at("data").clone();
        this.trigger("data");
    },
    "_h_getAdditional": function(ev) {
        var ac = this._getAdditional(ev.getData());
        if (ac) {
            this.trigger("additionalChange")
        }
    },
    "_getAdditional": function(vc) {
        var change = this._size === null;
        var os = this._size;
        this._size = vc.at("size").clone();
        this._hasSize = true;
        if (!change) {
            change = !(os["=="](this._size));
        }
        
        return change;
    }
});

module.exports = File;
