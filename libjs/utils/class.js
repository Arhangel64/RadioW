"use strict";

var Class = function() {};
        
Class.inherit = function(proto) {
    var superClass = this;
    var Base = function() {};
    var subclass = proto && (proto.constructor !== Object) ? proto.constructor : function() {
        superClass.apply(this, arguments)
    };
    
    Base.prototype = this.prototype;
    var fn = new Base();
    
    for (var key in proto) {
        if (proto.hasOwnProperty(key)) {
            fn[key] = proto[key];
        }
    }
    for (var method in this) {
        if (this.hasOwnProperty(method)) {
            subclass[method] = this[method];
        }
    }
    
    fn.constructor = subclass;
    subclass.prototype = subclass.fn = fn;
    
    return subclass;
}

module.exports = Class.inherit({
    className: "Class",
    constructor: function() {
        if (!(this instanceof Class)) {
            throw new SyntaxError("You didn't call \"new\" operator");
        }
        
        Class.call(this);
        this._uncyclic = [];
    },
    destructor: function() {
        for (var i = 0; i < this._uncyclic.length; ++i) {
            this._uncyclic[i].call(this);
        }

        for (var key in this) {
            this[key] = undefined;
            delete this[key];
        }
    }
});
