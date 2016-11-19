"use strict";
(function pageStorage_js() {
    var moduleName = "models/pageStorage";
    
    var defineArray = [];
    defineArray.push("models/model");
    defineArray.push("lib/wType/vocabulary");
    defineArray.push("lib/wType/string");
    
    define(moduleName, defineArray, function pageStorage_module() {
        var Model = require("models/model");
        var Vocabulary = require("lib/wType/vocabulary");
        var String = require("lib/wType/string");
        
        var PageStorage = Model.inherit({
            "className": "PageStorage",
            "constructor": function(addr) {
                Model.fn.constructor.call(this, addr);
                
                this.addHandler("pageAddress");
            },
            "getPageAddress": function(url) {
                var vc = new Vocabulary();
                
                vc.insert("url", new String(url));
                this.send(vc, "getPageAddress");
            },
            "_h_pageAddress": function(ev) {
                var data = ev.getData();
                
                this.trigger("pageAddress", data.at("address").clone());
            }
        });
        
        return PageStorage;
    });
})();
