"use strict";
(function panesList_js() {
    var moduleName = "views/panesList";
    
    var defineArray = [];
    defineArray.push("views/view");
    defineArray.push("views/layout");
    defineArray.push("views/label");
    defineArray.push("lib/wController/localModel");
    defineArray.push("views/pane");
    
    define(moduleName, defineArray, function panesList_module() {
        var View = require("views/view");
        var Layout = require("views/layout");
        var Label = require("views/label");
        var LM = require("lib/wController/localModel");
        var Pane = require("views/pane");
        
        var PanesList = Layout.inherit({
            "className": "PanesList",
            "constructor": function PanesListView(controller, options) {
                var base = {
                    nestWidth: 100,
                    nestHeight: 100,
                    verticalSpace: 10,
                    scrollable: Layout.Scroll.VirtualVertical
                };
                W.extend(base, options);
                this._ctrlInitialized = false;
                Layout.fn.constructor.call(this, controller, base);
                
                this._scr.on("scrollTop", this._onScrollTop, this);
                this._scr.on("dragStart", this._onScrollDragStart, this);
                this._scr.on("dragEnd", this._onScrollDragEnd, this);
                
                this._hbi = Object.create(null);
                this._overflown = false;
                this._rows = 0;
                this._cachedMinH = 0;
                this._cols = 0;
                this._scrolled = 0;
                this._scrollShift = 0;
                this._rangeUpdate = false;
                this._skipPaneActivate = false;
                this._proxyClearSkippingPaneActivate = this._clearSkippingPaneActivate.bind(this);
                
                this._f.on("removedController", this._onRemovedController, this);
                this._f.on("rangeStart", this._onRangeStart, this);
                this._f.on("rangeEnd", this._onRangeEnd, this);
                this._f.setSubscriptionRange(0, 0);
            },
            "append": function(child, index) {
                var model = new LM();
                var nest = new Layout(model, {
                    minHeight: this._o.nestHeight,
                    maxHeight: this._o.nestHeight,
                    minWidth: this._o.nestWidth,
                    minWidth: this._o.nestWidth,
                    scrollable: Layout.Scroll.None
                });
                nest._uncyclic.push(function() {model.destructor()});
                nest.append(child);
                child.on("activate", this._onChildActivate, this); //todo need to remove handler on deletion
                this._addChild(nest, 0, index);
                
                nest.setSize(this._o.nestWidth, this._o.nestHeight);
                
                if (this._cols && !this._rangeUpdate) {
                    this._positionElement(index);
                    if (index !== this._c.length - 1) {
                        this._refreshPositions(index + 1);
                    }
                }
            },
            "_clearSkippingPaneActivate": function() {
                this._skipPaneActivate = false;
            },
            "_onAddElement": function() {
                this._recalculateRows();
            },
            "_onChildActivate": function(address) {
                if (!this._skipPaneActivate) {
                    lorgar.changePage(address);
                }
            },
            "_onData": function() {
                if (this._f.initialized) {
                    if (!this._ctrlInitialized) {
                        this._f.on("addElement", this._onAddElement, this);
                        this._ctrlInitialized = true;
                    }
                    this._recalculateRows();
                }
            },
            "_onNewController": function(ctrl, index) {
                var label = new Pane(ctrl);
                this.append(label, index);
            },
            "_onRangeEnd": function() {
                this._rangeUpdate = false;
                this._refreshPositions(0);
            },
            "_onRangeStart": function() {
                this._rangeUpdate = true;
            },
            "_onRemovedController": function(ctrl, index) {
                var obj = this._c[index];
                this._removeChildByIndex(index);
                obj.c.destructor();
                
                if (!this._rangeUpdate) {
                    this._refreshPositions(index);
                }
            },
            "_onScrollDragStart": function() {
                this._skipPaneActivate = true;
            },
            "_onScrollDragEnd": function() {
                setTimeout(this._proxyClearSkippingPaneActivate, 1);
            },
            "_onScrollTop": function(y) {
                this._scrolled = y;
                this._recalculateShown();
            },
            "_positionElement": function(index) {
                var row = Math.floor(index / this._cols);
                var col = index % this._cols;
                var e = this._c[index];
                
                e.c.setLeft(col * this._o.nestWidth + col * this._hSpace);
                e.c.setTop(row * this._o.nestHeight + row * this._o.verticalSpace - this._scrollShift);
            },
            "_recalculateRows": function() {
                var rows = Math.ceil(this._f.data.size() / this._cols);
                if (rows !== this._rows) {
                    this._rows = rows;
                    this._cachedMinH = (rows * this._o.nestHeight) + (rows - 1) * this._o.verticalSpace;
                }
                this._scr.setMinSize(this._w, Math.max(this._cachedMinH, this._h));
            },
            "_recalculateShown": function() {
                var possibleRows = Math.ceil(this._h / (this._o.nestHeight + this._o.verticalSpace));
                var amount = this._cols * (possibleRows);
                
                this._scrollShift = this._scrolled % (this._o.nestHeight + this._o.verticalSpace);
                var start = Math.floor(this._scrolled / (this._o.nestHeight + this._o.verticalSpace)) * this._cols;
                var end = start + amount;
                
                this._f.setSubscriptionRange(start, end);
                this._refreshPositions(0);
            },
            "_refreshPositions": function(start) {
                for (var i = start; i < this._c.length; ++i) {
                    this._positionElement(i);
                }
            },
            "_removeChildByIndex": function(i) {
                var child = this._c[i].c;
                this._c.splice(i, 1);
                child._p = undefined;
                
                if (this._o.scrollable) {
                    this._scr.removeChild(child._e);
                } else {
                    this._e.removeChild(child._e);
                }
                
                child.off("changeLimits", this._onChildChangeLimits, this);
            },
            "setSize": function(w, h) {
                View.fn.setSize.call(this, w, h);
                
                this._cols = Math.floor(this._w / this._o.nestWidth);
                this._hSpace = (this._w - (this._cols * this._o.nestWidth)) / (this._cols - 1);
                
                if (this._o.scrollable) {
                    this._recalculateRows();
                }
                this._recalculateShown();
                this._refreshPositions(0);
            }
        });
        
        return PanesList;
    });
})();
