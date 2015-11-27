"use strict";
var HttpError = require("../lib/httpError");

module.exports = function(req, res, next) {
    return next(new HttpError(404, 'Page not found!'));
};