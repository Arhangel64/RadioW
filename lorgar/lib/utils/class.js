"use strict";
(function class_js() {
    var moduleName = "lib/utils/class";
    var defineArray = [];
    
    define(moduleName, defineArray, function class_module() {
        var Class = function() {};
        
        Class.inherit = function(proto) {
            var superClass = this;
            var Base = function() {};
            var subclass = proto && (proto.constructor !== Object) ? proto.constructor : function() {
                superClass.apply(this, arguments)
            };
            
            Base.prototype = this.prototype;
            var fn = subclass.fn = new Base();
            subclass.prototype = fn;
            
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
            
            return subclass;
        }
        
        return Class.inherit({
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
                    this.uncyclic[i].call();
                }

                for (var key in this) {
                    if (this.hasOwnProperty(key)) {
                        this[key] = undefined;
                        delete this[key];
                    }
                }
            }
        });
    });
})();