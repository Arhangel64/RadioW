"use strict";
var path = require("path");
module.exports = function(req, res, next) {
    res.reply = function(info) {
        this.render("index", {info: info});
    };
    next();
};
