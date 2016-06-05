"use strict";

var List = require("./list");
var Model = require("./model");
var ModelString = require("./string");
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
        this.addModel(navigationPanel, "navigationPanel");
    },
    "addNav": function(name, address) {
        var vc = new Vocabulary();
        
        vc.insert("address", address);
        vc.insert("text", new String(name));
        
        //var model = new ModelString(this._np._address["+"](new Address(["" + this._np._data.size()])));
        //this._np.addModel(model);
        this._np.push(vc);
    }
});

module.exports = GlobalControls;