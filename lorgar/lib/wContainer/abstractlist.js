"use strict";
(function abstractlist_js() {
    var moduleName = "lib/wContainer/abstractlist";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    
    define(moduleName, defineArray, function abstractlist_module() {
        var Class = require("lib/utils/class");
        
        var AbstractList = Class.inherit({
            "className": "AbstractList",
            "constructor": function() {
                Class.fn.constructor.call(this);
                
                this._begin = new ListNode();
                this._end = this._begin;
                this._size = 0;
            },
            "destructor": function() {
                Class.fn.destructor.call(this);
            },
            "begin": function() {
                return new ListIterator(this._begin);
            },
            "end": function() {
                return new ListIterator(this._end);
            },
            "pop_front": function() {
                if (this._begin === this._end) {
                    throw new Error("An attempt to pop from empty list");
                }
                var node = this._end._prev;
                
                if (node === this._begin) {
                    this._begin = this._end;
                } else {
                    node._prev._next = this._end;
                    this._end._prev = node._prev;
                }
                if (node._data instanceof Class) {
                    node._data.destructor();
                } 
            },
            "push_back": function(data) {
                var node = new ListNode();
                if (this._size === 0) {
                    this._begin = node;
                } else {
                    this._end._prev._next = node;
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
            "constructor": function(data, next) {
                Class.fn.constructor.call(this);
                
                this._data = null;
                this._next = null;
                this._prev = null;
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
                return this._node._data === other._node._data;
            }
        });
        
        AbstractList.dataType = undefined;
        
        AbstractList.template = function(data) {
            
            var List = AbstractList.inherit({
                "className": "List",
                "constructor": function() {
                    AbstractList.fn.constructor.call(this);
                }
            });
            
            List.dataType = data;
            
            return List;
        };
        
        return AbstractList;
    });
})();