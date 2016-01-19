"use strict";
(function js() {
    var moduleName = "lib/utils/subscribable";

    var defineArray = [];
    defineArray.push("lib/utils/class");

    define(moduleName, defineArray, function module() {
        var Class = require("lib/utils/class");

        var Subscribable = Class.inherit({
            "className": "Subscribable",
            "constructor": function() {
                Class.fn.constructor.call(this);

                this._events = {};
            },
            "destructor": function() {
                this.off();
                Class.fn.destructor.call(this);
            },

            "on": function(name, handler, context) {
                var handlers = this._events[name];

                if (typeof name !== "string") {
                    throw new Error("Name of event is mandatory");
                }

                if (!(handler instanceof Function)) {
                    throw new Error("Handler of event is mandatory");
                }

                if (!handlers) {
                    handlers = [];
                    this._events[name] = handlers;
                }

                handlers.push({
                    handler: handler,
                    context: context || this,
                    once: false
                });
            },
            "one": function(name) {
                CSubscribable.fn.on.apply(this, arguments);
                this._events[name][this._events[name].length - 1].once = true;
            },
            "off": function(name, handler) {

                if (typeof name === "string" && this._events[name]) {
                    if (handler instanceof Function) {
                        var handlers = this._events[name];
                        for (var i = handlers.length - 1; i >= 0 ; --i) {
                            if (handlers[i].handler === handler) {
                                handlers.splice(i, 1);
                            }
                        }
                    } else {
                        delete this._events[name];
                    }
                } else {
                    this._events = {};
                }
            },
            "trigger": function() {
                var args = [].slice.call(arguments);
                if (args.length === 0) {
                    throw new Error("Name of event is mandatory");
                }
                var answer = false;
                var name = args.shift();
                var handlers = this._events[name];
                if (handlers) {
                    for (var i = 0; i < handlers.length; ++i) {
                        var handle = handlers[i];
                        answer = handle.handler.apply(handle.context, args);
                        if (handle.once) {
                            handlers.splice(i, 1);
                            --i;
                        }

                        if (answer === false) {
                            return false;
                        }
                    }
                }
                return answer;
            }
        });

        return Subscribable;
    })
})();