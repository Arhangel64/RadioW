"use strict";
(function test_js() {
    var moduleName = "test/test";
    
    var defineArray = [];
    defineArray.push("lib/utils/class");
    defineArray.push("lib/wType/string");
    defineArray.push("lib/wType/address");
    defineArray.push("lib/wContainer/abstractmap")
    defineArray.push("lib/wContainer/abstractlist")
    defineArray.push("lib/wContainer/abstractorder")
    
    define(moduleName, defineArray, function test_module() {
        var Class = require("lib/utils/class");
        var String = require("lib/wType/string");
        var Address = require("lib/wType/address");
        var AbstractMap = require("lib/wContainer/abstractmap");
        var AbstractList = require("lib/wContainer/abstractlist");
        var AbstractOrder = require("lib/wContainer/abstractorder");
        
        var Test = Class.inherit({
            "className": "Test",
            "constructor": function() {
                Class.fn.constructor.call(this);
            },
            "testMap": function() {
                var Map = AbstractMap.template(Address, String);
                var map = window.map = new Map();
                
                var noEl = map.find(new Address(["addr1", "hop1"]));
                if (!noEl["=="](map.end())) {
                    throw new Error("problem with end!");
                }
                
                map.insert(new Address(["addr1", "hop1"]), new String("hello"));
                map.insert(new Address(["addr2", "hop2"]), new String("world"));
                
                if (map.size() !== 2) {
                    throw new Error("problem with insertion!");
                }
                
                var itr = map.find(new Address(["addr1", "hop1"]));
                
                if (itr["=="](map.end())) {
                    throw new Error("problem with finding!");
                }
                
                if (itr["*"]().second.toString() !== "hello") {
                    throw new Error("wrong element found");
                }
                itr["++"]();
                if (itr["*"]().second.toString() !== "world") {
                    throw new Error("iterator dereferenced into wrong element after incrementetion");
                }
                
                map.erase(itr);
                
                itr = map.find(new Address(["addr2", "hop2"]));
                
                if (!itr["=="](map.end())) {
                    throw new Error("problem with erasing!");
                }
                
                map.clear();
                
                if (map.size() > 0) {
                    throw new Error("problem with clearing!");
                }
                map.destructor();
            },
            "testList": function() {
                var List = AbstractList.template(String);
                
                var list = new List();
                
                if (!list.begin()["=="](list.end())) {
                   throw new Error("problem with empty list");
                }
                
                list.push_back(new String("h"));
                
                if (list.size() !== 1) {
                    throw new Error("problem with size");
                }
                var beg = list.begin();
                var end = list.end();
                
                beg["++"]();
                end["--"]();
                
                if (!beg["=="](list.end())) {
                    throw new Error("problem with iterator incrementation");
                }
                if (!end["=="](list.begin())) {
                    throw new Error("problem with iterator decrementation");
                }
                
                list.pop_back();
                
                if (!list.begin()["=="](list.end())) {
                    throw new Error("problem with empty list");
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
                
                if (str.toString() !== "hello, world!") {
                    throw new Error("Error push back and erasing");
                }
                
                list.clear();
                
                if (!list.begin()["=="](list.end())) {
                    throw new Error("problem with empty list");
                }
                
                list.destructor();
            },
            "testOrder": function() {
                var Order = AbstractOrder.template(Address);
                
                var order = new Order();
                
                var addr1 = new Address(["hop1", "hop2"]);
                order.push_back(addr1);
                
                if (order.size() !== 1) {
                    throw new Error("problem with size");
                }
                
                var begin = order.begin();
                var itr = begin.clone();
                
                if (begin["*"]() !== addr1) {
                    throw new Error("problem with iterator");
                }
                
                itr["++"]();
                if (!itr["=="](order.end())) {
                    throw new Error("problem with iterator, end");
                }
                
                if (!order.find(addr1)["=="](begin)) {
                    throw new Error("problem with finding");
                }
                
                order.erase(addr1);
                if (addr1.toString() !== new Address(["hop1", "hop2"]).toString()) {
                    throw new Error("key have been destroyed afrer eresing element");
                }
                
                if (!order.begin()["=="](order.end())) {
                    throw new Error("error: problem with empty order");
                }
                order.push_back(new Address(["hop1", "hop2"]))
                order.push_back(new Address(["hop1", "hop3"]))
                order.push_back(new Address(["hop1", "hop4"]))
                order.push_back(new Address(["hop1", "hop5"]))
                order.push_back(new Address(["hop1", "hop6"]))
                order.push_back(new Address(["hop1", "hop7"]))
                
                if (order.size() !== 6) {
                    throw new Error("problem with size");
                }
                
                itr = order.find(new Address(["hop1", "hop4"]));
                var end = order.end();
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
                
                order.destructor();
            },
            "run": function() {
                console.log("Starting tests");
                try {
                    this.testList();
                    this.testMap();
                    this.testOrder();
                } catch(e) {
                    console.error("Testing failed!"):
                    console.error(e.message);
                    return;
                }
                console.log("Testing complete");
            }
        });
        
        return Test;
    });
})();