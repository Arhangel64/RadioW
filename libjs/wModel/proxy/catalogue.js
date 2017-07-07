"use strict";

var Proxy = require("./proxy");
var Vocabulary = require("../../wType/vocabulary");
var Vector = require("../../wType/vector");
var Ctrl = require("../../wController/catalogue");

var Catalogue = Proxy.inherit({
    "className": "List",            //no, it is not a typo, this is the way data structure here suppose to look like
    "constructor": function(address, ctrlAddr, ctrlOpts, socket) {
        var controller = new Ctrl(ctrlAddr, ctrlOpts);
        Proxy.fn.constructor.call(this, address, controller, socket);
        
        this.controller.on("data", this._onRemoteData, this);
        this.controller.on("addElement", this._onRemoteAddElement, this);
        //this.controller.on("removeElement", this._onRemoteRemoveElement, this); //not supported yet
    },
    "_getAllData": function() {
        var vec = new Vector();
        
        var itr = this.controller.data.begin();
        var end = this.controller.data.end();
        
        for (; !itr["=="](end); itr["++"]()) {
            vec.push(itr["*"]().clone());
        }
        
        return vec;
    },
    "_h_subscribe": function(ev) {
        Proxy.fn._h_subscribe.call(this, ev);
        
        if (this.ready) {
            this._h_get(ev);
        }
    },
    "_onRemoteData": function() {
        this.setReady(true);
        
        var vc = new Vocabulary();
        vc.insert("data", this._getAllData());
        this.broadcast(vc, "get")
    },
    "_onRemoteAddElement": function(obj) {
        if (this.ready) {                   //only end appends are supported now
            var vc = new Vocabulary();
            vc.insert("data", obj.clone());
            
            this.broadcast(vc, "push");
        }
    }
});

module.exports = Catalogue;
