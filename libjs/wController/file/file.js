"use strict";

var Controller = require("../controller");
var WVocabulary = require("../../wType/vocabulary");

var File = Controller.inherit({
    "className": "File",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this._hasData = false;
        this._hasAdditional = false;
        this.data = null;
        this._additional = null;
        
        this.addHandler("get");
        this.addHandler("getAdditional");
    },
    "destructor": function() {
        if (this._hasData) {
            this.data.destructor();
        }
        if (this._hasAdditional) {
            this._additional.destructor();
        }
        Controller.fn.destructor.call(this);
    },
    "_getAdditional": function(add) {
        var ac = !this._hasAdditional || !this._additional["=="](add);
        if (ac) {
            if (this._hasAdditional) {
                this._additional.destructor();
            }
            this._additional = add.clone();
        }
        this._hasAdditional = true;
        return ac;
    },
    "getMimeType": function() {
        return this._additional.at("mimeType").toString();
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
            this.trigger("additionalChange");
        }
    },
    "requestData": function() {
        var vc = new WVocabulary();
        
        this.send(vc, "get");
    }
});

module.exports = File;
