"use strict";
(function mainLayout_js() {
    var moduleName = "views/mainLayout";
    
    var defineArray = [];
    defineArray.push("views/gridLayout");
    defineArray.push("views/label");
    defineArray.push("views/navigationPanel");
    defineArray.push("views/layout");
    defineArray.push("lib/wController/localModel");
    
    define(moduleName, defineArray, function mainLayout_module() {
        var GridLayout = require("views/gridLayout");
        var ViewLabel = require("views/label");
        var ViewNavigationPanel = require("views/navigationPanel");
        var Layout = require("views/layout");
        var LocalModel = require("lib/wController/localModel");
        
        var MainLayout = GridLayout.inherit({
            "className": "MainLayout",
            "_onNewController": function(controller) {
                GridLayout.fn._onNewController.call(this, controller);
                
                var view;
                
                switch (controller.name) {
                    case "version":
                        var lm = new LocalModel({
                            backgroundColor: "secondaryColor"
                        });
                        var lay = new Layout(lm, {maxHeight: 15})
                        view = new ViewLabel(controller);
                        lay.append(view, Layout.Aligment.RightCenter);
                        this.append(lay, 2, 0, 1, 3);
                        break;
                    case "navigationPanel":
                        view = new ViewNavigationPanel(controller);
                        this.append(view, 0, 0, 1, 3);
                        break;
                    case "themes":
                        break;
                    default:
                        //this.trigger("serviceMessage", "Unsupported view: " + name + " (" + type + ")", 1);
                        break;
                }
            }
        });
        
        return MainLayout;
    });
})();
