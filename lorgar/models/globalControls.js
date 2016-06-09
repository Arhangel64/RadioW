"use strict";
(function globalControls_js() {
    var moduleName = "models/globalControls";
    
    var defineArray = [];
    defineArray.push("models/list");
    defineArray.push("models/string");
    defineArray.push("models/navigationPanel");
    
    defineArray.push("views/string");
    defineArray.push("views/navigationPanel");
    
    define(moduleName, defineArray, function globalControls_module() {
        var List = require("models/list");
        var ModelString = require("models/string");
        var NavigationPanel = require("models/navigationPanel");
        
        var ViewString = require("views/string");
        var ViewNavigationPanel = require("views/navigationPanel");
        
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
                        var vm = new ModelString(addr.clone());
                        this.addModel(vm);
                        var vv = new ViewString({maxHeight: 20});
                        this._layout.append(vv, 2, 0);
                        vm.addView(vv);
                        break;
                    case "navigationPanel":
                        var npm = new NavigationPanel(addr.clone());
                        this.addModel(npm);
                        var vnp = new ViewNavigationPanel();
                        this._layout.append(vnp, 0, 0);
                        npm.addView(vnp);
                        break;
                    default:
                        console.warn("Unsupported global control: " + name + " (" + type + ")");
                }
            }
        });
        
        return GlobalControls;
    });
})();