var Test = require("./test/test");
new Test();

var express = require("express");
var morgan = require("morgan");
var favicon = require("serve-favicon");
var Magnus = require("./core/magnus");

var config = require("./config");
var log = require("./lib/log")(module);

var app = express();

app.set('view engine', 'jade');
app.set('views', __dirname + '/views');

app.use(favicon(__dirname + "/public/favicon.ico"));
app.use(morgan('dev'));

app.use(require("./middleware/reply"));

app.use(express.static('public'));

app.use(require("./middleware/notFound"));
app.use(require("./middleware/errorHandler"));

var server = app.listen(config.get("port"), "127.0.0.1", function () {

  var port = server.address().port;

 log.info("Magnus is listening on port " + port);

});
var magnus = new Magnus();

