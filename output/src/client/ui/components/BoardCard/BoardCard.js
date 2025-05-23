// Generated by Melange

import BoardCardCss from "./BoardCard.css";
import * as Caml_option from "melange.js/caml_option.js";
import * as Css from "../../../css/Css.js";
import * as Js__Js_dict from "melange.js/js_dict.js";
import * as Js__Js_math from "melange.js/js_math.js";
import * as React from "react";
import * as JsxRuntime from "react/jsx-runtime";

const css = BoardCardCss;

function BoardCard(Props) {
  let label = Props.label;
  let revealOpt = Props.reveal;
  let valueOpt = Props.value;
  let classNameOpt = Props.className;
  const reveal = revealOpt !== undefined ? revealOpt : false;
  const value = valueOpt !== undefined ? Caml_option.valFromOption(valueOpt) : undefined;
  const className = classNameOpt !== undefined ? classNameOpt : "";
  const cssVariables = React.useMemo((function () {
    const delay = Js__Js_math.random_int(50, 800);
    const translateY = Js__Js_math.random_int(-12, 8);
    const rotate = Js__Js_math.random_int(-8, 8);
    return Js__Js_dict.fromList({
      hd: [
        "--BoardCard-inline-transitionDelay",
        String(delay) + "ms"
      ],
      tl: {
        hd: [
          "--BoardCard-inline-translateY",
          String(translateY) + "px"
        ],
        tl: {
          hd: [
            "--BoardCard-inline-rotate",
            String(rotate) + "deg"
          ],
          tl: /* [] */ 0
        }
      }
    });
  }), []);
  return JsxRuntime.jsxs("div", {
    children: [
      JsxRuntime.jsx("div", {
        children: value !== undefined ? JsxRuntime.jsxs("div", {
            children: [
              JsxRuntime.jsx("div", {
                className: "BoardCard-face BoardCard-cardBack"
              }),
              JsxRuntime.jsx("div", {
                children: reveal ? value : null,
                className: "BoardCard-face BoardCard-cardFront"
              })
            ],
            className: "BoardCard-card"
          }) : null,
        className: "BoardCard-cardWrapper",
        style: cssVariables
      }),
      JsxRuntime.jsx("p", {
        children: label,
        className: "BoardCard-label"
      })
    ],
    className: Css.merge([
      "BoardCard",
      reveal ? "is-flipped" : "",
      className
    ])
  });
}

const make = BoardCard;

export {
  css,
  make,
}
/* css Not a pure module */
