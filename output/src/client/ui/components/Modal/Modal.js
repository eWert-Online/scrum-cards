// Generated by Melange

import ModalCss from "./Modal.css";
import * as Css from "../../../css/Css.js";
import * as Curry from "melange.js/curry.js";
import * as Hooks__Hooks_ClickAway from "../../../hooks/Hooks_ClickAway.js";
import * as React from "react";
import * as ReactDom from "react-dom";
import * as JsxRuntime from "react/jsx-runtime";

const css = ModalCss;

function Modal(Props) {
  let classNameOpt = Props.className;
  let isOpen = Props.isOpen;
  let onClose = Props.onClose;
  let children = Props.children;
  const className = classNameOpt !== undefined ? classNameOpt : "";
  const modalRef = React.useRef(null);
  Hooks__Hooks_ClickAway.useClickAway({
    hd: modalRef,
    tl: /* [] */ 0
  }, (function (param) {
    if (onClose !== undefined) {
      return Curry._1(onClose, undefined);
    }
    
  }));
  const portal = React.useMemo((function () {
    return document.createElement("div");
  }), []);
  React.useEffect((function () {
    if (!isOpen) {
      return;
    }
    const body = document.body;
    body.appendChild(portal);
    return (function (param) {
      body.removeChild(portal);
    });
  }), [
    isOpen,
    portal
  ]);
  if (isOpen) {
    return ReactDom.createPortal(JsxRuntime.jsx("div", {
      children: JsxRuntime.jsxs("div", {
        ref: modalRef,
        children: [
          onClose !== undefined ? JsxRuntime.jsx("button", {
              children: "close window",
              className: "Modal-close",
              type: "button",
              onClick: (function (param) {
                Curry._1(onClose, undefined);
              })
            }) : null,
          JsxRuntime.jsx("div", {
            children: children,
            className: Css.merge_list({
              hd: "Modal-content",
              tl: {
                hd: className,
                tl: /* [] */ 0
              }
            })
          })
        ],
        className: "Modal"
      }),
      className: "Modal-wrapper is-active"
    }), portal);
  } else {
    return null;
  }
}

const make = Modal;

export {
  css,
  make,
}
/* css Not a pure module */
