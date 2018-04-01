"use strict";
var Class = require("../utils/class");

var ByteArray = Class.inherit({
    "className": "ByteArray",
    "constructor": function(size) {
        Class.fn.constructor.call(this);
        
        this._referenceMode = false;
        this._data = new Uint8Array(size);
        this._shiftBegin = 0;
        this._shiftEnd = 0;
    },
    "_checkReference": function() {
        if (this._referenceMode) {
            var buffer = new ArrayBuffer(this._data.length - this._shiftBegin);
            var newData = new Uint8Array(buffer);
            newData.set(this._data, this._shiftBegin);
            this._data = newData;
            this._shiftBegin = 0;
            this._referenceMode = false;
        }
    },
    "fill": function(/*Uint8Array*/arr, /*Number*/size, /*[Number]*/shift) {
        this._checkReference();
        shift = shift || 0;
        
        if (this._shiftEnd === 0 && (this._data.length <= size - shift)) {
            this._referenceMode = true;
            this._data = arr.subarray(shift, this._data.length + shift);
            this._shiftEnd = this._data.length;
            shift += this._shiftEnd;
        } else {
            while (!this.filled() && shift < size) {
                this._data[this._shiftEnd] = arr[shift];
                ++shift;
                ++this._shiftEnd;
            }
        }
        
        return shift;
    },
    "filled": function() {
        return this._data.length === this._shiftEnd;
    },
    "size": function() {
        return this._shiftEnd - this._shiftBegin;
    },
    "maxSize": function() {
        return this._data.length;
    },
    "push8": function(int) {
        this._checkReference();
        
        this._data[this._shiftEnd] = int;
        ++this._shiftEnd;
    },
    "push16": function(int) {
        var h = (int >> 8) & 0xff;
        var l = int & 0xff;
        
        this.push8(h);
        this.push8(l);
    },
    "push32": function(int) {
        var hh = (int >> 24) & 0xff;
        var hl = (int >> 16) & 0xff;
        var lh = (int >> 8) & 0xff;
        var ll = int & 0xff;
        
        this.push8(hh);
        this.push8(hl);
        this.push8(lh);
        this.push8(ll);
    },
    "push64": function(int) {
        
    },
    "pop8": function(int) {
        var ret = this._data[this._shiftBegin];
        ++this._shiftBegin;
        return ret;
    },
    "pop16": function(int) {
        var ret = (this.pop8() << 8);
        ret = ret | this.pop8();
        
        return ret;
    },
    "pop32": function(int) {
        var ret = this.pop8() << 24;
        ret = ret | (this.pop8() << 16);
        ret = ret | (this.pop8() << 8);
        ret = ret | this.pop8();
        
        return ret;
    },
    "pop64": function(int) {
        
    },
    "data": function() {
        return this._data.subarray(this._shiftBegin, this._shiftEnd);
    }
});

module.exports = ByteArray;
