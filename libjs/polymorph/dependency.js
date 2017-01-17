"use strict";

var Dependency = function(options) {
    if (!Dependency.configured) {
        throw new Error("Unable to create a dependency without static variables configuration, use Dependency.configure first");
    }
    this._string = options.string;
    this._text = options.text;
    
    this._parse();
}

Dependency.configured = false;
Dependency.configure = function(libDir, target) {
    this.libDir = libDir;
    this.target = target;
    var tPath = target.replace(libDir, "lib");
    tPath = tPath.replace(".js", "");
    tPath = tPath.split("/");
    tPath.pop();
    this.path = tPath;
    this.configured = true;
};

Dependency.prototype.log = function() {
    console.log("---------------------------------");
    console.log("Found string: " + this._string);
    console.log("Captured dependency: " + this._text);
    console.log("Output dependency: " + this._name);
    console.log("Lib is: " + Dependency.libDir);
    console.log("Target is: " + Dependency.target);
    console.log("---------------------------------");
}

Dependency.prototype._parse = function() {
    var fl = this._text[0];
    var dArr = this._text.split("/");
    
    if (fl !== ".") {
        this._name = "lib/" + this._text + "/index";
    } else {
        var summ = Dependency.path.slice();
        this._name = "";
        
        for (var i = 0; i < dArr.length; ++i) {
            var hop = dArr[i];
            switch (hop) {
                case ".":
                case "":
                    break;
                case "..":
                    summ.pop();
                    break
                default:
                    summ.push(hop);
                    break;
            }
        }
        
        for (var j = 0; j < summ.length; ++j) {
            if (j !== 0) {
                this._name += "/"
            }
            this._name += summ[j];
        }
    }
}

Dependency.prototype.modify = function(line) {
    return line.replace(this._text, this._name);
}
Dependency.prototype.toString = function() {
    return this._name;
}

module.exports = Dependency;
