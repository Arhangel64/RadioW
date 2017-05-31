var express = require("express");
var morgan = require("morgan");
var favicon = require("serve-favicon");
var Magnus = require("./core/magnus");

require("./lib/utils/globalMethods");

var config = require("./config");
var log = require("./lib/log")(module);

if (config.get("testing")) {
    var Test = require("./test/test");
    var test = new Test()
    test.run();
    test.destructor();
}

var app = express();

app.set('view engine', 'jade');
app.set('views', __dirname + '/views');

app.use(favicon(__dirname + "/public/favicon.ico"));

var httpLog = config.get("httpLog");
if (httpLog) {
    app.use(morgan('dev'));
}

app.use(require("./middleware/reply"));

app.use(express.static(__dirname + '/public'));

app.use(require("./middleware/pageInMagnus"));
app.use(require("./middleware/notFound"));
app.use(require("./middleware/errorHandler"));

var server = app.listen(config.get("webServerPort"), "127.0.0.1", function () {

  var port = server.address().port;

 log.info("Webserver is listening on port " + port);

});
var magnus = global.magnus = new Magnus(config);

