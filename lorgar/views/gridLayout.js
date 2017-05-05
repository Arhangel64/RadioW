"use strict";
(function gridLayout_js() {
    var moduleName = "views/gridLayout";
    
    var defineArray = [];
    defineArray.push("views/view");
    defineArray.push("views/layout");
    
    define(moduleName, defineArray, function gridLayout_module() {
        var View = require("views/view");
        var Layout = require("views/layout");
        
        var GridLayout = Layout.inherit({
            "className": "GridLayout",
            "constructor": function(controller, options) {
                var base = {
                    
                };
                W.extend(base, options);
                Layout.fn.constructor.call(this, controller, base);
                
                this._lay = [[[]]];
                this._cols = [{}];
                this._rows = [{}];
            },
            "append": function(child, row, col, rowSpan, colSpan, aligment) {
                aligment = aligment || 5;
                this._addChild(child, aligment);
                
                rowSpan = rowSpan || 1;
                colSpan = colSpan || 1;
                
                var tRow = row  + rowSpan;
                var tCol = col  + colSpan;
                
                while (this._lay.length < tRow) {
                    this._lay.push([]);
                    
                }
                for (var i = 0; i < this._lay.length; ++i) {
                    while (this._lay[i].length < tCol) {
                        this._lay[i].push([]);
                    }
                }
                var obj = {
                    child: child,
                    colspan: colSpan,
                    rowspan: rowSpan,
                    a: aligment
                }
                
                for (i = row; i < tRow; ++i) {
                    for (var j = col; j < tCol; ++j) {
                        this._lay[i][j].push(obj);
                    }
                }
                
                this._recountLimits();
                if (this._w !== undefined && this._h !== undefined) {
                    this.refreshLay();
                }
            },
            "_cleanupLay": function() {
                var i;
                var rowsC = false;
                var colsC = false;
                while (!rowsC) {
                    for (i = 0; i < this._lay[this._lay.length - 1].length; ++i) {
                        if (this._lay[this._lay.length - 1][i].length) {
                            rowsC = true;
                            break;
                        }
                    }
                    if (!rowsC) {
                        this._lay.pop()
                        rowsC = !this._lay.length;
                        colsC = !this._lay.length;
                    }
                }
                while (!colsC) {
                    for (i = 0; i < this._lay.length; ++i) {
                        if (this._lay[i][this._lay[i].length - 1].length) {
                            colsC = true;
                            break;
                        }
                    }
                    if (!colsC) {
                        for (i = 0; i < this._lay.length; ++i) {
                            this._lay[i].pop();
                        }
                    }
                }
            },
            "clear": function() {
                Layout.fn.clear.call(this);
                
                this._lay = [[[]]];
                this._cols = [{}];
                this._rows = [{}];
            },
            "_onChildChangeLimits": function() {
                this._recountLimits();
                this.refreshLay();
            },
            "_recountLimits": function() {
                this._cols = [];
                this._rows = [];
                
                for (var i = 0; i < this._lay.length; ++i) {
                    while (!this._rows[i]) {
                        this._rows.push({});
                    }
                    for (var j = 0; j < this._lay[i].length; ++j) {
                        while (!this._cols[j]) {
                            this._cols.push({});
                        }
                        for (var k = 0; k < this._lay[i][j].length; ++k) {
                            var e = this._lay[i][j][k];
                            
                            var colMinW = this._cols[j].min || 0;
                            var rowMinH = this._rows[i].min || 0;
                            var colMaxW = this._cols[j].max || Infinity;
                            var rowMaxH = this._rows[i].max || Infinity;
                            
                            this._cols[j].min = Math.max(colMinW, e.child._o.minWidth / e.colspan);
                            this._rows[i].min = Math.max(rowMinH, e.child._o.minHeight / e.rowspan);
                            this._cols[j].max = Math.min(colMaxW, e.child._o.maxWidth * e.colspan);
                            this._rows[i].max = Math.min(rowMaxH, e.child._o.maxHeight * e.rowspan);
                        }
                        if (!this._lay[i][j].length) {
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
                var i;
                
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
                    var restW = this._w;
                    var cWMax = this._cols.slice();
                    var cWMin = this._cols.slice();
                    
                    cWMax.sort(GridLayout._candidatesSortMax);
                    cWMin.sort(GridLayout._candidatesSortMin);
                    
                    while (restW) {
                        var portionW = restW / cWMin.length;
                        
                        if (portionW < cWMin[0].min) {
                            cWMin[0].cur = cWMin[0].min;
                            restW -= cWMin[0].min;
                            cWMax.splice(cWMax.indexOf(cWMin[0]), 1);
                            cWMin.shift();
                        } else if (portionW > cWMax[0].max) {
                            cWMax[0].cur = cWMax[0].max;
                            restW -= cWMax[0].max;
                            cWMin.splice(cWMin.indexOf(cWMax[0]), 1);
                            cWMax.shift();
                        } else {
                            for (i = 0; i < cWMin.length; ++i) {
                                cWMin[i].cur = portionW;
                            }
                            restW = 0;
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
                    var restH = this._h;
                    var cHMax = this._rows.slice();
                    var cHMin = this._rows.slice();
                    
                    cHMax.sort(GridLayout._candidatesSortMax);
                    cHMin.sort(GridLayout._candidatesSortMin);
                    
                    while (restH) {
                        var portionH = restH / cHMin.length;
                        
                        if (portionH < cHMin[0].min) {
                            cHMin[0].cur = cHMin[0].min;
                            restH -= cHMin[0].min;
                            cHMax.splice(cHMax.indexOf(cHMin[0]), 1);
                            cHMin.shift();
                        } else if (portionH > cHMax[0].max) {
                            cHMax[0].cur = cHMax[0].max;
                            restH -= cHMax[0].max;
                            cHMin.splice(cHMin.indexOf(cHMax[0]), 1);
                            cHMax.shift();
                        } else {
                            for (i = 0; i < cHMin.length; ++i) {
                                cHMin[i].cur = portionH;
                            }
                            restH = 0;
                        }
                    }
                }
                this._positionElements();
            },
            "_positionElements": function() {
                var shiftW = 0;
                var shiftH = 0;
                
                var positioned = [];
                
                for (var i = 0; i < this._lay.length; ++i) {
                    shiftW = 0;
                    for (var j = 0; j < this._lay[i].length; ++j) {
                        for (var k = 0; k < this._lay[i][j].length; ++k) {
                            var e = this._lay[i][j][k];
                            var child = e.child;
                            if (positioned.indexOf(child) === -1) {
                                var tWidth = 0;
                                var tHeight = 0;
                                var k;
                                for (k = 0; k < e.colspan; ++k) {
                                    tWidth += this._cols[j + k].cur;
                                }
                                for (k = 0; k < e.rowspan; ++k) {
                                    tHeight += this._rows[i + k].cur;
                                }
                                child.setSize(tWidth, tHeight);
                                
                                switch (e.a) {
                                    case Layout.Aligment.LeftTop:
                                        child.setTop(shiftH);
                                        child.setLeft(shiftW);
                                        break;
                                    case Layout.Aligment.LeftCenter:
                                        child.setTop(shiftH + (tHeight - child._h) / 2);
                                        child.setLeft(shiftW);
                                        break;
                                    case Layout.Aligment.LeftBottom:
                                        child.setTop(shiftH + (tHeight - child._h));
                                        child.setLeft(shiftW);
                                        break;
                                    case Layout.Aligment.CenterTop:
                                        child.setTop(shiftH);
                                        child.setLeft(shiftW + (tWidth - child._w) / 2);
                                        break;
                                    case Layout.Aligment.CenterCenter:
                                        child.setTop(shiftH + (tHeight - child._h) / 2);
                                        child.setLeft(shiftW + (tWidth - child._w) / 2);
                                        break;
                                    case Layout.Aligment.CenterBottom:
                                        child.setTop(shiftH + (tHeight - child._h));
                                        child.setLeft((tWidth - child._w) / 2);
                                        break;
                                    case Layout.Aligment.RightTop:
                                        child.setTop(shiftH);
                                        child.setLeft(shiftW + (tWidth - child._h));
                                        break;
                                    case Layout.Aligment.RightCenter:
                                        child.setTop((tHeight - child._h) / 2);
                                        child.setLeft(shiftW + (tWidth - child._h));
                                        break;
                                    case Layout.Aligment.RightBottom:
                                        child.setTop(shiftH + (tHeight - child._h));
                                        child.setLeft(shiftW + (tWidth - child._h));
                                        break;
                                    
                                }
                                positioned.push(child);
                            }
                        }
                        shiftW += this._cols[j].cur;
                    }
                    shiftH += this._rows[i].cur;
                }
            },
            "removeChild": function(child) {
                Layout.fn.removeChild.call(this, child);
                
                for (var i = 0; i < this._lay.length; ++i) {
                    for (var j = 0; j < this._lay[i].length; ++j) {
                        for (var k = 0; k < this._lay[i][j].length; ++k) {
                            if (child === this._lay[i][j][k].child) {
                                this._lay[i][j].splice(k, 1);
                            }
                        }
                    }
                }
                
                this._cleanupLay();
                this._recountLimits();
                this.refreshLay();
            },
            "setSize": function(w, h) {
                View.fn.setSize.call(this, w, h);
                
                if (this._c.length) {
                    this.refreshLay();
                }
            }
        });
        
        GridLayout._candidatesSortMin = function(a, b) {
            return b.min - a.min;
        }
        GridLayout._candidatesSortMax = function(a, b) {
            return a.max - b.max;
        }
        
        return GridLayout;
    });
})();
