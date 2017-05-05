"use strict";
(function() {
    var moduleName = "views/page";
    
    var defineArray = [];
    defineArray.push("views/gridLayout");
    
    define(moduleName, defineArray, function() {
        var GridLayout = require("views/gridLayout");
        
        var Page = GridLayout.inherit({
            "className": "Page",
            "constructor": function(f, options) {
                var base = {};
                
                W.extend(base, options);
                GridLayout.fn.constructor.call(this, f, base);
                
                this._f.on("newPageElement", this._onNewPageElement, this);
                this._f.on("clear", this.clear, this);
                
                for (var i = 0; i < this._f.elements.length; ++i) {
                    this._onNewPageElement(this._f.elements[i]);
                }
            },
            "destructor": function() {
                this._f.off("newPageElement", this._onNewPageElement, this);
                this._f.off("clear", this.clear, this);
                
                GridLayout.fn.destructor.call(this);
            },
            "_onNewPageElement": function(element) {
                var view = Page.createByType(element.viewType, element.controller, element.viewOptions);
                
                this.append(view, element.row, element.col, element.rowSpan, element.colSpan, element.aligment);
            }
        });
        
        return Page;
    });
})()
