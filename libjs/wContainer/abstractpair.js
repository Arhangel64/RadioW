"use strict";
var Class = require("../utils/class");
        
var AbstractPair = Class.inherit({
    "className": "AbstractPair",
    "constructor": function(first, second) {
        Class.fn.constructor.call(this);
        
        if (!this.constructor.firstType || !this.constructor.secondType) {
            throw new Error("An attempt to instantiate a pair without declared member types");
        }
        if (!(first instanceof this.constructor.firstType)) {
            throw new Error("An attempt to construct a pair from wrong arguments");
        }
        
        this.first = first;
        this.second = second;
    },
    "destructor": function() {
        this.first.destructor();
        if (this.second) {
            this.second.destructor();
        }
            
        Class.fn.destructor.call(this);
    },
    "<": function(other) {
        if (!(other instanceof this.constructor)) {
            throw new Error("Can't compare pairs with different content types");
        }
        if (this.constructor.complete) {
            if (this.first["<"](other.first)) {
                return true;
            } else if(this.first["=="](other.first)) {
                this.second["<"](other.second);
            } else {
                return false;
            }
        } else {
            return this.first["<"](other.first);
        }
    },
    ">": function(other) {
        if (!(other instanceof this.constructor)) {
            throw new Error("Can't compare pairs with different content types");
        }
        if (this.constructor.complete) {
            if (this.first[">"](other.first)) {
                return true;
            } else if(this.first["=="](other.first)) {
                this.second[">"](other.second);
            } else {
                return false;
            }
        } else {
            return this.first[">"](other.first);
        }
    },
    "==": function(other) {
        if (!(other instanceof this.constructor)) {
            throw new Error("Can't compare pairs with different content types");
        }
        if (this.constructor.complete) {
            return this.first["=="](other.first) && this.second["=="](other.second);
        } else {
            return this.first["=="](other.first);
        }
    }
});

AbstractPair.firstType = undefined;
AbstractPair.secondType = undefined;
AbstractPair.complete = false;

AbstractPair.template = function(first, second, complete) {
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
    if (
            complete &&
            (
                !(second.prototype["<"] instanceof Function) ||
                !(second.prototype[">"] instanceof Function) ||
                !(second.prototype["=="] instanceof Function)
            )
        )
    {
        throw new Error("Not acceptable second type");
    }
    var Pair = AbstractPair.inherit({
        "className": "Pair",
        "constructor": function(first, second) {
            AbstractPair.fn.constructor.call(this, first, second);
        }
    });
    Pair.firstType = first;
    Pair.secondType = second;
    Pair.complete = complete === true;
    
    return Pair;
};

module.exports = AbstractPair;