"use strict";
var fs = require("fs");
var DepResolver = require("./depresolver");

var path = process.argv[2];
var target = process.argv[3];
var moduleName = process.argv[4];
var env = process.argv[5];
var libDir = process.argv[6];

var isNode = true;
if (env === "browser") {
    isNode = false;
}

var dr = new DepResolver({
    target: target,
    libDir: libDir
});

fs.readFile(path, function(err, buffer) {
    if (err) throw err;
    var file = buffer.toString();
    var output = "";
    if (!isNode) {
        file = file.replace(/module.exports[\s]*=/g, "return");
        var lines = file.split("\n");
        
        output =    "(function (global){ \n" +
                    "  var moduleName = \""+moduleName+"\"\n";
                    
        var add = dr.resolve(lines);
        for (var i = 0; i < add.header.length; ++i) {
            output += "  " + add.header[i] + "\n";
        }
        
        for (var i = 0; i < lines.length; ++i) {
            var line = lines[i];
            output += "    " + line + "\n";
        }
        
        output += "  " + add.bottom;
        output += "})(window);";   
    } else {
        output = file;
    }
    fs.writeFile(target, output, function(err) {
        if (err) throw err;
        process.exit(0);
    });
})

