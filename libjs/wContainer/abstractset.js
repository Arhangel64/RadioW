"use strict";
var Class = require("../utils/class");
var RBTree = require("bintrees").RBTree;

var AbstractSet = Class.inherit({
    "className": "AbstractSet",
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
        var value = itr["*"]();
        if (!this._data.remove(value)) {
            throw new Error("An attempt to remove non-existing set element");
        }
        if (this._owning) {
            value.destructor();
        }
    },
    "find": function(key) {
        if (!(key instanceof this.constructor.dataType)) {
            throw new Error("An attempt to find wrong value type in the set");
        }
        var iter = this._data.findIter(key);
        if (iter === null) {
            return this.end();
        }
        return new Iterator(iter);
    },
    "insert": function(value) {
        if (!(value instanceof this.constructor.dataType)) {
            throw new Error("An attempt to insert wrong value type to set");
        }
        if (!this._data.insert(value)) {
            throw new Error("An attempt to insert already existing element into a set");
        }
    },
    "r_each": function(funct) {
        this._data.reach(funct);
    },
    "size": function() {
        return this._data.size;
    },
    "lowerBound": function(key) {
        if (!(key instanceof this.constructor.dataType)) {
            throw new Error("An attempt to find wrong value type in the set");
        }
        return new Iterator(this._data.lowerBound(key));
    },
    "upperBound": function(key) {
        if (!(key instanceof this.constructor.dataType)) {
            throw new Error("An attempt to find wrong value type in the set");
        }
        return new Iterator(this._data.upperBound(key));
    }
});

var Iterator = Class.inherit({
    "className": "SetIterator",
    "constructor": function(rbtree_iterator) {
        Class.fn.constructor.call(this);
        
        this._itr = rbtree_iterator;
    },
    "++": function() {
        if ((this._itr._cursor === null)) {
            throw new Error("An attempt to increment an iterator pointing to the end of the set");
        }
        this._itr.next();
    },
    "--": function() {
        this._itr.prev();
        if ((this._itr._cursor === null)) {
            throw new Error("An attempt to decrement an iterator pointing to the beginning of the set");
        }
    },
    "==": function(other) {
        return this._itr.data() === other._itr.data();
    },
    "*": function() {
        if ((this._itr._cursor === null)) {
            throw new Error("An attempt to dereference an iterator pointing to the end of the set");
        }
        return this._itr.data();
    }
});

AbstractSet.dataType = undefined;
AbstractSet.iterator = Iterator;

AbstractSet.template = function(type) {
    var Set = AbstractSet.inherit({
        "className": "Set",
        "constructor": function(owning) {
            AbstractSet.fn.constructor.call(this, owning);
        }
    });
    
    Set.dataType = type;
    
    return Set;
};

module.exports = AbstractSet;
