"use strict";
var Class = require("../utils/class");
var RBTree = require("bintrees").RBTree;
var AbstractPair = require("./abstractpair");

var AbstractMap = Class.inherit({
    "className": "AbstractMap",
    "constructor": function(owning) {
        Class.fn.constructor.call(this);
        
        this._owning = owning !== false;
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
    },
    "destructor": function() {
        this.clear();
        
        Class.fn.destructor.call(this);
    },
    "begin": function() {
        var itr = this._data.iterator();
        if (itr.next() !== null) {
            return new Iterator(itr);
        }
        return this.end();
    },
    "clear": function() {
        if (this._owning) {
            this._data.each(function(data) {
                data.destructor();
            });
        }
        
        this._data.clear();
    },
    "each": function(funct) {
        this._data.each(funct);
    },
    "end": function() {
        return new Iterator(this._data.iterator());
    },
    "erase": function(itr) {
        var pair = itr["*"]();
        if (!this._data.remove(pair)) {
            throw new Error("An attempt to remove non-existing map element");
        }
        if (this._owning) {
            pair.destructor();
        }
    },
    "find": function(key) {
        var pair = new this.constructor.dataType(key);
        
        var iter = this._data.findIter(pair);
        if (iter === null) {
            return this.end();
        }
        return new Iterator(iter);
    },
    "insert": function(key, value) {
        var pair = new this.constructor.dataType(key, value);
        
        if (!this._data.insert(pair)) {
            throw new Error("An attempt to insert already existing element into a map");
        }
    },
    "r_each": function(funct) {
        this._data.reach(funct);
    },
    "size": function() {
        return this._data.size;
    }
});

var Iterator = Class.inherit({
    "className": "MapIterator",
    "constructor": function(rbtree_iterator) {
        Class.fn.constructor.call(this);
        
        this._itr = rbtree_iterator;
    },
    "++": function() {
        if ((this._itr._cursor === null)) {
            throw new Error("An attempt to increment an iterator pointing to the end of the list");
        }
        this._itr.next();
    },
    "--": function() {
        this._itr.prev();
        if ((this._itr._cursor === null)) {
            throw new Error("An attempt to decrement an iterator pointing to the beginning of the list");
        }
    },
    "==": function(other) {
        return this._itr.data() === other._itr.data();
    },
    "*": function() {
        if ((this._itr._cursor === null)) {
            throw new Error("An attempt to dereference an iterator pointing to the end of the list");
        }
        return this._itr.data();
    }
});

AbstractMap.dataType = undefined;
AbstractMap.iterator = Iterator;

AbstractMap.template = function(first, second) {
    var dt = AbstractPair.template(first, second);
    
    var Map = AbstractMap.inherit({
        "className": "Map",
        "constructor": function(owning) {
            AbstractMap.fn.constructor.call(this, owning);
        }
    });
    
    Map.dataType = dt;
    
    return Map;
};

module.exports = AbstractMap;