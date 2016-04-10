"use strict";
(function abstractlist_js() {
    var moduleName = "lib/wContainer/abstractlist";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    
    define(moduleName, defineArray, function abstractlist_module() {
        var Class = require("lib/utils/class");
        
        var AbstractList = Class.inherit({
            "className": "AbstractList",
            "constructor": function(owning) {
                Class.fn.constructor.call(this);
                
                if (!this.constructor.dataType) {
                    throw new Error("An attempt to instantiate a list without declared member type");
                }
                
                this._owning = owning !== false;
                this._begin = new ListNode(this._owning);
                this._end = this._begin;
                this._size = 0;
                
            },
            "destructor": function() {
                this.clear();
                
                Class.fn.destructor.call(this);
            },
            "begin": function() {
                return new ListIterator(this._begin);
            },
            "clear": function() {
                var node = this._begin;
                while (node !== this._end) {
                    node = node._next;
                    node._prev.destructor();
                }
                node._prev = null;
                
                this._begin = node;
                this._size = 0;
            },
            "end": function() {
                return new ListIterator(this._end);
            },
            "erase": function(begin, end) {
                if (end === undefined) {
                    end = begin.clone();
                    end["++"]();
                }
                
                if (begin._node === this._begin) {
                    this._begin = end._node;
                    end._node._prev = null;
                } else {
                    begin._node._prev._next = end._node;
                    end._node._prev = begin._node._prev;
                }
                
                while(!begin["=="](end)) {
                    
                    var node = begin._node;
                    begin["++"]();
                    --this._size;
                    node.destructor();
                }
            },
            "pop_back": function() {
                if (this._begin === this._end) {
                    throw new Error("An attempt to pop from empty list");
                }
                var node = this._end._prev;
                
                if (node === this._begin) {
                    this._begin = this._end;
                    this._end._prev = null;
                } else {
                    node._prev._next = this._end;
                    this._end._prev = node._prev;
                }
                node.destructor();
                
                --this._size;
            },
            "push_back": function(data) {
                if (!(data instanceof this.constructor.dataType)) {
                    throw new Error("An attempt to insert wrong data type into list");
                }
                
                var node = new ListNode(this._owning);
                node._data = data;
                if (this._size === 0) {
                    this._begin = node;
                } else {
                    this._end._prev._next = node;
                    node._prev = this._end._prev;
                }
                
                node._next = this._end;
                this._end._prev = node;
                
                this._size++;
            },
            "size": function() {
                return this._size;
            }
        });
        
        var ListNode = Class.inherit({
            "className": "ListNode",
            "constructor": function(owning) {
                Class.fn.constructor.call(this);
                
                this._data = null;
                this._next = null;
                this._prev = null;
                this._owning = owning !== false;
            },
            "destructor": function() {
                if (this._owning && (this._data instanceof Class)) {
                    this._data.destructor();
                }
                
                Class.fn.destructor.call(this);
            }
        });
        
        var ListIterator = Class.inherit({
            "className": "ListIterator",
            "constructor": function(node) {
                Class.fn.constructor.call(this);
                
                this._node = node;
            },
            "++": function() {
                if (this._node._next === null) {
                    throw new Error("An attempt to increment iterator, pointing at the end of container");
                }
                this._node = this._node._next;
            },
            "--": function() {
                if (this._node._prev === null) {
                    throw new Error("An attempt to decrement iterator, pointing at the beginning of container");
                }
                this._node = this._node._prev;
            },
            "*": function() {
                if (this._node._data === null) {
                    throw new Error("An attempt to dereference iterator, pointing at the end of container");
                }
                return this._node._data;
            },
            "==": function(other) {
                return this._node === other._node;
            },
            "clone": function() {
                return new ListIterator(this._node);
            }
        });
        
        AbstractList.dataType = undefined;
        AbstractList.iterator = ListIterator;
        
        AbstractList.template = function(data) {
            
            if (!(data instanceof Function)) {
                throw new Error("Wrong argument to template a list");
            }
            
            var List = AbstractList.inherit({
                "className": "List",
                "constructor": function(owning) {
                    AbstractList.fn.constructor.call(this, owning);
                }
            });
            
            List.dataType = data;
            
            return List;
        };
        
        return AbstractList;
    });
})();