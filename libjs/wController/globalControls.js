"use strict";

var List = require("./list");
var String = require("./string");
var NavigationPanel = require("./navigationPanel");
var ThemeSelecter = require("./themeSelecter");
var Theme = require("./theme");

var GlobalControls = List.inherit({
    "className": "GlobalControls",
    "constructor": function(address) {
        List.fn.constructor.call(this, address);
    },
    "addElement": function(vc) {
        List.fn.addElement.call(this, vc);
        
        var name = vc.at("name").toString();
        var type = vc.at("type").toString();
        var addr = vc.at("address");
        var ctrl;
        var supported = true;
        
        switch (name) {
            case "version":
                ctrl = new String(addr.clone());
                break;
            case "navigationPanel":
                ctrl = new NavigationPanel(addr.clone());
                break;
            case "themes":
                ctrl = new ThemeSelecter(addr.clone());
                ctrl.on("selected", this._onThemeSelected, this);
                break;
            default:
                supported = false;
                this.trigger("serviceMessage", "Unsupported global control: " + name + " (" + type + ")", 1);
                break;
        }
        if (supported) {
            ctrl.name = name;
            this.addController(ctrl);
        }
    },
    "clear": function() {
        List.fn.clear.call(this);
        this.clearChildren();
    },
    "_onThemeReady": function(theme) {
        this.trigger("themeSelected", theme._data);
        this.removeController(theme);
        theme.destructor();
    },
    "_onThemeSelected": function(obj) {
        var theme = new Theme(obj.value.clone());
        this.addController(theme);
        theme.on("ready", this._onThemeReady.bind(this, theme));
    }
});

module.exports = GlobalControls;
