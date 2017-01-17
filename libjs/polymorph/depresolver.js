"use strict";
var Dependency = require("./dependency");

var DepResolver = function(options) {
    Dependency.configure(options.libDir, options.target)
    
    this._reg1 = /(?:require\s*\((?:\"(.*)\"|\'(.*)\')\)[^;]*;)(?!\s*\/\/not\sa\sd)/g;
    this._reg2 = /(?:\"(.+)\"|\'(.+)\'),{0,1}(?=\s*\/\/resolve as d)/g;
}

DepResolver.prototype.resolve = function(lines) {
    var regres;
    var dep;
    var header = [];
    var dependencies = [];
    dependencies.push("var defineArray = [];\n");
    
    for (var i = 0; i < lines.length; ++i) {
        var line = lines[i];
        var found = false;
        while((regres = this._reg1.exec(line)) !== null) {
            dep = new Dependency({
                string: regres[0],
                text: regres[1]
            });
            lines[i] = dep.modify(line);
            dependencies.push("defineArray.push(\"" + dep.toString() + "\");");
            found = true;
        }
        if (!found) {
            while((regres = this._reg2.exec(line)) !== null) {
                dep = new Dependency({
                    string: regres[0],
                    text: regres[1]
                });
                
                lines[i] = dep.modify(line);
            }
        }
        
    }
    header.push("define(moduleName, defineArray, function() {");
    
    return {
        header: header,
        dependencies: dependencies,
        bottom: "});"
    }
};

module.exports = DepResolver;
