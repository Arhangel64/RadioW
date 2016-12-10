"use strict";

var Controller = require("./controller");

var Link = Controller.inherit({
    "className": "Link",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this._targetAddress = undefined;
        this._text = undefined;
        
        this.addHandler("get");
    },
    "destructor": function() {
        for (var i = 0; i < this._views.length; ++i) {
            this._views[i].off("activate", this._onViewActivate, this);
        }
        
        Controller.fn.destructor.call(this);
    },
    "addView": function(view) {
        Controller.fn.addView.call(this, view);
        
        view.on("activate", this._onViewActivate, this);
    },
    "_h_get": function(ev) {
        var data = ev.getData();
        
        this._text = data.at("text").toString();
        this._targetAddress = data.at("targetAddress").clone();
        
        for (var i = 0; i < this._views.length; ++i) {
            this._views[i].data(this._text);
        }
    },
    "_onViewActivate": function() {
        var addr = this._targetAddress.clone();
        setTimeout(function() {                             //TODO post triggering!
            Link.changePage(addr);
        }, 1);
    }
});

Link.changePage = function(addr) {
    for (var i = 0; i < this._changePageHandlers.length; ++i) {
        this._changePageHandlers[i].mth.call(this._changePageHandlers[i].ctx, addr);
    }
}
Link._changePageHandlers = [];
Link.registerChangePageHandler = function(mth, ctx) {
    this._changePageHandlers.push({mth: mth, ctx: ctx});
}

module.exports = Link;
