// Generated by Melange

import ConfirmModalCss from "./ConfirmModal.css";
import * as Components__Button from "../Button/Button.js";
import * as Components__Headline from "../Headline/Headline.js";
import * as Components__Modal from "../Modal/Modal.js";
import * as Css from "../../../css/Css.js";
import * as Curry from "melange.js/curry.js";
import * as Images__Icons from "../../../images/Icons.js";
import * as Stdlib__Option from "melange/option.js";
import * as JsxRuntime from "react/jsx-runtime";

const css = ConfirmModalCss;

function ConfirmModal(Props) {
  let title = Props.title;
  let text = Props.text;
  let classNameOpt = Props.className;
  let variantOpt = Props.variant;
  let isOpenOpt = Props.isOpen;
  let onCancel = Props.onCancel;
  let onConfirm = Props.onConfirm;
  let loadingOpt = Props.loading;
  const className = classNameOpt !== undefined ? classNameOpt : "";
  const variant = variantOpt !== undefined ? variantOpt : "Danger";
  const isOpen = isOpenOpt !== undefined ? isOpenOpt : false;
  const loading = loadingOpt !== undefined ? loadingOpt : false;
  const onConfirm$1 = onConfirm[1];
  let tmp;
  if (onCancel !== undefined) {
    const fn = onCancel[1];
    tmp = JsxRuntime.jsx("li", {
      children: JsxRuntime.jsx(Components__Button.make, {
        variant: "Secondary",
        action: {
          NAME: "Fn",
          VAL: (function (param) {
            Curry._1(fn, undefined);
          })
        },
        children: onCancel[0]
      }),
      className: "ConfirmModal-action"
    });
  } else {
    tmp = null;
  }
  return JsxRuntime.jsxs(Components__Modal.make, {
    className: Css.merge([
      "ConfirmModal",
      variant === "Warning" ? "ConfirmModal--warning" : (
          variant === "Info" ? "ConfirmModal--info" : "ConfirmModal--danger"
        ),
      className
    ]),
    isOpen: isOpen,
    onClose: Stdlib__Option.map((function (prim) {
      return prim[1];
    }), onCancel),
    children: [
      JsxRuntime.jsxs("div", {
        children: [
          variant === "Warning" ? JsxRuntime.jsx(Images__Icons.ExclamationTriangle.make, {
              className: "ConfirmModal-icon"
            }) : (
              variant === "Info" ? JsxRuntime.jsx(Images__Icons.Info.make, {
                  className: "ConfirmModal-icon"
                }) : JsxRuntime.jsx(Images__Icons.ExclamationCircle.make, {
                  className: "ConfirmModal-icon"
                })
            ),
          JsxRuntime.jsxs("div", {
            children: [
              JsxRuntime.jsx(Components__Headline.Secondary.make, {
                className: "ConfirmModal-title",
                tag: "h2",
                children: title
              }),
              JsxRuntime.jsx("p", {
                children: text,
                className: "ConfirmModal-text"
              })
            ],
            className: "ConfirmModal-content"
          })
        ],
        className: "ConfirmModal-main"
      }),
      JsxRuntime.jsxs("ul", {
        children: [
          tmp,
          JsxRuntime.jsx("li", {
            children: JsxRuntime.jsx(Components__Button.make, {
              variant: "Primary",
              danger: variant === "Danger",
              disabled: loading,
              action: {
                NAME: "Fn",
                VAL: (function (param) {
                  Curry._1(onConfirm$1, undefined);
                })
              },
              children: onConfirm[0]
            }),
            className: "ConfirmModal-action"
          })
        ],
        className: "ConfirmModal-actions"
      })
    ]
  });
}

const make = ConfirmModal;

export {
  css,
  make,
}
/* css Not a pure module */
