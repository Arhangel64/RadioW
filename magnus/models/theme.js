"use strict";

var Model = require("./model");
var Vocabulary = require("../lib/wType/vocabulary");
var String = require("../lib/wType/string");
var Uint64 = require("../lib/wType/uint64");

var Theme = Model.inherit({
    "className": "Theme",
    "constructor": function(address, name, theme) {
        Model.fn.constructor.call(this, address);
        
        this._themeName = name;
        var result = {};
        W.extend(result, Theme.default, theme);
        
        var data = new Vocabulary();
        
        for (var key in result) {
            if (result.hasOwnProperty(key)) {
                var type = typeof result[key];
                switch (type) {
                    case "number":
                        data.insert(key, new Uint64(result[key]));
                        break;
                    default:
                        data.insert(key, new String(result[key]));
                        break;
                }
            }
        }
        
        this._data = data;
        
        this.addHandler("get");
    },
    "getName": function() {
        return this._themeName;
    },
    "_h_get": function(ev) {
        var vc = new Vocabulary();
        
        vc.insert("data", this._data.clone());
        vc.insert("name", new String(this._themeName));
        this.response(vc, "get", ev);
    },
    "_h_subscribe": function(ev) {
        Model.fn._h_subscribe.call(this, ev);
        
        this._h_get(ev);
    }
});

Theme.default = {
    mainColor: "#ffffff",
    mainFontColor: "#222222",
    primaryColor: "#0000ff",
    primaryFontColor: "#ffffff",
    secondaryColor: "#dddddd",
    secondaryFontColor: "#222222",
    
    smallFont: "Liberation",
    smallFontSize: "12px",
    casualFont: "Liberation",
    casualFontSize: "16px",
    largeFont: "Liberation",
    largeFontSize: "20px"
}

module.exports = Theme;