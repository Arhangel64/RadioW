"use strict";
var WTest = require("./test");
var AbstractList = require("../wContainer/abstractlist");

var String = require("../wType/string");

var TAbsctractList = WTest.inherit({
    "className": "TAbsctractList",
    "constructor": function() {
        WTest.fn.constructor.call(this, "AbstractList");
        
        var List = AbstractList.template(String);
        this._list = new List();
        
        this._actions.push(this.testBeginEnd);
        this._actions.push(this.testSize);
        this._actions.push(this.testIterators);
        this._actions.push(this.testErasing);
        this._actions.push(this.testClear);
        this._actions.push(this.testInsertion);
    },
    "destructor": function() {
        this._list.destructor();
        
        WTest.fn.destructor.call(this);
    },
    "testBeginEnd": function() {
        if (!this._list.begin()["=="](this._list.end())) {
            throw new Error("problem with empty list");
        }
    },
    "testSize": function() {
        this._list.push_back(new String("h"));
        
        if (this._list.size() !== 1) {
            throw new Error("problem with size");
        }
    },
    "testInsertion": function() {
        var str1 = new String("one");
        this._list.insert(str1, this._list.end());
        
        if (!this._list.begin()["*"]()["=="](str1)) {
            throw new Error("Problem with insertion to an empty list");
        }
        
        var str2 = new String("two");
        this._list.insert(str2, this._list.begin());
        
        if (!this._list.begin()["*"]()["=="](str2)) {
            throw new Error("Problem with insertion to the beginning of the list");
        }
        
        var itr = this._list.begin();
        itr["++"]();
        var str3 = new String("oneAndAHalf");
        this._list.insert(str3, itr);
        
        itr["--"]();
        if (!itr["*"]()["=="](str3)) {
            throw new Error("Problem with insertion to the middle of the list");
        }
        
        var arr = [str2, str3, str1];
        var itr1 = this._list.begin();
        for (var i = 0; i < arr.length; ++i, itr1["++"]()) {
            if (!itr1["*"]()["=="](arr[i])) {
                throw new Error("Problem with the order of elements in list after insertion");
            }
        }
    },
    "testIterators": function() {
        var beg = this._list.begin();
        var end = this._list.end();
        
        beg["++"]();
        end["--"]();
        
        if (!beg["=="](this._list.end())) {
            throw new Error("problem with iterator incrementation");
        }
        if (!end["=="](this._list.begin())) {
            throw new Error("problem with iterator decrementation");
        }
        
        this._list.pop_back();
        
        if (!this._list.begin()["=="](this._list.end())) {
            throw new Error("problem with empty list");
        }
    },
    "testErasing": function() {
        this._list.push_back(new String("h"));
        this._list.push_back(new String("e"));
        this._list.push_back(new String("l"));
        this._list.push_back(new String("l"));
        this._list.push_back(new String("o"));
        this._list.push_back(new String(","));
        this._list.push_back(new String(" "));
        this._list.push_back(new String("w"));
        this._list.push_back(new String("w"));
        
        var itr = this._list.end();
        itr["--"]();
        
        this._list.push_back(new String("o"));
        this._list.push_back(new String("r"));
        this._list.push_back(new String("l"));
        this._list.push_back(new String("d"));
        this._list.push_back(new String("!"));
        
        this._list.erase(itr);
        
        var beg = this._list.begin();
        var end = this._list.end();
        
        var str = new String();
        
        for (; !beg["=="](end); beg["++"]()) {
            str["+="](beg["*"]());
        }
        
        if (str.toString() !== "hello, world!") {
            throw new Error("Error push back and erasing");
        }
    },
    "testClear": function() {
        this._list.clear();
        
        if (!this._list.begin()["=="](this._list.end())) {
            throw new Error("problem with empty list");
        }
    }
});

module.exports = TAbsctractList;
