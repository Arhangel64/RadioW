"use strict";
(function abstractpair_js() {
    var moduleName = "lib/wContainer/abstractpair";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    
    define(moduleName, defineArray, function abstractpair_module() {
        var Class = require("lib/utils/class");
        
        var AbstractPair = Class.inherit({
            "className": "AbstractPair",
            "constructor": function(first, second) {
                Class.fn.constructor.call(this);
                
                if (!this.constructor.firstType || !this.constructor.secondType) {
                    throw new Error("An attempt to instantiate a pair without declared member types");
                }
                if (!(first instanceof this.constructor.firstType) || !(second instanceof this.constructor.secondType)) {
                    throw new Error("An attempt to construct a pair from wrong arguments");
                }
                
                this.first = first;
                this.second = second;
            },
            "destructor": function() {
                this.first.destructor();
                this.second.destructor();
                
                Class.fn.destructor.call(this);
            },
            "<": function(other) {
                if (!(other instanceof this.constructor)) {
                    throw new Error("Can't compare pairs with different content types");
                }
                return this.first["<"](other.first);
            },
            ">": function(other) {
                if (!(other instanceof this.constructor)) {
                    throw new Error("Can't compare pairs with different content types");
                }
                return this.first[">"](other.first);
            },
            "==": function(other) {
                if (!(other instanceof this.constructor)) {
                    throw new Error("Can't compare pairs with different content types");
                }
                return this.first["=="](other.first);
            }
        });
        
        AbstractPair.firstType = undefined;
        AbstractPair.secondType = undefined;
        
        AbstractPair.template = function(first, second) {
            if (!(first instanceof Function) || !(second instanceof Function)) {
                throw new Error("An attempt to create template pair from wrong arguments");
            }
            if (
                    !(first.prototype["<"] instanceof Function) ||
                    !(first.prototype[">"] instanceof Function) ||
                    !(first.prototype["=="] instanceof Function)
               ) 
            {
                throw new Error("Not acceptable first type");
            }
            var Pair = AbstractPair.inherit({
                "className": "Pair",
                "constructor": function(first, second) {
                    AbstractPair.fn.constructor.call(this, first, second);
                }
            });
            Pair.firstType = first;
            Pair.secondType = second;
            
            return Pair;
        };
        
        return AbstractPair;
    });
})();