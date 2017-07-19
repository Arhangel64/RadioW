"use strict";
(function draggable_js() {
    var moduleName = "views/helpers/draggable";

    var defineArray = [];
    defineArray.push("lib/utils/subscribable");

    define(moduleName, defineArray, function draggable_module() {
        var Subscribable = require("lib/utils/subscribable");

        var Draggable = Subscribable.inherit({
            "className": "Draggable",
            "constructor": function Draggable (view, options) {
                var base = {
                    snapDistance: 0
                };
                W.extend(base, options);
                Subscribable.fn.constructor.call(this, base);

                this._o = base;
                this._e = view._e;
                this._v = view;
                this._v.addClass("draggable");

                this._initProxy();
                this._dragging = false;
                this._toFlush = false;

                this._x = 0;
                this._y = 0;

                this._e.addEventListener("mousedown", this._proxy.onMouseDown, false);
                this._e.addEventListener("touchstart", this._touch, false);
                this._e.addEventListener("touchmove", this._touch, false);
                this._e.addEventListener("touchend", this._touch, false);
            },
            "destructor": function () {
                if (this._dragging) {
                    window.removeEventListener("mouseup", this._proxy.onMouseUp);
                    window.removeEventListener("mousemove", this._proxy.onMouseMove);
                }
                
                if (!this._v.destroying) {
                    this._v.removeClass("draggable");
                }

                this._e.removeEventListener("mousedown", this._proxy.onMouseDown);
                this._e.removeEventListener("touchstart", this._touch);
                this._e.removeEventListener("touchmove", this._touch);
                this._e.removeEventListener("touchend", this._touch);

                Subscribable.fn.destructor.call(this);
            },
            "_initProxy": function () {
                this._proxy = {
                    "onMouseDown": this._onMouseDown.bind(this),
                    "onMouseUp": this._onMouseUp.bind(this),
                    "onMouseMove": this._onMouseMove.bind(this)
                }
            },
            "_onMouseDown": function (e) {
                if (e.which === 1) {
                    window.addEventListener("mouseup", this._proxy.onMouseUp);
                    window.addEventListener("mousemove", this._proxy.onMouseMove);
                    lorgar._body.addClass("non-selectable");
                    this._x = e.pageX;
                    this._y = e.pageY;
                }
            },
            "_onMouseMove": function (e) {
                var x = e.pageX - this._x;
                var y = e.pageY - this._y;

                if (this._dragging) {
                    this._v.trigger("drag", {
                        x: x,
                        y: y
                    });
                } else {
                    var absX = Math.abs(x);
                    var absY = Math.abs(y);

                    if (absX > this._o.snapDistance || absY > this._o.snapDistance) {
                        this._dragging = true;
                        lorgar._body.addClass("dragging");
                        this._v.trigger("dragStart", {
                            x: x,
                            y: y
                        });
                    }
                }
                return false;
            },
            "_onMouseUp": function (e) {
                window.removeEventListener("mouseup", this._proxy.onMouseUp);
                window.removeEventListener("mousemove", this._proxy.onMouseMove);

                this._x = 0;
                this._y = 0;
                lorgar._body.removeClass("non-selectable");
                if (this._dragging) {
                    e.preventDefault();
                    this._dragging = false;
                    lorgar._body.removeClass("dragging");
                    this._v.trigger("dragEnd");
                    return false;
                }
            },
            "_touch": function (e) {
                e.preventDefault();
                if (e.touches.length > 1 || (e.type == "touchend" && e.touches.length > 0))
                    return;

                var type = null;
                var touch = null;
                var src = e.currentTarget;
                switch (e.type) {
                    case "touchstart":
                        type = "mousedown";
                        touch = e.changedTouches[0];

                        break;
                    case "touchmove":
                        type = "mousemove";
                        touch = e.changedTouches[0];
                        src = window;
                        break;
                    case "touchend":
                        type = "mouseup";
                        touch = e.changedTouches[0];
                        src = window;
                        break;
                }

                var event = new MouseEvent(type, {
                    button: 0,
                    screenX: touch.screenX,
                    screenY: touch.screenY,
                    clientX: touch.clientX,
                    clientY: touch.clientY
                });
                src.dispatchEvent(event);
            }
        });

        return Draggable;
    });
})();
