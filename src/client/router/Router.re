type path = list(string);
type hash = string;
type search = string;

module Route = {
  type t =
    | Home
    | JoinGame(string)
    | Play(string)
    | NotFound;

  let encode = (route): path => {
    switch (route) {
    | Home => [""]
    | JoinGame(game_id) => ["join-game", game_id]
    | Play(game_id) => ["play", game_id]
    | NotFound => ["not-found"]
    };
  };

  let decode = (path: path) => {
    switch (path) {
    | []
    | [""] => Home
    | ["join-game", game_id] => JoinGame(game_id)
    | ["play", game_id] => Play(game_id)
    | _ => NotFound
    };
  };

  let equal = (a, b) => {
    let a = encode(a);
    let b = encode(b);

    a == b;
  };

  let toAbsoluteUri = t => {
    let baseUri = Bindings.Location.origin;
    let path =
      t
      |> encode
      |> List.fold_left((path, segment) => path ++ "/" ++ segment, "");
    baseUri ++ path;
  };
};

type url = {
  path,
  hash,
  search,
  route: Route.t,
};

let decodePath = route =>
  route
  |> Route.encode
  |> List.fold_left((path, segment) => path ++ "/" ++ segment, "");

let push = route => {
  let path = decodePath(route);
  Js.log("Going to:" ++ path);
  ReasonReactRouter.push(path);
};

let replace = route => {
  let path = decodePath(route);
  Js.log("Going to:" ++ path);
  ReasonReactRouter.replace(path);
};

let useUrl = () => {
  let url = ReasonReactRouter.useUrl();
  let path = url.path;
  {
    path,
    hash: url.hash,
    search: url.search,
    route: Route.decode(path),
  };
};

module QueryString = {
  [@mel.get] external location: Dom.window => Dom.location = "location";

  [@mel.get] external search: Dom.location => string = "search";

  let search = () =>
    switch ([%external window]) {
    | None => ""
    | Some(window: Dom.window) =>
      switch (window->location->search) {
      | ""
      | "?" => ""
      | raw => raw |> Js.String.slice(~start=1)
      }
    };

  let getParam = key => {
    let param =
      search()
      |> String.split_on_char('&')
      |> List.find_opt(urlParam => {
           let paramArray = urlParam |> String.split_on_char('=');
           switch (paramArray) {
           | [k, _value] => k === key
           | _ => false
           };
         });

    switch (param) {
    | None => None
    | Some(param) =>
      switch (param |> String.split_on_char('=')) {
      | [_param, value] => Some(value)
      | _ => None
      }
    };
  };
};

module Redirect = {
  [@react.component]
  let make = (~route, ~replace as shouldReplace=false) => {
    React.useEffect2(
      () => {
        if (shouldReplace) {
          replace(route);
        } else {
          push(route);
        };
        None;
      },
      (route, shouldReplace),
    );

    React.null;
  };
};
