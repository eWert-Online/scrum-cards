// Generated by Melange

import PlayGameHeaderCss from "./PlayGameHeader.css";
import * as Components__Button from "../../components/Button/Button.js";
import * as Components__Header from "../../components/Header/Header.js";
import * as Components__Headline from "../../components/Headline/Headline.js";
import * as Components__Logo from "../../components/Logo/Logo.js";
import * as Curry from "melange.js/curry.js";
import * as Shared_api_universal__Routes from "../../../../shared/api/universal/routes/Routes.js";
import * as Shared_api_universal__Routes__Routes_Builder from "../../../../shared/api/universal/routes/Routes_Builder.js";
import * as JsxRuntime from "react/jsx-runtime";

const css = PlayGameHeaderCss;

function PlayGameHeader(Props) {
  let game = Props.game;
  return JsxRuntime.jsxs(Components__Header.make, {
              className: "PlayGameHeader",
              children: [
                JsxRuntime.jsx(Components__Headline.Secondary.make, {
                      children: JsxRuntime.jsx(Components__Logo.make, {
                            className: "PlayGameHeader-logo"
                          })
                    }),
                JsxRuntime.jsx(Components__Headline.Secondary.make, {
                      children: game.game_name
                    }),
                JsxRuntime.jsx("div", {
                      children: JsxRuntime.jsx(Components__Button.make, {
                            variant: "Primary",
                            danger: true,
                            action: {
                              NAME: "Href",
                              VAL: Curry._1(Shared_api_universal__Routes__Routes_Builder.sprintf(Shared_api_universal__Routes.Api.leave_game(undefined)), game.game_id)
                            },
                            children: "Leave Game"
                          }),
                      className: "PlayGameHeader-actions"
                    })
              ]
            });
}

const make = PlayGameHeader;

export {
  css ,
  make ,
}
/* css Not a pure module */
