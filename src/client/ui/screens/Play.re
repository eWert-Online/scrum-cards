module Api = Shared_api_client.Api;
module ApiRoutes = Shared_api_client.Routes;

let useGame = id => {
  let (state, setState) = React.useState(() => `Loading);

  React.useEffect2(
    () => {
      let abortController = Bindings.AbortController.make();

      Fetch.fetchWithInit(
        ApiRoutes.(Builder.sprintf(get_game(), id)),
        Fetch.RequestInit.make(
          ~method_=Get,
          ~signal={
            Bindings.AbortController.signal(abortController);
          },
          (),
        ),
      )
      |> Js.Promise.then_(Fetch.Response.json)
      |> Js.Promise.then_(data => {
           let game = Api.read_load_game_response(data);
           Js.Promise.resolve @@ setState(_ => `Data(Some(game)));
         })
      |> Js.Promise.catch(_ => {
           Js.Promise.resolve @@ setState(_ => `Data(None))
         })
      |> ignore;

      Some(() => Bindings.AbortController.abort(abortController));
    },
    (id, setState),
  );

  state;
};

let useMe = () => {
  let (state, setState) = React.useState(() => `Loading);

  React.useEffect1(
    () => {
      let abortController = Bindings.AbortController.make();

      Fetch.fetchWithInit(
        ApiRoutes.(Builder.sprintf(me())),
        Fetch.RequestInit.make(
          ~method_=Get,
          ~signal={
            Bindings.AbortController.signal(abortController);
          },
          (),
        ),
      )
      |> Js.Promise.then_(Fetch.Response.json)
      |> Js.Promise.then_(data => {
           let me = Api.read_who_am_i_response(data);
           Js.Promise.resolve @@ setState(_ => `Data(Some(me)));
         })
      |> Js.Promise.catch(_ =>
           Js.Promise.resolve @@ setState(_ => `Data(None))
         )
      |> ignore;

      Some(() => Bindings.AbortController.abort(abortController));
    },
    [|setState|],
  );

  state;
};

[@react.component]
let make = (~gameId) => {
  let game = useGame(gameId);
  let me = useMe();

  switch (game, me) {
  | (`Loading, _)
  | (_, `Loading) => React.null
  | (`Data(None), _) =>
    <Components.ConfirmModal
      variant=`Info
      isOpen=true
      title="Game not found"
      text="The game you are looking for does not exist (anymore). You can create a new game instead."
      onConfirm=(
        "Create new Game",
        (() => Router.replace(Router.Route.Home)),
      )
    />
  | (_, `Data(None)) =>
    <Router.Redirect route={Router.Route.JoinGame(gameId)} />
  | (`Data(Some(game)), `Data(Some(me))) =>
    <Components.Layout>
      <Components.Layout.Header>
        <Patterns.PlayGameHeader game />
      </Components.Layout.Header>
      <Components.Layout.Content>
        <Patterns.GameBoard gameId game me />
      </Components.Layout.Content>
    </Components.Layout>
  };
};
