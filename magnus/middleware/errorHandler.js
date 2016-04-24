var defaultHandler = require("errorhandler");

var config = require("../config");
var log = require("../lib/log");
var HttpError = require("../lib/httpError");

function errorHandler(err, req, res, next) {
	if (typeof err == "number") {
		err = new HttpError(err);
	}
	
	if (err instanceof HttpError) {
		sendHttpError(err, res, req);
	} else {
		if (config.get("build") === "debug") {
			var handler = defaultHandler();
			handler(err, req, res, next);
		} else {
			log.error(err);
			err = new HttpError(500);
			sendHttpError(err, res, req);
		}
	}
}

function sendHttpError(error, res, req) {
	res.status(error.status);
	//if (req.headers['x-requested-with'] == 'XMLHttpRequest') {
	//	res.json(error);
	//} else {
	//	res.reply(error);
	//}
	res.reply(error);
}

module.exports = errorHandler;
