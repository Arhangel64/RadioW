"use strict";
var counter = 0;
var Subscribable = require("../utils/subscribable");
var Handler = require("../wDispatcher/handler");
var Address = require("../wType/address");
var String = require("../wType/string");
var Vocabulary = require("../wType/vocabulary");
var Event = require("../wType/event");

var Controller = Subscribable.inherit({
    "className": "Controller",
    "constructor": function(addr) {
        Subscribable.fn.constructor.call(this);
        
        this._subscribed = false;
        this._registered = false;
        this._pairAddress = addr;
        this._address = new Address([this.className.toString() + counter++]);
        
        this._handlers = [];
        this._controllers = [];
        this.properties = [];
        
        this.addHandler("properties");
    },
    "destructor": function() {
        var i;

        if (this._subscribed) {
            this.unsubscribe();
        }
        if (this._registered) {
            this.unregister();
        }
        
        for (i = 0; i < this._controllers.length; ++i) {
            this._controllers[i].destructor();
        }
        
        for (i = 0; i < this._handlers.length; ++i) {
            this._handlers[i].destructor();
        }
        this._pairAddress.destructor();
        this._address.destructor();
        
        Subscribable.fn.destructor.call(this);
    },
    "addController": function(controller) {
        if (!(controller instanceof Controller)) {
            throw new Error("An attempt to add not a controller into " + this.className);
        }
        controller.on("serviceMessage", this._onControllerServiceMessage, this);
        this._controllers.push(controller);
        if (this._registered) {
            controller.register(this._dp, this._socket);
        }
        if (this._subscribed && !controller._subscribed) {
            controller.subscribe();
        }
        this.trigger("newController", controller);
    },
    "addHandler": function(name) {
        if (!(this["_h_" + name] instanceof Function)) {
            throw new Error("An attempt to create handler without a handling method");
        }
        
        var handler = new Handler(this._address["+"](new Address([name])), this, this["_h_" + name]);
        
        this._handlers.push(handler);
        if (this._registered) {
            this._dp.registerHandler(handler);
        }
    },
    "getPairAddress": function() {
        return this._pairAddress.clone();
    },
    "_h_properties": function(ev) {
        this.trigger("clearProperties");
        this.properties = [];
        var data = ev.getData();
        var list = data.at("properties");
        var size = list.size();
        for (var i = 0; i < size; ++i) {
            var vc = list.at(i);
            var pair = {p: vc.at("property").toString(), k: vc.at("key").toString()};
            this.properties.push(pair);
            this.trigger("addProperty", pair.k, pair.p);
        }
    },
    "_onControllerServiceMessage": function(msg, severity) {
        this.trigger("serviceMessage", msg, severity);
    },
    "_onSocketDisconnected": function() {
        this._subscribed = false;
    },
    "register": function(dp, socket) {
        var i;
        if (this._registered) {
            throw new Error("Controller " + this._address.toString() + " is already registered");
        }
        this._dp = dp;
        this._socket = socket;
        socket.on("disconnected", this._onSocketDisconnected, this);
        
        for (i = 0; i < this._controllers.length; ++i) {
            this._controllers[i].register(this._dp, this._socket);
        }
        
        for (i = 0; i < this._handlers.length; ++i) {
            dp.registerHandler(this._handlers[i]);
        }
        this._registered = true;
    },
    "removeController": function(ctrl) {
        if (!(ctrl instanceof Controller)) {
            throw new Error("An attempt to remove not a controller from " + this.className);
        }
        var index = this._controllers.indexOf(ctrl);
        if (index !== -1) {
            if (this._subscribed) {
                ctrl.unsubscribe();
            }
            if (this._dp) {
                ctrl.unregister();
            }
            this._controllers.splice(index, 1);
        
            this.trigger("removedController", ctrl);
        } else {
            throw new Error("An attempt to remove not not existing controller from " + this.className);
        }
    },
    "send": function(vc, handler) {
        if (!this._registered) {
            throw new Error("An attempt to send event from model " + this._address.toString() + " which is not registered");
        }
        var addr = this._pairAddress["+"](new Address([handler]));
        var id = this._socket.getId().clone();
        
        vc.insert("source", this._address.clone());
        
        var ev = new Event(addr, vc);
        ev.setSenderId(id);
        this._socket.send(ev);
        ev.destructor();
    },
    "subscribe": function() {
        if (this._subscribed) {
            throw new Error("An attempt to subscribe model " + this._address.toString() + " which is already subscribed");
        }
        this._subscribed = true;
        
        var vc = new Vocabulary();
        this.send(vc, "subscribe");
        
        for (var i = 0; i < this._controllers.length; ++i) {
            this._controllers[i].subscribe();
        }
    },
    "unregister": function() {
        if (!this._registered) {
            throw new Error("Controller " + this._address.toString() + " is not registered");
        }
        var i;
        
        for (i = 0; i < this._controllers.length; ++i) {
            this._controllers[i].unregister();
        }
        for (i = 0; i < this._handlers.length; ++i) {
            this._dp.unregisterHandler(this._handlers[i]);
        }
        this._socket.off("disconnected", this._onSocketDisconnected, this);
        delete this._dp;
        delete this._socket;
        
        this._registered = false;
    },
    "unsubscribe": function() {
        if (!this._subscribed) {
            throw new Error("An attempt to unsubscribe model " + this._address.toString() + " which is not subscribed");
        }
        this._subscribed = false;
        
        if (this._socket.isOpened()) {
            var vc = new Vocabulary();
            this.send(vc, "unsubscribe");
        }
        
        for (var i = 0; i < this._controllers.length; ++i) {
            this._controllers[i].unsubscribe();
        }
    }
});

