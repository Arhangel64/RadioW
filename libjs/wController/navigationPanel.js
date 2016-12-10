"use strict";
var List = require("./list");
var Link = require("./link");
var ViewLink = require("../../views/nav");

var NavigationPanel = List.inherit({
    "className": "NavigationPanel",
    "constructor": function(addr) {
        List.fn.constructor.call(this, addr);
    },
    "addController": function(ctrl) {
        List.fn.addController.call(this, ctrl);
        
        if (this._views.length) {
        
            for (var i = 0; i < this._views.length; ++i) {
                var view = new ViewLink();
                ctrl.addView(view)
                this._views[i].push(view);
            }
        }
    },
    "addElement": function(element) {
        var address = element.at("address").clone();
        
        var ctrl = new Link(address);
        this.addController(ctrl);
        
        List.fn.addElement.call(this, element);
    },
    "addView": function(view) {
        List.fn.addView.call(this, view);
        
        for (var i = 0; i < this._controllers.length; ++i) {
            var link = new ViewLink();
            this._controllers[i].addView(link)
            view.push(link);
        }
    }
});

module.exports = NavigationPanel;
