"use strict";
(function gridLayout_js() {
    var moduleName = "views/gridLayout";
    
    var defineArray = [];
    defineArray.push("views/layout");
    
    define(moduleName, defineArray, function gridLayout_module() {
        var Layout = require("views/layout");
        
        var GridLayout = Layout.inherit({
            "className": "GridLayout",
            "constructor": function(options) {
                var base = {
                    
                };
                W.extend(base, options);
                Layout.fn.constructor.call(this, base);
                
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
                
                this._recountLimits();
                if (this._w !== undefined && this._h !== undefined) {
                    this.refreshLay();
                }
            },
            "_cleanupLay": function() {
                
            },
            "_recountLimits": function() {
                this._cols = [];
                this._rows = [];
                
                for (var i = 0; i < this._lay.length; ++i) {
                    for (var j = 0; j < this._lay[i].length) {
                        var e = this._lay[i][j];
                        while (!this._cols[j]) {
                            this._cols.push({});
                        }
                        while (!this._rows[i]) {
                            this._rows.push({});
                        }
                        if (e) {
                            var colMinW = this._cols[j].min || 0;
                            var rowMinH = this._rows[i].min || 0;
                            var colMaxW = this._cols[j].max === undefined ? Infinity : this._cols[j].max;
                            var rowMaxH = this._rows[i].max === undefined ? Infinity : this._rows[i].max;
                            
                            this._cols[j].min = Math.max(colMinW, e.options.minWidth);
                            this._rows[i].min = Math.max(rowMinH, e.options.minHeight);
                            this._cols[j].max = Math.min(colMaxW, e.options.maxWidth);
                            this._rows[i].max = Math.min(rowMaxH, e.options.maxHeight);
                        } else {
                            this._cols[j].min = this._cols[j].min || 0;
                            this._rows[i].min = this._rows[i].min || 0;
                            this._cols[j].max = this._cols[j].max || 0;
                            this._rows[i].max = this._rows[i].max || 0;
                        }
                    }
                }
            },
            "refreshLay": function() {
                var totalMaxW = 0;
                var totalMaxH = 0;
                var totalMinW = 0;
                var totalMinH = 0;
                
                for (i = 0; i < this._cols.length; ++i) {
                    totalMaxW += this._cols[i].max;
                    totalMinW += this._cols[i].min
                }
                for (i = 0; i < this._rows.length; ++i) {
                    totalMaxH += this._rows[i].max;
                    totalMinH += this._rows[i].min;
                }
                
                if (this._w <= totalMinW || this._w >= totalMaxW) {
                    var kW;
                    var keyW;
                    if (this._w <= totalMinW) {
                        kW = this._w / totalMinW;
                        keyW = "min";
                    } else {
                        kW = this._w / totalMaxW;
                        keyW = "max";
                    }
                    
                    for (i = 0; i < this._cols.length; ++i) {
                        this._cols[i].cur = this._cols[i][keyW] * kW;
                    }
                } else {
                    var restW = this._w - totalMinW;
                    var minDotationW = Infinity;
                    var candidatesW = [];
                    for (i = 0; i < this._cols.length; ++i) {
                        this._cols[i].cur = this._cols[i].min;
                        
                        if (this._cols[i].cur < this._cols[i].max) {
                            var donationW = this._cols[i].max - this._cols[i].cur;
                            this._cols[i].don = donationW;
                            candidatesW.push(this._cols[i]);
                            Math.min(minDotationW, donationW);
                        }
                        
                    }
                    
                    candidatesW.sort(GridLayout._candidatesSort);
                    
                    while (restW) {
                        var portionW = restW / candidatesW.length;
                        if (portionW <= minDotationW) {
                            for (i = 0; i < candidatesW.length; ++i) {
                                candidatesW[i].cur += portionW;
                                delete candidatesW[i].don
                            }
                            restW = 0;
                        } else {
                            candidatesW[0].cur += minDotationW;
                            delete candidatesW[0].don;
                            candidatesW.shift();
                            restW -=minDotationW;
                            minDotationW = candidatesW[0].don;
                        }
                    }
                }
                
                if (this._h <= totalMinH || this._w >= totalMaxH) {
                    var kH;
                    var keyH;
                    if (this._h <= totalMinH) {
                        kH = this._h / totalMinH;
                        keyH = "min";
                    } else {
                        kH = this._h / totalMaxH;
                        keyH = "max";
                    }
                    
                    for (i = 0; i < this._rows.length; ++i) {
                        this._rows[i].cur = this._rows[i][keyH] * kH;
                    }
                } else {
                    var restH = this._h - totalMinH;
                    var minDotationH = Infinity;
                    var candidatesH = [];
                    for (i = 0; i < this._rows.length; ++i) {
                        this._rows[i].cur = this._rows[i].min;
                        
                        if (this._rows[i].cur < this._rows[i].max) {
                            var donationH = this._rows[i].max - this._rows[i].cur;
                            this._rows[i].don = donationH;
                            candidatesH.push(this._rows[i]);
                            Math.min(minDotationH, donationH);
                        }
                        
                    }
                    candidatesH.sort(GridLayout._candidatesSort);
                    while (restH) {
                        var portionH = restH / candidatesH.length;
                        if (portionH <= minDotationH) {
                            for (i = 0; i < candidatesH.length; ++i) {
                                candidatesH[i].cur += portionH;
                                delete candidatesH[i].don
                            }
                            restH = 0;
                        } else {
                            candidatesH[0].cur += minDotationH;
                            delete candidatesH[0].don;
                            candidatesH.shift();
                            restH -=minDotationH;
                            minDotationH = candidatesH[0].don;
                        }
                    }
                }
                this._positionElements();
            },
            "_positionElements": function() {
                var shiftW = 0;
                var shiftH = 0;
                
                for (var i = 0; i < this._lay.length; ++i) {
                    shiftH = 0;
                    for (var j = 0; j < this._lay[i].length) {
                        var e = this._lay[i][j];
                        if (e) {
                            e.setSize(this._cols[j].cur, this._rows[i].cur);
                            e._e.style.top = shiftH + "px"
                            e._e.style.left = shiftW + "px"
                        }
                        shiftW += this._cols[j].cur;
                    }
                    shiftH += this._rows[i].cur;
                }
            },
            "removeChild": function(child) {
                Layout.fn.removeChild.call(this, child);
                
                for (var i = 0; i < this._lay.length; ++i) {
                    for (var j = 0; j < this._lay[i].length) {
                        if (child === this._lay[i][j]) {
                            this._lay[i][j] = false;
                        }
                    }
                }
                
                this._cleanupLay();
                this._recountLimits();
                this.refreshLay();
            },
            "setSize": function(w, h) {
                this._w = this.constrainWidth(w);
                this._h = this.constrainHeight(h);
                
                this._e.style.width = this._w + "px";
                this._e.style.height = this._h + "px";
                
                this.refreshLay();
            }
        });
        
        GridLayout._candidatesSort = function(a, b) {
            return a.don - b.don;
        }
        
        return GridLayout;
    });
})();