// Generated by Melange

import * as Caml_option from "melange.js/caml_option.js";
import * as Components__ConfirmModal from "../components/ConfirmModal/ConfirmModal.js";
import * as Components__Layout from "../components/Layout/Layout.js";
import * as Curry from "melange.js/curry.js";
import * as Fetch from "melange-fetch/Fetch.js";
import * as Patterns__GameBoard from "../patterns/GameBoard/GameBoard.js";
import * as Patterns__PlayGameHeader from "../patterns/PlayGameHeader/PlayGameHeader.js";
import * as Router from "../../router/Router.js";
import * as Shared_api_client from "../../../shared/api/client/shared_api_client.js";
import * as Shared_api_universal__Routes from "../../../shared/api/universal/routes/Routes.js";
import * as Shared_api_universal__Routes__Routes_Builder from "../../../shared/api/universal/routes/Routes_Builder.js";
import * as React from "react";
import * as JsxRuntime from "react/jsx-runtime";

function useGame(id) {
  const match = React.useState(function () {
    return "Loading";
  });
  const setState = match[1];
  React.useEffect((function () {
    const abortController = new AbortController();
    fetch(Curry._1(Shared_api_universal__Routes__Routes_Builder.sprintf(Shared_api_universal__Routes.Api.get_game(undefined)), id), Fetch.RequestInit.make(/* Get */ 0, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, Caml_option.some(abortController.signal))(undefined)).then(function (prim) {
      return prim.json();
    }).then(function (data) {
      const game = Curry._1(Shared_api_client.Api.read_load_game_response, data);
      return Promise.resolve(Curry._1(setState, (function (param) {
        return {
          NAME: "Data",
          VAL: game
        };
      })));
    }).catch(function (param) {
      return Promise.resolve(Curry._1(setState, (function (param) {
        return {
          NAME: "Data",
          VAL: undefined
        };
      })));
    });
    return (function (param) {
      abortController.abort();
    });
  }), [
    id,
    setState
  ]);
  return match[0];
}

function useMe(param) {
  const match = React.useState(function () {
    return "Loading";
  });
  const setState = match[1];
  React.useEffect((function () {
    const abortController = new AbortController();
    fetch(Shared_api_universal__Routes__Routes_Builder.sprintf(Shared_api_universal__Routes.Api.me(undefined)), Fetch.RequestInit.make(/* Get */ 0, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, Caml_option.some(abortController.signal))(undefined)).then(function (prim) {
      return prim.json();
    }).then(function (data) {
      const me = Curry._1(Shared_api_client.Api.read_who_am_i_response, data);
      return Promise.resolve(Curry._1(setState, (function (param) {
        return {
          NAME: "Data",
          VAL: me
        };
      })));
    }).catch(function (param) {
      return Promise.resolve(Curry._1(setState, (function (param) {
        return {
          NAME: "Data",
          VAL: undefined
        };
      })));
    });
    return (function (param) {
      abortController.abort();
    });
  }), [setState]);
  return match[0];
}

function Play(Props) {
  let gameId = Props.gameId;
  const game = useGame(gameId);
  const me = useMe(undefined);
  let exit = 0;
  if (typeof game === "string") {
    return null;
  }
  const game$1 = game.VAL;
  if (game$1 !== undefined && !(typeof me === "string" || me.NAME !== "Data")) {
    const me$1 = me.VAL;
    if (me$1 !== undefined) {
      return JsxRuntime.jsxs(Components__Layout.make, {
        children: [
          JsxRuntime.jsx(Components__Layout.Header.make, {
            children: JsxRuntime.jsx(Patterns__PlayGameHeader.make, {
              game: game$1
            })
          }),
          JsxRuntime.jsx(Components__Layout.Content.make, {
            children: JsxRuntime.jsx(Patterns__GameBoard.make, {
              gameId: gameId,
              game: game$1,
              me: me$1
            })
          })
        ]
      });
    }
    
  } else {
    exit = 2;
  }
  if (exit === 2) {
    if (me === "Loading") {
      return null;
    }
    if (typeof game !== "string" && game.NAME === "Data" && game.VAL === undefined) {
      return JsxRuntime.jsx(Components__ConfirmModal.make, {
        title: "Game not found",
        text: "The game you are looking for does not exist (anymore). You can create a new game instead.",
        variant: "Info",
        isOpen: true,
        onConfirm: [
          "Create new Game",
          (function (param) {
            Router.replace(/* Home */ 0);
          })
        ]
      });
    }
    
  }
  const route = Curry._1(Shared_api_universal__Routes__Routes_Builder.sprintf(Shared_api_universal__Routes.join_game(undefined)), gameId);
  window.location.replace(route);
  return null;
}

const make = Play;

export {
  useGame,
  useMe,
  make,
}
/* Components__ConfirmModal Not a pure module */
