"use strict";
var WTest = require("./test");
var AbstractOrder = require("../wContainer/abstractorder");
var Address = require("../wType/address");

var TAbstractOrder = WTest.inherit({
    "className": "TAbstractOrder",
    "constructor": function() {
        WTest.fn.constructor.call(this, "AbstractOrder");
        
        var Order = AbstractOrder.template(Address);
        this._order = new Order();
        
        this._actions.push(this.testSize);
        this._actions.push(this.testIterators);
        this._actions.push(this.testEmpty);
        this._actions.push(this.testPushBackFind);
    },
    "destructor": function() {
        this._order.destructor();
        
        WTest.fn.destructor.call(this);
    },
    "testSize": function() {
        var addr1 = new Address(["hop1", "hop2"]);
        this._order.push_back(addr1);
        
        if (this._order.size() !== 1) {
            throw new Error("problem with size");
        }
    },
    "testIterators": function() {
        var addr1 = new Address(["hop1", "hop2"]);
        
        var begin = this._order.begin();
        var itr = begin.clone();
        
        if (!begin["*"]()["=="](addr1)) {
            throw new Error("problem with iterator");
        }
        
        itr["++"]();
        if (!itr["=="](this._order.end())) {
            throw new Error("problem with iterator, end");
        }
        
        if (!this._order.find(addr1)["=="](begin)) {
            throw new Error("problem with finding");
        }
        
        this._order.erase(addr1);
        
        if (addr1.toString() !== new Address(["hop1", "hop2"]).toString()) {
            throw new Error("key have been destroyed afrer eresing element");
        }
    },
    "testEmpty": function() {
        if (!this._order.begin()["=="](this._order.end())) {
            throw new Error("error: problem with empty order");
        }
    },
    "testPushBackFind": function() {
        this._order.push_back(new Address(["hop1", "hop2"]))
        this._order.push_back(new Address(["hop1", "hop3"]))
        this._order.push_back(new Address(["hop1", "hop4"]))
        this._order.push_back(new Address(["hop1", "hop5"]))
        this._order.push_back(new Address(["hop1", "hop6"]))
        this._order.push_back(new Address(["hop1", "hop7"]))
        
        if (this._order.size() !== 6) {
            throw new Error("problem with size");
        }
        
        var itr = this._order.find(new Address(["hop1", "hop4"]));
        var end = this._order.end();
        var arr = [
            new Address(["hop1", "hop4"]),
            new Address(["hop1", "hop5"]),
            new Address(["hop1", "hop6"]),
            new Address(["hop1", "hop7"])
        ]
        var i = 0;
        
        for (; !itr["=="](end); itr["++"]()) {
            if (!itr["*"]()["=="](arr[i])) {
                throw new Error("problem with finding element in the middle and iteration to the end");
            }
            ++i;
        }
    }
});

module.exports = TAbstractOrder;