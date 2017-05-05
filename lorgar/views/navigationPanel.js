"use strict";
(function navigationPanel_js() {
    var moduleName = "views/navigationPanel";
    
    var defineArray = [];
    defineArray.push("views/gridLayout");
    defineArray.push("views/nav");
    defineArray.push("views/view");
    defineArray.push("lib/wController/localModel");
    
    define(moduleName, defineArray, function navigationPanel_module() {
        var GridLayout = require("views/gridLayout");
        var Nav = require("views/nav");
        var View = require("views/view");
        var LocalModel = require("lib/wController/localModel");
        
        var NavigationPanel = GridLayout.inherit({
            "className": "NavigationPanel",
            "constructor": function(controller, options) {
                var base = {
                    minHeight: 50,
                    maxHeight: 50
                };
                W.extend(base, options)
                GridLayout.fn.constructor.call(this, controller, base);
                
                this._spacerHelper = new LocalModel();
                this._spacer = new View(this._spacerHelper);
            },
            "destructor": function() {
                this._spacer.destructor();
                this._spacerHelper.destructor();
                
                GridLayout.fn.destructor.call(this);
            },
            "clear": function() {
                this._spacer.remove();
                GridLayout.fn.clear.call(this);
            },
            "_onNewController": function(controller) {
                this._spacer.remove();
                var nav = new Nav(controller);
                this.append(nav, 0, this._c.length, 1, 1);
                this.append(this._spacer, 0, this._c.length, 1, 1);
                
                nav.on("activate", this._onNavActivate, this);
            },
            "_onNavActivate": function(address) {
                lorgar.changePage(address);
            }
        });
        
        return NavigationPanel;
    });
})();
