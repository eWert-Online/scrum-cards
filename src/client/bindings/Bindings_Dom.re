external document: Dom.document = "document";

[@mel.get] external body: Dom.document => Dom.element = "body";

[@mel.get]
external documentElement: Dom.document => Dom.element = "documentElement";

[@mel.send]
external querySelector:
  (Dom.node_like('a), string) => Js.Nullable.t(Dom.element) =
  "querySelector";

[@mel.send]
external querySelectorAll:
  (Dom.node_like('a), string) =>
  Js.Nullable.t(Js.Array.array_like(Dom.element)) =
  "querySelectorAll";

[@mel.send]
external appendChild: (Dom.node_like('a), Dom.node_like('b)) => unit =
  "appendChild";

[@mel.send]
external removeChild: (Dom.node_like('a), Dom.node_like('b)) => unit =
  "removeChild";

[@mel.send]
external createElement: (Dom.document, string) => Dom.element =
  "createElement";

[@mel.send]
external contains: (Dom.node_like('a), Dom.node_like('b)) => bool =
  "contains";

[@mel.send] external click: (Dom.node_like('a), unit) => unit = "click";

[@mel.send]
external addEventListener:
  (
    Dom.eventTarget_like('a),
    [@mel.string] [ | `mouseup(Dom.mouseEvent => unit)]
  ) =>
  unit =
  "addEventListener";

[@mel.send]
external removeEventListener:
  (
    Dom.eventTarget_like('a),
    [@mel.string] [ | `mouseup(Dom.mouseEvent => unit)]
  ) =>
  unit =
  "removeEventListener";

[@mel.get]
external target: Dom.event_like('a) => Dom.eventTarget_like('b) = "target";

[@mel.get]
external reactEventTarget:
  React.Event.synthetic('a) => Dom.eventTarget_like('b) =
  "target";

external btoa: string => string = "btoa";

external atob: string => string = "atob";

module Progress = {
  [@mel.get]
  external loaded: Dom.event_like(Dom._progressEvent) => float = "loaded";

  [@mel.get]
  external total: Dom.event_like(Dom._progressEvent) => float = "total";
};

module Input = {
  [@mel.set] external setValue: (Dom.element, string) => unit = "value";
};

module Window = {
  external window: Dom.window = "window";

  type rafId;
  type intervalId;

  type mediaQueryList = {
    matches: bool,
    media: string,
  };

  [@mel.send]
  external matchMedia: (Dom.window, string) => mediaQueryList = "matchMedia";

  [@mel.send]
  external onMatchMediaEvent:
    (mediaQueryList, [ | `change], mediaQueryList => unit) => unit =
    "addEventListener";

  [@mel.send]
  external requestAnimationFrame: (Dom.window, unit => unit) => rafId =
    "requestAnimationFrame";

  [@mel.send]
  external cancelAnimationFrame: (Dom.window, rafId) => unit =
    "cancelAnimationFrame";

  [@mel.send]
  external setInterval: (Dom.window, unit => unit, int) => intervalId =
    "setInterval";

  [@mel.send]
  external clearInterval: (Dom.window, intervalId) => unit = "clearInterval";

  [@mel.send]
  external addEventListener:
    (Dom.window, [@mel.string] [ | `scroll(Dom.uiEvent => unit)]) => unit =
    "addEventListener";

  [@mel.send]
  external removeEventListener:
    (Dom.window, [@mel.string] [ | `scroll(Dom.uiEvent => unit)]) => unit =
    "removeEventListener";

  [@mel.get] external scrollY: Dom.window => float = "scrollY";

  [@mel.send] external scrollTo: (Dom.window, int, int) => unit = "scrollTo";
};

module Element = {
  type domRect = {
    bottom: float,
    height: float,
    left: float,
    right: float,
    top: float,
    width: float,
    x: float,
    y: float,
  };

  [@mel.get] external id: Dom.element => string = "id";

  [@mel.send] external focus: Dom.element => unit = "focus";

  [@mel.get] external innerText: Dom.element => string = "innerText";

  [@mel.get]
  external previousElementSibling: Dom.element => Js.Nullable.t(Dom.element) =
    "previousElementSibling";

  [@mel.get]
  external nextElementSibling: Dom.element => Js.Nullable.t(Dom.element) =
    "nextElementSibling";

  [@mel.get] external clientHeight: Dom.element => int = "clientHeight";

  [@mel.get] external scrollHeight: Dom.element => int = "scrollHeight";

  [@mel.get] external clientWidth: Dom.element => int = "clientWidth";

  [@mel.get] external scrollWidth: Dom.element => int = "scrollWidth";

  [@mel.get] external scrollTop: Dom.element => int = "scrollTop";

  [@mel.get] external offsetTop: Dom.element => int = "offsetTop";

  [@mel.get] external offsetHeight: Dom.element => int = "offsetHeight";

  [@mel.send]
  external getBoundingClientRect: Dom.element => domRect =
    "getBoundingClientRect";

  [@mel.set] external setScrollTop: (Dom.element, int) => unit = "scrollTop";

  [@mel.send]
  external setAttribute: (Dom.element, string, string) => unit =
    "setAttribute";
};
