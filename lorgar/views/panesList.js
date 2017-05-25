"use strict";
(function panesList_js() {
    var moduleName = "views/panesList";
    
    var defineArray = [];
    defineArray.push("views/view");
    defineArray.push("views/layout");
    defineArray.push("views/label");
    defineArray.push("lib/wController/localModel");
    
    define(moduleName, defineArray, function panesList_module() {
        var View = require("views/view");
        var Layout = require("views/layout");
        var Label = require("views/label");
        var LM = require("lib/wController/localModel");
        
        var PanesList = Layout.inherit({
            "className": "PanesList",
            "constructor": function PanesListView(controller, options) {
                var base = {
                    nestWidth: 100,
                    nestHeight: 100,
                    verticalSpace: 10,
                    scrollable: Layout.Scroll.Vertical
                };
                W.extend(base, options);
                Layout.fn.constructor.call(this, controller, base);
                
                this._overflown = false;
                this._rows = 0;
                this._cachedMinH = 0;
                this._cols = 0;
                
                this._f.on("newElement", this._onNewElement, this);
                var size = this._f.data.size();
                for (var i = 0; i < size; ++i) {
                    this._onNewElement(this._f.data.at(i));
                }
            },
            "destructor": function() {
                Layout.fn.destructor.call(this);
            },
            "append": function(child, aligment) {
                var model = new LM();
                var nest = new Layout(model, {
                    minHeight: this._o.nestHeight,
                    maxHeight: this._o.nestHeight,
                    minWidth: this._o.nestWidth,
                    minWidth: this._o.nestWidth,
                    scrollable: Layout.Scroll.Both
                });
                nest.append(child, aligment);
                this._addChild(nest, 0);
                
                nest.setSize(this._o.nestWidth, this._o.nestHeight);
                
                if (this._cols) {
                    this._positionElement(this._c.length - 1);
                    this._recalculateRows();
                }
            },
            "_onNewController": function(ctrl) {
                var label = new Label(ctrl);
                this.append(label);
            },
            "_onNewElement": function(id) {
                var model = new LM();
                model.data = id.toString();
                var label = new Label(model);
                this.append(label);
                
                this._uncyclic.push(function() {
                    model.destructor();
                });
            },
            "_positionElement": function(index) {
                var row = Math.floor(index / this._cols);
                var col = index % this._cols;
                var e = this._c[index];
                
                e.c.setLeft(col * this._o.nestWidth + col * this._hSpace);
                e.c.setTop(row * this._o.nestHeight + row * this._o.verticalSpace);
            },
            "_recalculateRows": function() {
                var rows = Math.ceil(this._c.length / this._cols);
                if (rows !== this._rows) {
                    this._rows = rows;
                    this._cachedMinH = (rows * this._o.nestWidth) + (rows - 1) * this._o.verticalSpace;
                }
                this._scr.setMinSize(this._w, Math.max(this._cachedMinH, this._h));
            },
            "setSize": function(w, h) {
                View.fn.setSize.call(this, w, h);
                
                this._cols = Math.floor(this._w / this._o.nestWidth);
                this._hSpace = (this._w - (this._cols * this._o.nestWidth)) / (this._cols - 1);
                
                if (this._o.scrollable) {
                    this._recalculateRows();
                }
                
                for (var i = 0; i < this._c.length; ++i) {
                    this._positionElement(i);
                }
            }
        });
        
        return PanesList;
    });
})();
