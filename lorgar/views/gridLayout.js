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
                var notification = this._recountLimits();
                if (!notification) {
                    this.refreshLay();
                }
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
                                var s;
                                for (s = 0; s < e.colspan; ++s) {
                                    tWidth += this._cols[j + s].cur;
                                }
                                for (s = 0; s < e.rowspan; ++s) {
                                    tHeight += this._rows[i + s].cur;
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
            "_recountLimits": function() {
                this._cols = [];
                this._rows = [];
                var i, j;
                var multiCols = [];
                var multiRows = [];
                var proccessed = Object.create(null);
                
                for (i = 0; i < this._lay.length; ++i) {
                    while (!this._rows[i]) {
                        this._rows.push({});
                    }
                    for (j = 0; j < this._lay[i].length; ++j) {
                        while (!this._cols[j]) {
                            this._cols.push({});
                        }
                        for (var k = 0; k < this._lay[i][j].length; ++k) {
                            var e = this._lay[i][j][k];
                            
                            if (proccessed[e.child._id]) {
                                this._cols[j].min = this._cols[j].min || 0;
                                this._rows[i].min = this._rows[i].min || 0;
                                this._cols[j].max = this._cols[j].max || 0;
                                this._rows[i].max = this._rows[i].max || 0;
                                continue;
                            }
                            
                            var colMinW = this._cols[j].min || 0;
                            var rowMinH = this._rows[i].min || 0;
                            var colMaxW = this._cols[j].max || Infinity;
                            var rowMaxH = this._rows[i].max || Infinity;
                            
                            if (e.colspan === 1) {
                                this._cols[j].min = Math.max(colMinW, e.child._o.minWidth);
                                this._cols[j].max = Math.min(colMaxW, e.child._o.maxWidth);
                            } else {
                                this._cols[j].min = colMinW;
                                this._cols[j].max = colMaxW;
                                
                                multiCols.push({
                                    p: j,
                                    e: e
                                });
                            }
                            if (e.rowspan === 1) {
                                this._rows[i].min = Math.max(rowMinH, e.child._o.minHeight);
                                this._rows[i].max = Math.min(rowMaxH, e.child._o.maxHeight);
                            } else {
                                this._rows[i].min = rowMinH;
                                this._rows[i].max = rowMaxH;
                                
                                multiRows.push({
                                    p: i,
                                    e: e
                                });
                            }
                            
                            proccessed[e.child._id] = true;
                        }
                        if (!this._lay[i][j].length) {
                            this._cols[j].min = this._cols[j].min || 0;
                            this._rows[i].min = this._rows[i].min || 0;
                            this._cols[j].max = this._cols[j].max || 0;
                            this._rows[i].max = this._rows[i].max || 0;
                        }
                    }
                }
                
                for (i = 0; i < multiCols.length; ++i) {
                    var e = multiCols[i].e;
                    var pos = multiCols[i].p;
                    var span = e.colspan;
                    var target = pos + span;
                    var minSize = 0;
                    var maxSize = 0;
                    for (j = pos; j < target; ++j) {
                        minSize += this._cols[j].min;
                        maxSize += this._cols[j].max;
                    }
                    if (e.child._o.minWidth > minSize) {
                        var portion = (e.child._o.minWidth - minSize) / span;
                        for (j = pos; j < target; ++j) {
                            this._cols[j].min += portion;
                        }
                    }
                    if (e.child._o.maxWidth < maxSize) {
                        var portion = (maxSize - e.child._o.maxWidth) / span;
                        for (j = pos; j < target; ++j) {
                            this._cols[j].max -= portion;
                        }
                    }
                }
                
                for (i = 0; i < multiRows.length; ++i) {
                    var e = multiRows[i].e;
                    var pos = multiRows[i].p;
                    var span = e.rowspan;
                    var target = pos + span;
                    var minSize = 0;
                    var maxSize = 0;
                    for (j = pos; j < target; ++j) {
                        minSize += this._rows[j].min;
                        maxSize += this._rows[j].max;
                    }
                    if (e.child._o.minHeight > minSize) {
                        var portion = (e.child._o.minHeight - minSize) / span;
                        for (j = pos; j < target; ++j) {
                            this._rows[j].min += portion;
                        }
                    }
                    if (e.child._o.maxHeight < maxSize) {
                        var portion = (maxSize - e.child._o.maxHeight) / span;
                        for (j = pos; j < target; ++j) {
                            this._rows[j].max -= portion;
                        }
                    }
                }
                
                var minWidth = 0;
                var minHeight = 0;
                var maxWidth = 0;
                var maxHeight = 0;
                
                for (i = 0; i < this._rows.length; ++i) {
                    minHeight += this._rows[i].min;
                    this._rows[i].max = Math.max(this._rows[i].max, this._rows[i].min);
                    maxHeight += this._rows[i].max;
                }
                for (i = 0; i < this._cols.length; ++i) {
                    minWidth += this._cols[i].min;
                    this._cols[i].max = Math.max(this._cols[i].max, this._cols[i].min);
                    maxWidth += this._cols[i].max;
                }
                
                return this._setLimits(minWidth, minHeight, maxWidth, maxHeight);
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
                    distribute(this._w, this._cols);
                }
                
                if (this._h <= totalMinH || this._h >= totalMaxH) {
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
                    distribute(this._h, this._rows);
                }
                this._positionElements();
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
        
        function distribute (size, array) {
            var i, portion;
            var cMax = [];
            for (i = 0; i < array.length; ++i) {
                array[i].cur = array[i].min;
                size -= array[i].min;
                
                if (array[i].cur < array[i].max) {
                    cMax.push(array[i]);
                }
            }
            cMax.sort(GridLayout._candidatesSortMax);
            
            while (cMax.length && size) {
                portion = size / cMax.length;
                var last = cMax[cMax.length -1];
                
                if (portion >= last.max) {
                    size -= last.max - last.cur;
                    last.cur = last.max;
                    cMax.pop();
                } else {
                    for (i = 0; i < cMax.length; ++i) {
                        cMax[i].cur += portion;
                    }
                    size = 0;
                }
            }
            
            if (size) {
                portion = size / array.length;
                for (i = 0; i < array.length; ++i) {
                    array.cur += portion;
                }
            }
        }
        
        GridLayout._candidatesSortMax = function(a, b) {
            return b.max - a.max;
        }
        
        return GridLayout;
    });
})();
