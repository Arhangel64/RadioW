"use strict";
var http = require("http");

class HttpError extends Error
{
    constructor(status, message) {
        super(status, message);
        Error.captureStackTrace(this, HttpError);

        this.status = status;
        this.message = message || http.STATUS_CODES[status] || "Error";
    }
}

module.exports = HttpError;
