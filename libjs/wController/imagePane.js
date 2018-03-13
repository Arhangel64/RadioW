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
    "destructor": function() {
        if (this._hasImage) {
            this._uncyclic.push(function() {
                this.image.destructor();
            });
        }
        
        Vocabulary.fn.destructor.call(this);
    },
    "addElement": function(key, element) {
        if (key === "image" && !this._hasImage) {
            this._hasImage = true;
            this.image = new File(new Address(["images", element.toString()]));
            this.image.on("serviceMessage", this._onControllerServiceMessage, this);
            
            if (this._registered) {
                global.registerForeignController("Corax", this.image);
            }
            
            if (this._subscribed) {
                global.subscribeForeignController("Corax", this.image);
            }
        }
        Vocabulary.fn.addElement.call(this, key, element);
    },
    "hasImage": function() {
        return this._hasImage;
    },
    "removeElement": function(key) {
        Vocabulary.fn.removeElement.call(this, key);
        
        if (key === "image" && this._hasImage) {
            if (this._subscribed) {
                global.unsubscribeForeignController("Corax", this.image);
            }
            
            if (this._registered) {
                global.unregisterForeignController("Corax", this.image);
            }
            
            this._hasImage = false;
            this.image.destructor();
            this.image = null;
        }
    },
    "unregister": function() {
        Vocabulary.fn.unregister.call(this);
        
        if (this._hasImage) {
            global.unregisterForeignController("Corax", this.image);
        }
    },
    "unsubscribe": function() {
        Vocabulary.fn.unsubscribe.call(this);
        
        if (this._hasImage) {
            global.unsubscribeForeignController("Corax", this.image);
        }
    }
});

module.exports = ImagePane;
