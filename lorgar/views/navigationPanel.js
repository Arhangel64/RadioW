"use strict";
(function navigationPanel_js() {
    var moduleName = "views/navigationPanel";
    
    var defineArray = [];
    defineArray.push("views/gridLayout");
    defineArray.push("views/view");
    
    define(moduleName, defineArray, function navigationPanel_module() {
        var GridLayout = require("views/gridLayout");
        var View = require("views/view");
        
        var NavigationPanel = GridLayout.inherit({
            "className": "NavigationPanel",
            "constructor": function(options) {
                var base = {
                    minHeight: 50,
                    maxHeight: 50
                };
                W.extend(base, options)
                GridLayout.fn.constructor.call(this, base);
                
                this.addClass("primary");
                this._spacer = new View();
            },
            "destructor": function() {
                this._spacer.destructor();
                
                GridLayout.fn.destructor.call(this);
            },
            "clear": function() {
                this._spacer.remove();
                GridLayout.fn.clear.call(this);
            },
            "push": function(element) {
                this._spacer.remove();
                this.append(element, 0, this._c.length, 1, 1);
                this.append(this._spacer, 0, this._c.length, 1, 1);
            }
        });
        
        return NavigationPanel;
    });
})();