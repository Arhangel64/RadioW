"use strict";
(function gridLayout_js() {
    var moduleName = "views/gridLayout";
    
    var defineArray = [];
    defineArray.push("views/layout");
    
    define(moduleName, defineArray, function gridLayout_module() {
        var Layout = require("views/layout");
        
        var GridLayout = Layout.inherit({
            "className": "GridLayout",
            "constructor": function(options, element) {
                var base = {
                    
                };
                W.extend(base, options);
                Layout.fn.constructor.call(this, base, element);
                
                this._lay = [[]];
                this._cols = [];
                this._rows = [];
            },
            "append": function(child, row, col) {
                this._c.push(child);
                this._e.appendChild(child._e);
                child._p = this;
                
                while (this._lay.length - 1 !== row) {
                    this._lay.push([]);
                }
                while (this._lay[row].length - 1 !== col) {
                    this._lay[row].push(false);
                }
                this._lay[row][col] = child;
                if (this._w !== undefined && this._h !== undefined) {
                    this.refreshLay();
                }
            },
            "refreshLay": function() {
                this._cols = [];
                this._rows = [];
                
                for (var i = 0; i < this._lay.length; ++i) {
                    for (var j = 0; j < this._lay[i].length) {
                        var e = this._lay[i][j];
                        if (e) {
                            var colW = this._cols[j] || 0;
                            var rowH = this._rows[i] || 0;
                            this._cols[j] = Math.max(colW, e.options.minWidth);
                            this._rows[i] = Math.max(rowH, e.options.minHeight);
                        } else {
                            this._cols[j] = this._cols[j] || 0;
                            this._rows[i] = this._rows[i] || 0;
                        }
                    }
                }
                
                var totalW = 0;
                var totalH = 0;
                
                for (i = 0; i < this._cols.length; ++i) {
                    totalW += this._cols[i];
                }
                for (i = 0; i < this._rows.length; ++i) {
                    totalH += this._rows[i];
                }
                
                var kW = this._w / totalW;
                var kH = this._h / totalH;
                var shiftW = 0;
                var shiftH = 0;
                
                for (i = 0; i < this._cols.length; ++i) {
                    this._cols[i] = this._cols[i] * kW;
                    shiftW += this._cols[i];
                    for (j = 0; j < this._lay.length; ++j) {
                        e = this._lay[j][i];
                        if (e) {
                            e._e.style.left = shiftW + "px";
                        }
                    }
                }
                for (i = 0; i < this._rows.length; ++i) {
                    this._rows[i] = this._rows[i] * kH;
                    shiftH += this._rows[i];
                    for (j = 0; j < this._lay[i].length; ++j) {
                        e = this._lay[i][j];
                        if (e) {
                            e._e.style.top = shiftH + "px";
                        }
                    }
                }
            },
            "setSize": function(w, h) {
                this._w = this.constrainWidth(w);
                this._h = this.constrainHeight(h);
                
                this._e.style.width = this._w + "px";
                this._e.style.height = this._h + "px";
                
                this.refreshLay();
            }
        });
        
        return GridLayout;
    });
})();