"use strict";
var Class = require("../lib/utils/class");
var String = require("../lib/wType/string");
var Address = require("../lib/wType/address");
var AbstractMap = require("../lib/wContainer/abstractmap");
var AbstractList = require("../lib/wContainer/abstractlist");
var AbstractOrder = require("../lib/wContainer/abstractorder");

var Test = Class.inherit({
    "className": "Test",
    "constructor": function() {
        Class.fn.constructor.call(this);
        
        this.testMap();
        this.testList();
        this.testOrder();
    },
    "testMap": function() {
        var Map = AbstractMap.template(Address, String);
        var map = new Map();
        
        var noEl = map.find(new Address(["addr1", "hop1"]));
        if (!noEl["=="](map.end())) {
            console.log("error: problem with end!");
        }
        
        map.insert(new Address(["addr1", "hop1"]), new String("hello"));
        map.insert(new Address(["addr2", "hop2"]), new String("world"));
        
        if (map.size() !== 2) {
            console.log("error: problem with insertion!");
        }
        
        var itr = map.find(new Address(["addr1", "hop1"]));
        
        if (itr["=="](map.end())) {
            console.log("error: problem with finding!");
        }
        
        console.log(itr["*"]().second.toString());
        itr["++"]();
        console.log(itr["*"]().second.toString());
        
        map.erase(itr);
        
        itr = map.find(new Address(["addr2", "hop2"]));
        
        if (!itr["=="](map.end())) {
            console.log("error: problem with erasing!");
        }
        
        map.clear();
        
        if (map.size() > 0) {
            console.log("error: problem with clearing!");
        }
        map.destructor();
    },
    "testList": function() {
        var List = AbstractList.template(String);
        
        var list = new List();
        
        if (!list.begin()["=="](list.end())) {
            console.log("error: problem with empty list");
        }
        
        list.push_back(new String("h"));
        
        if (list.size() !== 1) {
            console.log("error: problem with size");
        }
        var beg = list.begin();
        var end = list.end();
        
        beg["++"]();
        end["--"]();
        
        if (!beg["=="](list.end())) {
            console.log("error: problem with iterator incrementation");
        }
        if (!end["=="](list.begin())) {
            console.log("error: problem with iterator decrementation");
        }
        
        list.pop_back();
        
        if (!list.begin()["=="](list.end())) {
            console.log("error: problem with empty list");
        }
        
        list.push_back(new String("h"));
        list.push_back(new String("e"));
        list.push_back(new String("l"));
        list.push_back(new String("l"));
        list.push_back(new String("o"));
        list.push_back(new String(","));
        list.push_back(new String(" "));
        list.push_back(new String("w"));
        list.push_back(new String("w"));
        
        var itr = list.end();
        itr["--"]();
        
        list.push_back(new String("o"));
        list.push_back(new String("r"));
        list.push_back(new String("l"));
        list.push_back(new String("d"));
        list.push_back(new String("!"));
        
        list.erase(itr);
        
        beg = list.begin();
        end = list.end();
        
        var str = new String();
        
        for (; !beg["=="](end); beg["++"]()) {
            str["+="](beg["*"]());
        }
        
        console.log(str.toString());
        
        list.clear();
        
        if (!list.begin()["=="](list.end())) {
            console.log("error: problem with empty list");
        }
        
        list.destructor();
    },
    "testOrder": function() {
        var Order = AbstractOrder.template(Address);
        
        var order = new Order();
        
        var addr1 = new Address(["hop1", "hop2"]);
        order.push_back(addr1);
        
        if (order.size() !== 1) {
            console.log("error: problem with size");
        }
        
        var begin = order.begin();
        var itr = begin.clone();
        
        if (begin["*"]() !== addr1) {
            console.log("error: problem with iterator");
        }
        
        itr["++"]();
        if (!itr["=="](order.end())) {
            console.log("error: problem with iterator, end");
        }
        
        if (!order.find(addr1)["=="](begin)) {
            console.log("error: problem with finding");
        }
        
        order.erase(addr1);
        console.log(addr1);
        
        if (!order.begin()["=="](order.end())) {
            console.log("error: problem with empty order");
        }
        order.push_back(new Address(["hop1", "hop2"]))
        order.push_back(new Address(["hop1", "hop3"]))
        order.push_back(new Address(["hop1", "hop4"]))
        order.push_back(new Address(["hop1", "hop5"]))
        order.push_back(new Address(["hop1", "hop6"]))
        order.push_back(new Address(["hop1", "hop7"]))
        
        if (order.size() !== 6) {
            console.log("error: problem with size");
        }
        
        itr = order.find(new Address(["hop1", "hop4"]));
        var end = order.end();
        
        for (; !itr["=="](end); itr["++"]()) {
            console.log(itr["*"]().toString());
        }
    }
});

module.exports = Test;