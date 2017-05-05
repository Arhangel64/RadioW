"use strict";
(function mainLayout_js() {
    var moduleName = "views/mainLayout";
    
    var defineArray = [];
    defineArray.push("views/gridLayout");
    defineArray.push("views/label");
    defineArray.push("views/navigationPanel");
    
    define(moduleName, defineArray, function mainLayout_module() {
        var GridLayout = require("views/gridLayout");
        var ViewLabel = require("views/label");
        var ViewNavigationPanel = require("views/navigationPanel");
        
        var MainLayout = GridLayout.inherit({
            "className": "MainLayout",
            "_onNewController": function(controller) {
                GridLayout.fn._onNewController.call(this, controller);
                
                var view;
                
                switch (controller.name) {
                    case "version":
                        view = new ViewLabel(controller, {maxHeight: 15});
                        this.append(view, 2, 0, 1, 3);
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
