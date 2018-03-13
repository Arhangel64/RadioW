"use strict";
const fs = require("fs");

function pathCheck(/*Array*/path, cb) {
    fs.access(path[0], function(err) {
        if (err) {
            if (err.code === 'ENOENT') {
                fs.mkdir(path[0], function() {
                    if (path.length === 1) {
                        cb();
                    } else {
                        let nPath = path.slice();
                        let out = nPath.splice(1, 1);
                        nPath[0] += "/" + out[0];
                        pathCheck(nPath, cb);
                    }
                })
            } else {
                cb(err);
            }
        } else {
            if (path.length === 1) {
                cb();
            } else {
                let nPath = path.slice();
                let out = nPath.splice(1, 1);
                nPath[0] += "/" + out[0];
                pathCheck(nPath, cb);
            }
        }
    })
}

module.exports = pathCheck;
