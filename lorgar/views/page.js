"use strict";
(function() {
    var moduleName = "views/page";
    
    var defineArray = [];
    defineArray.push("views/gridLayout");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wContainer/abstractmap");
    
    define(moduleName, defineArray, function() {
        var GridLayout = require("views/gridLayout");
        var Address = require("lib/wType/address");
        var AbstractMap = require("lib/wContainer/abstractmap");
        
        var ContentMap = AbstractMap.template(Address, Object);
        
        var Page = GridLayout.inherit({
            "className": "Page",
            "constructor": function(f, options) {
                var base = {};
                
                W.extend(base, options);
                GridLayout.fn.constructor.call(this, f, base);
                
                this._map = new ContentMap(false);
                
                this._f.on("addItem", this._onAddItem, this);
                this._f.on("removeItem", this._onRemoveItem, this);
                this._f.on("clear", this.clear, this);
                
                var end = this._f.data.end();
                for (var itr = this._f.data.begin(); !itr["=="](end); itr["++"]()) {
                    var pair = itr["*"]();
                    this._onAddItem(pair.first, pair.second);
                }
            },
            "destructor": function() {
                this._f.off("addItem", this._onAddItem, this);
                this._f.off("removeItem", this._onRemoveItem, this);
                this._f.off("clear", this.clear, this);
                
                this._map.destructor();
                delete this._map;
                
                GridLayout.fn.destructor.call(this);
            },
            "clear": function() {
                GridLayout.fn.clear.call(this);
                
                if (this._map) {
                    this._map.clear();
                }
            },
            "_onAddItem": function(address, element) {
                var view = Page.createByType(element.viewType, element.controller, element.viewOptions);
                
                this._map.insert(address, view);
                
                this.append(view, element.row, element.col, element.rowspan, element.colspan, element.aligment);
            },
            "_onRemoveItem": function(address) {
                var itr = this._map.find(address);
                var pair = itr["*"]();
                
                this.removeChild(pair.second);
                pair.second.destructor();
                
                this._map.erase(itr);
            },
            "_setLimits": function(minWidth, minHeight, maxWidth, maxHeight) {
                var needToTell = false;
                if (this._o.minWidth !== minWidth) {
                    needToTell = true;
                    this._o.minWidth = minWidth;
                }
                if (this._o.minHeight !== minHeight) {
                    needToTell = true;
                    this._o.minHeight = minHeight;
                }
                if (needToTell) {
                    this.trigger("changeLimits", this);
                }
                
                return needToTell && this._events.changeLimits && this._events.changeLimits.length;     //to see if someone actually going to listen that event
            }
        });
        
        return Page;
    });
})()
