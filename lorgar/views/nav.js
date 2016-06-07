"use strict";
(function nav_js() {
    var moduleName = "views/nav";
    
    var defineArray = [];
    defineArray.push("views/layout");
    defineArray.push("views/string");
    
    define(moduleName, defineArray, function nav_module() {
        var Layout = require("views/layout");
        var String = require("views/string");
        
        var Nav = Layout.inherit({
            "className": "Nav",
            "constructor": function(options) {
                var base = {};
                W.extend(base, options);
                Layout.fn.constructor.call(this, base);
                
                this._initProxy();
                
                this._label = new String();
                this.append(this._label);
                this.addClass("primary");
                this.addClass("hoverable");
                
                this._initHandlers();
            },
            "destructor": function() {
                this._e.removeEventListener("click", this._proxy.onClick, false);
                
                Layout.fn.destructor.call(this);
            },
            "data": function(data) {
                this._label.data(data);
            },
            "_initHandlers": function() {
                this._e.addEventListener("click", this._proxy.onClick, false);
            },
            "_initProxy": function() {
                this._proxy = {
                    onClick: this._onClick.bind(this)
                }
            },
            "_onClick": function(e) {
                this.trigger("activate");
            }
        });
        
        return Nav;
    });
})();