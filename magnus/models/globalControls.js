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
        
        setTimeout(this._initModels.bind(this), 1);
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
    }
});

module.exports = GlobalControls;