"use strict";
module.exports = function(req, res, next) {
    if (global.magnus.hasPage(req.path)) {
        res.reply("Building " + req.path + "...");
    } else {
        next();
    }
};

