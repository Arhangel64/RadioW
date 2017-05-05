"use strict";
(function view_label_js() {
    var moduleName = "views/label";
    
    var defineArray = [];
    defineArray.push("views/view");
    
    define(moduleName, defineArray, function view_label_module() {
        var View = require("views/view");
        
        var Label = View.inherit({
            "className": "Label",
            "constructor": function(controller, options) {
                var base = {};
                W.extend(base, options)
                View.fn.constructor.call(this, controller, base);
                
                this._timeout = undefined;
                this._recalculationScheduled = false;
            },
            "destructor": function() {
                if (this._recalculationScheduled) {
                    clearTimeout(this._timeout);
                }
                
                View.fn.destructor.call(this);
            },
            "_onAddProperty": function(key, propertyName) {
                View.fn._onAddProperty.call(this, key, propertyName);
                
                if (sizeChangingProperties.indexOf(propertyName) !== -1) {
                    this._scheduleRecalculation();
                }
            },
            "_onData": function(data) {
                if (this._e.innerText !== data) {
                    this._e.innerText = data;
                    this._scheduleRecalculation();
                }
            },
            "_recalculateSize": function() {
                var fs = parseFloat(this._e.style.fontSize) || 16;
                var fontFamily = this._e.style.fontFamily || "Liberation";
                
                var h = fs + 2;
                var w = Label.calculateSingleString(fontFamily, fs, this._e.innerText || "");
                this.setConstSize(w, h);
                this._recalculationScheduled = false;
            },
            "_scheduleRecalculation": function() {
                if (!this._recalculationScheduled) {
                    this._timeout = setTimeout(this._recalculateSize.bind(this), 10);
                    this._recalculationScheduled = true;
                }
            }
        });
        
        var sizeChangingProperties = ["fontSize", "fontFamily", "whiteSpace"];
        
        Label.calculateSingleString = function(family, size, string) {
            var fontStorage = Label[family] || Label["Liberation"];
            
            var width = 0;
            
            for (var i = 0; i < string.length; ++i) {
                var letter = string[i];
                var l = fontStorage[letter] || 10;
                width += (size * (l) / 18);
            }
            
            return Math.ceil(width);
        };
        
        Label.Liberation = {  //Sans, measured on 18px size, not sure about the space, and not sure, how to calculate interval between letters
            " ": 5, //?
            "!": 5,
            "\"": 7,
            "#": 10,
            "$": 10,
            "%": 15,
            "&": 12,
            "'": 4,
            "(": 6,
            ")": 6,
            "*": 8,
            "+": 10,
            ",": 5,
            "-": 7,
            ".": 5,
            "/": 6,
            "0": 10,
            "1": 9,
            "2": 10,
            "3": 10,
            "4": 10,
            "5": 10,
            "6": 10,
            "7": 10,
            "8": 10,
            "9": 10,
            ":": 5,
            ";": 5,
            "<": 11,
            "=": 10,
            ">": 10,
            "?": 10,
            "@": 18,
            "A": 11,
            "B": 12,
            "C": 12,
            "D": 13,
            "E": 12,
            "F": 10,
            "G": 14,
            "H": 12,
            "I": 6,
            "J": 9,
            "K": 12,
            "L": 9,
            "M": 15,
            "N": 12,
            "O": 14,
            "P": 11,
            "Q": 14,
            "R": 12,
            "S": 12,
            "T": 11,
            "U": 12,
            "V": 12,
            "W": 16,
            "X": 12,
            "Y": 11,
            "Z": 11,
            "[": 6,
            "\\": 5,
            "]": 6,
            "^": 8,
            "_": 10,
            "`": 6,
            "a": 10,
            "b": 10,
            "c": 9,
            "d": 10,
            "e": 10,
            "f": 5,
            "g": 10,
            "h": 10,
            "i": 5,
            "j": 4,
            "k": 9,
            "l": 5,
            "m": 14,
            "n": 10,
            "o": 10,
            "p": 10,
            "q": 10,
            "r": 6,
            "s": 9,
            "t": 5,
            "u": 10,
            "v": 9,
            "w": 13,
            "x": 9,
            "y": 9,
            "z": 9,
            "{": 6,
            "|": 5,
            "}": 7,
            "~": 10,
            "¡": 7,
            "¢": 10,
            "£": 10,
            "¤": 9,
            "¥": 10,
            "¦": 5,
            "§": 10,
            "¨": 7,
            "©": 12,
            "ª": 7,
            "«": 10,
            "¬": 11,
            "®": 12,
            "¯": 10,
            "°": 8,
            "±": 9,
            "²": 7,
            "³": 6,
            "´": 6,
            "µ": 11,
            "¶": 9,
            "·": 7,
            "¸": 6,
            "¹": 6,
            "º": 7,
            "»": 10,
            "¼": 14,
            "½": 15,
            "¾": 14,
            "¿": 11,
            "А": 11,
            "Б": 11,
            "В": 12,
            "Г": 9,
            "Д": 12,
            "Е": 12,
            "Ж": 16,
            "З": 10,
            "И": 13,
            "Й": 12,
            "К": 11,
            "Л": 11,
            "М": 14,
            "Н": 13,
            "О": 13,
            "П": 13,
            "Р": 12,
            "С": 12,
            "Т": 11,
            "У": 11,
            "Ф": 13,
            "Х": 12,
            "Ц": 13,
            "Ч": 11,
            "Ш": 16,
            "Щ": 16,
            "Ъ": 14,
            "Ы": 15,
            "Ь": 11,
            "Э": 13,
            "Ю": 17,
            "Я": 13,
            "а": 9,
            "б": 11,
            "в": 9,
            "г": 7,
            "д": 10,
            "е": 10,
            "ж": 12,
            "з": 8,
            "и": 10,
            "й": 10,
            "к": 8,
            "л": 10,
            "м": 12,
            "н": 10,
            "о": 10,
            "п": 10,
            "р": 10,
            "с": 9,
            "т": 8,
            "у": 9,
            "ф": 14,
            "х": 9,
            "ц": 10,
            "ч": 10,
            "ш": 13,
            "щ": 15,
            "ъ": 11,
            "ы": 12,
            "ь": 9,
            "э": 10,
            "ю": 13,
            "я": 9
        };
        
        return Label;
    });
})();
