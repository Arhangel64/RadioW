"use strict";

var Address = require("../wType/address");

var Vocabulary = require("./vocabulary");
var File = require("./file/file");

var ImagePane = Vocabulary.inherit({
    "className": "ImagePane",
    "constructor": function(addr) {
        Vocabulary.fn.constructor.call(this, addr);
        
        this._hasImage = false;
        this.image = null;
    },
    "addElement": function(key, element) {
        if (key === "image" && !this._hasImage) {
            this._hasImage = true;
            this.image = new File(new Address(["images", element.toString()]));
            this.addForeignController("Corax", this.image);
        }
        Vocabulary.fn.addElement.call(this, key, element);
    },
    "hasImage": function() {
        return this._hasImage;
    },
    "removeElement": function(key) {
        Vocabulary.fn.removeElement.call(this, key);
        
        if (key === "image" && this._hasImage) {
            this.removeForeignController(this.image);
            
            this._hasImage = false;
            this.image.destructor();
            this.image = null;
        }
    }
});

module.exports = ImagePane;
