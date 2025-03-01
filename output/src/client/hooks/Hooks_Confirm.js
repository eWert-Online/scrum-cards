// Generated by Melange

import * as Context_types from "../context/types/Context_types.js";
import * as Curry from "melange.js/curry.js";
import * as React from "react";

function useConfirm(param) {
  const context = React.useContext(Context_types.Confirm.context);
  return React.useCallback((function (title, text, variant, onConfirm, onCancel, param) {
    Curry._1(context, {
      variant: variant,
      title: title,
      text: text,
      onConfirm: onConfirm,
      onCancel: onCancel
    });
  }), [context]);
}

export {
  useConfirm,
}
/* Context_types Not a pure module */
