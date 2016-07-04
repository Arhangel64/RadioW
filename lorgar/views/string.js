"use strict";
(function view_string_js() {
    var moduleName = "views/string";
    
    var defineArray = [];
    defineArray.push("views/view");
    
    define(moduleName, defineArray, function view_string_module() {
        var View = require("views/view");
        
        var ViewString = View.inherit({
            "className": "String",
            "constructor": function(options) {
                var base = {
                };
                W.extend(base, options)
                View.fn.constructor.call(this, base);
            },
            "_applyProp": function(pair) {
                var counter = 0;
                var value = this._currentTheme[pair.k];
                if (value) {
                    if (this._e.style[pair.p] !== value) {
                        this._e.style[pair.p] = value;
                        if (pair.p === "fontSize" || pair.p === "fontFamily") {
                            return true;
                        }
                    }
                }
                return false;
            },
            "applyTheme": function(theme) {
                this._currentTheme = theme;
                var counter = 0;
                
                for (var i = 0; i < this._styleProperties.length; ++i) {
                    if (this._applyProp(this._styleProperties[i])) {
                        ++counter;
                    }
                }
                if (counter > 0) {
                    this.trigger("measuresChanged");
                }
            },
            "data": function(data) {
                if (this._e.innerText !== data) {
                    this._e.innerText = data;
                    this.trigger("measuresChanged");
                }
                
            },
            "getFontSize": function() {
                var fs = this._e.style.fontSize;
                if (!fs) {
                    return 16;
                } else {
                    return parseInt(fs);
                }
            }
        });
        
        ViewString.calculateSingleString = function(family, size, string) {
            var fontStorage = ViewString[family] || ViewString["Liberation"];
            
            var width = 0;
            
            for (var i = 0; i < string.length; ++i) {
                var letter = string[i];
                var l = fontStorage[letter] || 10;
                width += (size * (l) / 18);
            }
            
            return Math.ceil(width);
        };
        
        ViewString.Liberation = {  //Sans, measured on 18px size, not sure about the space, and not sure, how to calculate interval between letters
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
        
        return ViewString;
    });
})();