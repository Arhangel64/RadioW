"use strict";

global.W = {
    extend: function () {
        var lTarget = arguments[0] || {};
        var lIndex = 1;
        var lLength = arguments.length;
        var lDeep = false;
        var lOptions, lName, lSrc, lCopy, lCopyIsArray, lClone;

        // Handle a deep copy situation
        if (typeof lTarget === "boolean") {
            lDeep = lTarget;
            lTarget = arguments[1] || {};
            // skip the boolean and the target
            lIndex = 2;
        }

        // Handle case when target is a string or something (possible in deep
        // copy)
        if (typeof lTarget !== "object" && typeof lTarget != "function") {
            lTarget = {};
        }

        if (lLength === lIndex) {
            lTarget = this;
            --lIndex;
        }

        for (; lIndex < lLength; lIndex++) {
            // Only deal with non-null/undefined values
            if ((lOptions = arguments[lIndex]) != undefined) {
                // Extend the base object
                for (lName in lOptions) {
                    lSrc = lTarget[lName];
                    lCopy = lOptions[lName];

                    // Prevent never-ending loop
                    if (lTarget === lCopy) {
                        continue;
                    }

                    // Recurse if we're merging plain objects or arrays
                    if (lDeep && lCopy && (Object.isObject(lCopy) || (lCopyIsArray = Array.isArray(lCopy)))) {
                        if (lCopyIsArray) {
                            lCopyIsArray = false;
                            lClone = lSrc && Array.isArray(lSrc) ? lSrc : [];

                        } else {
                            lClone = lSrc && Object.isObject(lSrc) ? lSrc : {};
                        }

                        // Never move original objects, clone them
                        lTarget[lName] = Object.extend(lDeep, lClone, lCopy);

                        // Don't bring in undefined values
                    } else {
                        if (lCopy !== undefined) {
                            lTarget[lName] = lCopy;
                        }
                    }
                }
            }
        }

        // Return the modified object
        return lTarget;
    }
};