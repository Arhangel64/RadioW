"use strict";

var Page = require("../lib/wModel/page");
var String = require("../lib/wModel/string");
var ProxyCatModel = require("../lib/wModel/proxy/catalogue");
var PaneModel = require("../lib/wModel/proxy/pane");

var Address = require("../lib/wType/address");
var Uint64 = require("../lib/wType/uint64");

var Handler = require("../lib/wDispatcher/handler");

var List = Page.inherit({
    "className": "ListPage",
    "constructor": function(address, name, remoteAddress, socket, ChildClass) {
        Page.fn.constructor.call(this, address, name);
        
        this._proxySocket = socket;
        this._ChildClass = ChildClass;
        
        var header = new String(this._address["+"](new Address(["header"])), name);
        header.addProperty("fontFamily", "casualFont");
        this.addItem(header, 0, 0, 1, 1, Page.Aligment.CenterTop);

        this._list = new ProxyCatModel(
            this._address["+"](new Address(["list"])), 
            remoteAddress, 
            {
                sorting: {ascending: true, field: "name"}
            }, 
            socket
        );
        this._list.className = "PanesList";
        var PaneClass = PaneModel[name];
        if (!PaneClass) {
            PaneClass = PaneModel.inherit({});
            PaneClass.pageAddress = address;
        }
        this._list.setChildrenClass(PaneClass);
        this.addItem(this._list, 1, 0, 1, 1, Page.Aligment.CenterTop);
        
        this._reporterHandler = new Handler(this._address["+"](new Address(["subscribeMember"])), this, this._h_subscribeMember);
    },
    "destructor": function() {
        this._reporterHandler.destructor();
        this.removeItem(this._list);
        this._list.destructor();
        
        Page.fn.destructor.call(this);
    },
    "_createChildPage": function(id) {
        var childName = id.toString();
        var postfix = new Address([childName]);
        var childAddr = this._address["+"](postfix);
        
        var child = new this._ChildClass(childAddr, childName, id.clone(), this._proxySocket);
        this.addPage(child);
        child.on("destroyMe", this._destroyChild.bind(this, child));     //to remove model if it has no subscribers
        child.checkSubscribersAndDestroy();
        
        return child;
    },
    "_destroyChild": function(child) {
        this.removePage(child);
        child.destructor();
    },
    "getChildPage": function(name) {
        var child = this._childPages[name];
        if (child === undefined) {
            var int = parseInt(name);
            if (int == name) {
                var id = new Uint64(int);
                var itr = this._list.controller.data.find(id);
                if (!itr["=="](this._list.controller.data.end())) {
                    child = this._createChildPage(id);
                }
            }
        }
        
        return child;
    },
    "_h_subscribeMember": function(ev) {
        var dest = ev.getDestination();
        var lastHops = dest["<<"](this._address.size());
        
        if (lastHops.size() === 2) {
            var command = lastHops.back().toString();
            var numId = parseInt(lastHops.front().toString());
            if ((command === "subscribe" || command === "get" || command === "ping") && numId === numId) {
                var id = new Uint64(numId);
                var child = this._createChildPage(id);
                child["_h_" + command](ev);
            } else {
                this.trigger("serviceMessage", "ListPage model got a strange event: " + ev.toString(), 1);
            }
        } else {
            this.trigger("serviceMessage", "ListPage model got a strange event: " + ev.toString(), 1);
        }
    },
    "setParentReporter": function(pr) {
        Page.fn.setParentReporter.call(this, pr);
        
        this._pr.registerParent(this._list.getAddress(), this._list.reporterHandler);
        this._pr.registerParent(this.getAddress(), this._reporterHandler);
        this._list.subscribe();
    },
    "unsetParentReporter": function() {
        this._pr.unregisterParent(this.getAddress());
        this._pr.unregisterParent(this._list.getAddress());
        
        Page.fn.unsetParentReporter.call(this);
    }
});

module.exports = List;
