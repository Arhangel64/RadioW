"use strict";
var List = require("./list");

var String = require("./string");
var ViewString = require("../../views/string");

var PanesList = List.inherit({
    "className": "PanesList",
    "constructor": function PanesListModel(addr) {
        List.fn.constructor.call(this, addr);
    },
    "addController": function(ctrl) {
        List.fn.addController.call(this, ctrl);
        
        if (this._views.length) {
            //var index = this._controllers.indexOf(ctrl)
        
            for (var i = 0; i < this._views.length; ++i) {
                var view = new ViewString({singleLine: true});
                ctrl.addView(view)
                this._views[i].append(view);
            }
        }
    },
    "addElement": function(element) {
        var address = element.at("address").clone();
        
        var controller = new String(address);
        this.addController(controller);
        
        List.fn.addElement.call(this, element);
    },
    "addView": function(view) {
        List.fn.addView.call(this, view);
        
        for (var i = 0; i < this._controllers.length; ++i) {
            var el = new ViewString();
            this._controllers[i].addView(el)
            view.append(el);
        }
    }
});

module.exports = PanesList;
