// Generated by Melange

import * as Caml_option from "melange.js/caml_option.js";
import * as Components__ConfirmModal from "../ui/components/ConfirmModal/ConfirmModal.js";
import * as Context_types from "./types/Context_types.js";
import * as Curry from "melange.js/curry.js";
import * as React from "react";
import * as JsxRuntime from "react/jsx-runtime";

function ConfirmContext$Provider(Props) {
  let children = Props.children;
  const match = React.useState(function () {
    
  });
  const setConfirmDialog = match[1];
  const confirmDialog = match[0];
  const match$1 = React.useState(function () {
    return false;
  });
  const setIsConfirming = match$1[1];
  const isConfirming = match$1[0];
  const match$2 = React.useState(function () {
    return false;
  });
  const setIsOpen = match$2[1];
  const showConfirmDialog = React.useCallback((function (confirmDialog) {
    Curry._1(setConfirmDialog, (function (param) {
      return confirmDialog;
    }));
    Curry._1(setIsOpen, (function (param) {
      return true;
    }));
  }), [
    setIsOpen,
    setConfirmDialog
  ]);
  const handleCancel = React.useCallback((function (param) {
    if (isConfirming) {
      return;
    }
    Curry._1(setIsOpen, (function (param) {
      return false;
    }));
    if (confirmDialog === undefined) {
      return;
    }
    const match = confirmDialog.onCancel;
    if (match !== undefined) {
      return Curry._1(match[1], undefined);
    }
    
  }), [
    confirmDialog,
    isConfirming,
    setIsOpen
  ]);
  const handleConfirm = React.useCallback((function (param) {
    Curry._1(setIsConfirming, (function (param) {
      return true;
    }));
    let confirmation;
    if (confirmDialog !== undefined) {
      const match = confirmDialog.onConfirm;
      confirmation = Curry._1(match[1], undefined);
    } else {
      confirmation = Promise.resolve(undefined);
    }
    return confirmation.then(function (param) {
      Curry._1(setIsConfirming, (function (param) {
        return false;
      }));
      Curry._1(setIsOpen, (function (param) {
        return false;
      }));
      return Promise.resolve(undefined);
    });
  }), [
    confirmDialog,
    setIsConfirming,
    setIsOpen
  ]);
  let tmp;
  if (confirmDialog !== undefined) {
    const match$3 = confirmDialog.onCancel;
    let tmp$1 = {
      title: confirmDialog.title,
      text: confirmDialog.text,
      isOpen: match$2[0],
      onConfirm: [
        confirmDialog.onConfirm[0],
        (function (param) {
          Curry._1(handleConfirm, undefined);
        })
      ],
      loading: isConfirming
    };
    if (confirmDialog.variant !== undefined) {
      tmp$1.variant = Caml_option.valFromOption(confirmDialog.variant);
    }
    const tmp$2 = match$3 !== undefined ? [
        match$3[0],
        handleCancel
      ] : undefined;
    if (tmp$2 !== undefined) {
      tmp$1.onCancel = Caml_option.valFromOption(tmp$2);
    }
    tmp = JsxRuntime.jsx(Components__ConfirmModal.make, tmp$1);
  } else {
    tmp = null;
  }
  return React.createElement(Context_types.Confirm.context.Provider, {
    value: showConfirmDialog,
    children: JsxRuntime.jsxs(JsxRuntime.Fragment, {
      children: [
        children,
        tmp
      ]
    })
  });
}

const Provider = {
  make: ConfirmContext$Provider
};

export {
  Provider,
}
/* Components__ConfirmModal Not a pure module */
