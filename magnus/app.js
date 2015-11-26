var express = require("express");
var morgan = require("morgan");
var path = require('path');

var app = express()

app.use(morgan('dev'));

app.use(express.static(path.join(__dirname, 'public')));

var server = app.listen(3000, "127.0.0.1", function () {

  var host = server.address().address
  var port = server.address().port

  console.log('Example app listening at http://%s:%s', host, port)

})