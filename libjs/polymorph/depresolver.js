"use strict";
var Dependency = require("./dependency");

var DepResolver = function(options) {
    this._libDir = options.libDir;
    this._target = options.target;
    
    Dependency.libDir = this._libDir;
    Dependency.target = this._target;
    
    this._reg = /require\(\"(.*)\"\)[^;]*;/g;
}

DepResolver.prototype.resolve = function(lines) {
    var regres;
    var header = [];
    header.push("var defineArray = [];\n");
    
    for (var i = 0; i < lines.length; ++i) {
        var line = lines[i];
        while((regres = this._reg.exec(line)) !== null) {
            var dep = new Dependency({
                string: regres[0],
                text: regres[1]
            });
            lines[i] = dep.modify(line);
            header.push("defineArray.push(\"" + dep.toString() + "\");");
        }
    }
    header.push("define(moduleName, defineArray, function() {");
    
    return {
        header: header,
        bottom: "});"
    }
};

module.exports = DepResolver;