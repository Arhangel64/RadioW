"use strict";
(function abstractmap_js() {
    var moduleName = "lib/wContainers/abstractmap";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    defineArray.push("lib/js_bintrees/rbtree");
    defineArray.push("lib/wContainers/abstractpair");
    
    define(moduleName, defineArray,function abstractmap_module() {
        var Class = require("lib/utils/class");
        var RBTree = require("lib/js_bintrees/rbtree");
        var AbstractPair = require("lib/wContainers/abstractpair");
        
        var AbstractMap = Class.inherit({
            "className": "AbstractMap",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                var check = new this.constructor.dataType(new this.constructor.dataType.firstType(), new this.constructor.dataType.secondType());
                
                if (!(check instanceof AbstractPair)) {
                    throw new Error("An attempt to instantiate a map without declared member type");
                }
                
                this._data = new RBTree(function (a, b) {
                    if (a[">"](b)) {
                        return 1;
                    }
                    if (a["<"](b)) {
                        return -1
                    }
                    if (a["=="](b)) {
                        return 0;
                    }
                });
                
                this._end = this._data.iterator();
            },
            "destructor": function() {
                this.clear();
                
                Class.fn.destructor.call(this);
            },
            "begin": function() {
                var itr = this._data.iterator();
                if (itr.next() !== null) {
                    return itr;
                }
                return this._end;
            },
            "clear": function() {
                this._data.each(function(data) {
                    data.destructor();
                });
                
                this._data.clear();
            },
            "each": function(funct) {
                this._data.each(funct);
            },
            "end": function() {
                return this._end;
            },
            "erase": function(itr) {
                if (!this._data.remove(itr.data())) {
                    throw new Error("An attempt to remove non-existing map element");
                }
            },
            "find": function(key) {
                var pair = new this.constructor.dataType(key, new this.constructor.dataType.secondType());
                
                var iter = this._data.findIter(pair);
                if (iter === null) {
                    return this._end;
                }
                return iter;
            },
            "insert": function(key, value) {
                var pair = new this.constructor.dataType(key, value);
                
                return this._data.insert(pair);
            },
            "r_each": function(funct) {
                this._data.reach(funct);
            },
            "size": function() {
                return this._data.size;
            }
        });
        
        AbstractMap.dataType = undefined;
        
        AbstractMap.template = function(first, second) {
            var dt = AbstractPair.template(first, second);
            
            var Map = AbstractMap.inherit({
                "className": "Map",
                "constructor": function() {
                    AbstractMap.fn.constructor.call(this);
                }
            });
            
            Map.dataType = dt;
            
            return Map;
        };
        
        return AbstractMap;
    });
})();