"use strict";

var ModelVocabulary = require("../lib/wModel/vocabulary");

var Vocabulary = require("../lib/wType/vocabulary");
var String = require("../lib/wType/string");

var Commands = ModelVocabulary.inherit({
    "className": "Commands",
    "constructor": function(address) {
        ModelVocabulary.fn.constructor.call(this, address);
        
        this._commands = global.Object.create(null);
    },
    "destructor": function() {
        for (var key in this._commands) {
            var cmd = this._commands[key];
            if (cmd.enabled) {
                this._removeHandler(cmd.handler);
            }
            cmd.name.destructor();
            cmd.handler.destructor();
            cmd.arguments.destructor();
            delete this._commands[key];
        }
        
        ModelVocabulary.fn.destructor.call(this);
    },
    "addCommand": function(key, handler, args) {
        if (this._commands[key]) {
            throw new Error("Command with this key already exist");
        }
        this._commands[key] = {
            name: new String(key),
            handler: handler,
            arguments: args,
            enabled: false
        }
    },
    "_disableCommand": function(cmd) {
        this._remveHandler(cmd.handler);
        cmd.enabled = false;
        this.erase(cmd.name);
    },
    "enableCommand": function(key, value) {
        var cmd = this._commands[key];
        
        if (!cmd) {
            throw new Error("An attempt to access non existing command");
        }
    
        if (cmd.enabled !== value) {
            if (value) {
                this._enableCommand(cmd);
            } else {
                this._disableCommand(cmd);
            }
        }
    },
    "_enableCommand": function(cmd) {
        this._addHandler(cmd.handler);
        cmd.enabled = true;
        
        var vc = new Vocabulary();
        vc.insert("address", cmd.handler.address.clone());
        vc.insert("argumanets", cmd.arguments.clone());
        
        this.insert(cmd.name.clone(), vc);
    }
});

module.exports = Commands;
