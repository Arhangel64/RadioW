"use strict";
var fs = require("fs");
var DepResolver = require("./depresolver");
var pathCheck = require("./pathCheck");

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
    if (err) {
        throw err;
    }
    console._stdout.write(String.fromCharCode(27) + "[1;32m");
    console._stdout.write("polymorph: ");
    console._stdout.write(String.fromCharCode(27) + "[0m");
    console._stdout.write("parsing " + moduleName + " for " + env);
    console._stdout.write("\n");
    
    var file = buffer.toString();
    var output = "";
    if (!isNode) {
        file = file.replace(/module.exports[\s]*=/g, "return");
        file = file.replace(/\"use\sstrict\";/g, "");
        var lines = file.split("\n");
        
        output =    "\"use strict\";\n" +
                    "(function(global) {\n" +
                    "  var moduleName = \""+moduleName+"\"\n";
                    
        var add = dr.resolve(lines);
        
        for (var i = 0; i < add.dependencies.length; ++i) {
            output += "  " + add.dependencies[i] + "\n";
        }
        
        output += "\n";
        
        for (var i = 0; i < add.header.length; ++i) {
            output += "  " + add.header[i] + "\n";
        }
        
        for (var i = 0; i < lines.length; ++i) {
            var line = lines[i];
            output += "    " + line + "\n";
        }
        
        output += "  " + add.bottom + "\n";
        output += "})(window);";   
    } else {
        output = file;
    }
    var p = target.split("/");
    p[1] = "/" + p[1]
    pathCheck(p.slice(1, -1), function(err) {
        if (err) throw err;
        fs.writeFile(target, output, function(err) {
            if (err) throw err;
                     process.exit(0);
        });
    })
})

