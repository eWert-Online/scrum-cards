// Generated by Melange

import HeaderCss from "./Header.css";
import * as Css from "../../../css/Css.js";
import * as JsxRuntime from "react/jsx-runtime";

const css = HeaderCss;

function Header(Props) {
  let classNameOpt = Props.className;
  let children = Props.children;
  const className = classNameOpt !== undefined ? classNameOpt : "";
  return JsxRuntime.jsx("header", {
    children: children,
    className: Css.merge_list({
      hd: "Header",
      tl: {
        hd: className,
        tl: /* [] */ 0
      }
    })
  });
}

const make = Header;

export {
  css,
  make,
}
/* css Not a pure module */
