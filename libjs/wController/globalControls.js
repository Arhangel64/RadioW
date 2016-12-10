"use strict";

var List = require("./list");
var String = require("./string");
var NavigationPanel = require("./navigationPanel");
var ThemeSelecter = require("./themeSelecter");
var Theme = require("./theme");

var ViewString = require("../../views/string");
var ViewNavigationPanel = require("../../views/navigationPanel");

var GlobalControls = List.inherit({
    "className": "GlobalControls",
    "constructor": function(address, ml) {
        List.fn.constructor.call(this, address);
        
        this._layout = ml;
    },
    "_h_get": function(ev) {
        List.fn._h_get.call(this, ev);
        
        var size = this._data.size();
        for (var i = 0; i < size; ++i) {
            this._createControl(this._data.at(i));
        }
    },
    "_h_push": function(ev) {
        List.fn._h_push.call(this, ev);
        
        var i = this._data.size() - 1;
        this._createControl(this._data.at(i));
    },
    "_createControl": function(vc) {
        var name = vc.at("name").toString();
        var type = vc.at("type").toString();
        var addr = vc.at("address");
        
        switch (name) {
            case "version":
                var vm = new String(addr.clone());
                this.addController(vm);
                var vv = new ViewString({maxHeight: 15});
                vv.addProperty({p: "backgroundColor", k:"secondaryColor"});
                vv.addProperty({p: "color", k:"secondaryFontColor"});
                vv.addProperty({p: "fontFamily", k:"smallFont"});
                vv.addProperty({p: "fontSize", k:"smallFontSize"});
                this._layout.append(vv, 2, 0, 1, 3);
                vm.addView(vv);
                break;
            case "navigationPanel":
                var npm = new NavigationPanel(addr.clone());
                this.addController(npm);
                var vnp = new ViewNavigationPanel();
                this._layout.append(vnp, 0, 0, 1, 3);
                npm.addView(vnp);
                break;
            case "themes":
                var ts = new ThemeSelecter(addr.clone());
                this.addController(ts);
                ts.on("selected", this._onThemeSelected, this);
                break;
            default:
                this.trigger("Unsupported global control: " + name + " (" + type + ")", 1);
        }
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
