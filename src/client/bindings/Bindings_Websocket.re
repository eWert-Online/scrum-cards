type t;
type messageEvent = {data: string};
type messageEventHandler = messageEvent => unit;

[@mel.new] external make: string => t = "WebSocket";
[@mel.send] external close: t => unit = "close";
[@mel.send] external sendString: (t, string) => unit = "send";

[@mel.send]
external addEventListener:
  (
    t,
    [@mel.string] [
      | `close(Dom.closeEvent => unit)
      | `error(Dom.event => unit)
      | `message(messageEvent => unit)
      | [@mel.as "open"] `open_(Dom.event => unit)
    ]
  ) =>
  unit =
  "addEventListener";
