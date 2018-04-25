"use strict";

var Address = require("../wType/address");

var Controller = require("./controller");
var File = require("./file/file");

var Image = Controller.inherit({
    "className": "Image",
    "constructor": function(addr) {
        Controller.fn.constructor.call(this, addr);
        
        this.data = undefined;
        this._hasCtrl = false;
        this._fileCtrl = undefined;
        this._need = 0;
        
        this.addHandler("get");
    },
    "dontNeedData": function() {
        --this._need;
    },
    "getMimeType": function () {
        return this._fileCtrl.getMimeType();
    },
    "hasData": function() {
        if (this._hasCtrl) {
            return this._fileCtrl.hasData();
        }
        return false;
    },
    "_h_get": function(ev) {
        var data = ev.getData();
        
        if (this._hasCtrl) {
            this.removeForeignController(this._fileCtrl);
            this._fileCtrl.destructor();
            delete this._fileCtrl;
            this._hasCtrl = false;
        }
        var strId = data.at("data").toString();
        if (strId !== "0") {
            this._fileCtrl = new File(new Address(["images", strId]));
            this.addForeignController("Corax", this._fileCtrl);
            
            this._fileCtrl.on("data", this._onControllerData, this);
            
            this._hasCtrl = true;
            if (this._need > 0) {
                this._fileCtrl.needData();
            }
        } else {
            this.trigger("clear");
        }
    },
    "needData": function() {
        if (this._need === 0 && this._hasCtrl) {
            this._fileCtrl.needData();
        }
        ++this._need;
    },
    "_onControllerData": function() {
        this.data = this._fileCtrl.data;
        
        this.trigger("data");
    }
});

module.exports = Image;
