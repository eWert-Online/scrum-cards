// Generated by Melange

import SwitchCss from "./Switch.css";
import * as Css from "../../../css/Css.js";
import * as Curry from "melange.js/curry.js";
import * as JsxRuntime from "react/jsx-runtime";

const css = SwitchCss;

function Switch(Props) {
  let classNameOpt = Props.className;
  let checked = Props.checked;
  let onChange = Props.onChange;
  let descriptionOpt = Props.description;
  let label = Props.label;
  let name = Props.name;
  const className = classNameOpt !== undefined ? classNameOpt : "";
  const description = descriptionOpt !== undefined ? descriptionOpt : "";
  const handleChange = function (evt) {
    Curry._1(onChange, evt.target.checked);
  };
  const has_description = description !== "";
  return JsxRuntime.jsxs("label", {
    children: [
      JsxRuntime.jsx("input", {
        className: "Switch-input",
        id: name,
        checked: checked,
        name: name,
        type: "checkbox",
        onChange: handleChange
      }),
      JsxRuntime.jsx("span", {
        className: "Switch-switch"
      }),
      JsxRuntime.jsxs("div", {
        children: [
          JsxRuntime.jsx("div", {
            children: label,
            className: "Switch-title"
          }),
          has_description ? JsxRuntime.jsx("aside", {
              children: description,
              className: "Switch-description"
            }) : null
        ],
        className: "Switch-info"
      })
    ],
    className: Css.merge([
      "Switch",
      has_description ? "Switch--described" : "",
      className
    ]),
    htmlFor: name
  });
}

const make = Switch;

export {
  css,
  make,
}
/* css Not a pure module */
