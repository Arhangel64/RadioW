var Winston = require("winston");
var config = require("../config");
var ENV = config.get('build');

function getLogger(module) {
    var path = module.filename.split('/').slice(-2).join('/');
    
    return new Winston.Logger({
	transports: [
	    new Winston.transports.Console({
		colorize: true,
		level: ENV == 'debug' ? 'debug' : 'error'
	    })
	]
    });
}

module.exports = getLogger;