Controller.createByType = function(type, address) {
    var typeName = this.ReversedModelType[type];
    if (typeName === undefined) {
        throw new Error("Unknown ModelType: " + type);
    }
    var Type = this.constructors[typeName];
    if (Type === undefined) {
        throw new Error("Constructor is not loaded yet, something is wrong");
    }
    return new Type(address);
}

Controller.initialize = function(rc, cb) {
    var deps = [];
    var types = [];
    for (var key in this.ModelTypesPaths) {
        if (this.ModelTypesPaths.hasOwnProperty(key)) {
            if (!rc || rc.indexOf(key) !== -1) {
                deps.push(this.ModelTypesPaths[key]);
                types.push(key);
            }
        }
    }
    require(deps, function() {
        for (var i = 0; i < types.length; ++i) {
            Controller.constructors[types[i]] = arguments[i];
        }
        cb();
    });
}

Controller.ModelType = {
    String:         0,
    List:           1,
    Vocabulary:     2,
    
    Attributes:     50,
    
    GlobalControls: 100,
    Link:           101,
    Page:           102,
    PageStorage:    103,
    PanesList:      104,
    Theme:          105,
    ThemeStorage:   106
};

Controller.ReversedModelType = {
    "0":        "String",
    "1":        "List",
    "2":        "Vocabulary",
    
    "50":       "Attributes",
    
    "100":      "GlobalControls",
    "101":      "Link",
    "102":      "Page",
    "103":      "PageStorage",
    "104":      "PanesList",
    "105":      "Theme",
    "106":      "ThemeStorage"
};

Controller.ModelTypesPaths = {
    String:         "./string",         //resolve as dependency
    List:           "./list",           //resolve as dependency
    Vocabulary:     "./vocabulary",     //resolve as dependency
    Attributes:     "./attributes",     //resolve as dependency
    GlobalControls: "./globalControls", //resolve as dependency
    Link:           "./link",           //resolve as dependency
    Page:           "./page",           //resolve as dependency
    PageStorage:    "./pageStorage",    //resolve as dependency
    PanesList:      "./panesList",      //resolve as dependency
    Theme:          "./theme",          //resolve as dependency
    ThemeStorage:   "./themeStorage"    //resolve as dependency
};

Controller.constructors = {};

module.exports = Controller;
