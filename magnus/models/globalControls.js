"use strict";

var List = require("./list");
var Model = require("./model");
var ModelString = require("./string");
var ModelLink = require("./link");
var ThemeStorage = require("./themeStorage");
var Vocabulary = require("../lib/wType/vocabulary");
var Address = require("../lib/wType/address");
var String = require("../lib/wType/string");

var GlobalControls = List.inherit({
    "className": "GlobalControls",
    "constructor": function(address) {
        List.fn.constructor.call(this, address);
        
        this._initModels()
    },
    "addModel": function(model, name) {
        List.fn.addModel.call(this, model);
        
        var vc = new Vocabulary();
        vc.insert("type", new String(model.className));
        vc.insert("address", model.getAddress());
        vc.insert("name", new String(name));
        
        this.push(vc);
    },
    "_initModels": function() {
        var version = new ModelString(this._address["+"](new Address(["version"])), global.magnus._cfg.get("version"));
        this.addModel(version, "version");
        
        var navigationPanel = this._np = new List(this._address["+"](new Address(["navigationPanel"])));
        
        navigationPanel.addProperty("backgroundColor", "primaryColor");
        this.addModel(navigationPanel, "navigationPanel");
        
        var ts = new ThemeStorage(this._address["+"](new Address(["themes"])));
        this.addModel(ts, "themes");
    },
    "addNav": function(name, address) {
        var vc = new Vocabulary();
        
        var model = new ModelLink(this._np._address["+"](new Address(["" + this._np._data.size()])), name, address);
        model.addProperty("fontSize", "largeFontSize");
        model.addProperty("fontFamily", "largeFont");
        model.addProperty("color", "primaryFontColor");
        this._np.addModel(model);
        
        vc.insert("address", model.getAddress());
        this._np.push(vc);
    }
});

module.exports = GlobalControls;