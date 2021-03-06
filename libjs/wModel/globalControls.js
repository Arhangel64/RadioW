"use strict";

var List = require("./list");
var Model = require("./model");
var ModelString = require("./string");
var ModelLink = require("./link");
var ThemeStorage = require("./themeStorage");
var Vocabulary = require("../wType/vocabulary");
var Address = require("../wType/address");
var String = require("../wType/string");

var GlobalControls = List.inherit({
    "className": "GlobalControls",
    "constructor": function(address) {
        List.fn.constructor.call(this, address);
        
        this._initModels()
    },
    "addModel": function(name, model) {
        List.fn.addModel.call(this, model);
        
        var vc = new Vocabulary();
        vc.insert("type", new String(model.className));
        vc.insert("address", model.getAddress());
        vc.insert("name", new String(name));
        
        this.push(vc);
    },
    "addModelAsLink": function(name, model) {
        var vc = new Vocabulary();
        vc.insert("type", new String(model.className));
        vc.insert("address", model.getAddress());
        vc.insert("name", new String(name));
        
        this.push(vc);
    },
    "_initModels": function() {
        var navigationPanel = this._np = new List(this._address["+"](new Address(["navigationPanel"])));
        
        navigationPanel.addProperty("backgroundColor", "primaryColor");
        this.addModel("navigationPanel", navigationPanel);
        
        var ts = new ThemeStorage(this._address["+"](new Address(["themes"])));
        this.addModel("themes", ts);
    },
    "addNav": function(name, address) {
        var vc = new Vocabulary();
        
        var model = new ModelLink(this._np._address["+"](new Address(["" + this._np._data.length()])), name, address);
        model.label.addProperty("fontSize", "largeFontSize");
        model.label.addProperty("fontFamily", "largeFont");
        model.label.addProperty("color", "primaryFontColor");
        this._np.addModel(model);
        
        vc.insert("address", model.getAddress());
        this._np.push(vc);
    }
});

module.exports = GlobalControls;
