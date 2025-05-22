[@mel.module "./GameBoard.css"] external css: string = "default";
module Api = Shared_api_client.Api;
module ApiRoutes = Shared_api_client.Routes;

[@react.component]
let make =
    (
      ~gameId: string,
      ~game: Api.load_game_response,
      ~me: Api.who_am_i_response,
    ) => {
  let gameDeck =
    React.useMemo1(() => Array.of_list(game.game_deck), [|game.game_deck|]);
  let confirm =
    Hooks.Confirm.useConfirm(
      ~variant=`Warning,
      ~title="Not everyone voted",
      ~text=
        "There are some players that did not pick a card yet. Do you really want to reveal all cards?",
      ~onCancel=("Keep hidden", ignore),
      (),
    );
  let (myCard, setMyCard) = React.useState(() => None);
  let (gameState, setGameState) =
    React.useState(() =>
      Api.{
        is_revealed: false,
        players: [],
      }
    );

  let ws = React.useRef(None);

  let send_request = request => {
    switch (ws.current) {
    | None => ()
    | Some(conn) =>
      let data = Js.Json.stringify @@ Api.write_ws_request(request);
      Bindings.Websocket.sendString(conn, data);
    };
  };

  let reset = () => {
    send_request(Api.InitReset);
  };

  let revealCards = () => {
    let hasUnplayedCard =
      List.exists(
        (player: Api.player) => {
          switch (player.typ, player.played_card) {
          | (`Spectator, _) => false
          | (`Player, Some(_)) => false
          | (`Player, None) => true
          }
        },
        gameState.players,
      );

    if (hasUnplayedCard) {
      confirm(
        ~onConfirm=(
          "Reveal all cards",
          () => Js.Promise.resolve @@ send_request(Api.RevealCards),
        ),
        (),
      );
    } else {
      send_request(Api.RevealCards);
    };
  };

  let playCard = card => {
    send_request(Api.PlayCard(card));
  };

  let revokeCard = () => {
    send_request(Api.RevokeCard);
  };

  React.useEffect3(
    () => {
      let wsConnection =
        ApiRoutes.(Builder.sprintf(Api.websocket(), gameId))
        |> Bindings.Websocket.make;
      ws.current = Some(wsConnection);

      Bindings.Websocket.addEventListener(
        wsConnection,
        `message(
          ({data}) => {
            switch (data |> Js.Json.parseExn |> Api.read_ws_response) {
            | Api.Reset =>
              let data =
                Js.Json.stringify @@ Api.write_ws_request(Api.ResetMe);
              Bindings.Websocket.sendString(wsConnection, data);
            | Api.UpdateGameState(s) => setGameState(_ => s)
            | Api.YourCard(s) => setMyCard(_ => s)
            | Api.Ping =>
              let data = Js.Json.stringify @@ Api.write_ws_request(Api.Pong);
              Bindings.Websocket.sendString(wsConnection, data);
            }
          },
        ),
      );

      Some(() => Bindings.Websocket.close(wsConnection));
    },
    (gameId, setGameState, setMyCard),
  );

  <section className="GameBoard">
    <div className="GameBoard-result">
      {if (gameState.is_revealed) {
         <Components.BarChart
           title="Results"
           entries={
             gameState.players
             |> List.filter_map(player => {
                  switch (player.Api.played_card) {
                  | None
                  | Some(Hidden) => None
                  | Some(Revealed(key)) => Some(key)
                  }
                })
             |> List.sort((a, b) => {
                  Js.Array.indexOf(gameDeck, ~value=a)
                  - Js.Array.indexOf(gameDeck, ~value=b)
                })
             |> List.fold_left(
                  (acc, key) => {
                    switch (Js.Map.get(acc, ~key)) {
                    | None => Js.Map.set(acc, ~key, ~value=1)
                    | Some(value) =>
                      Js.Map.set(acc, ~key, ~value=succ(value))
                    }
                  },
                  Js.Map.make(),
                )
             |> Js.Map.toArray
           }
         />;
       } else {
         React.null;
       }}
      {switch (
         game.game_reveal,
         me.who_am_i_typ,
         List.exists(
           player => {player.Api.typ === `Spectator},
           gameState.players,
         ),
       ) {
       | (`SpectatorsOnly, `Player, true) => React.null
       | (`SpectatorsOnly, `Player, false)
       | (`SpectatorsOnly, `Spectator, _)
       | (`Everyone, _, _) =>
         if (gameState.is_revealed) {
           <Components.Button
             variant=`Cta danger=true action={`Fn(_ => reset())}>
             "Reset"
           </Components.Button>;
         } else {
           <Components.Button variant=`Cta action={`Fn(_ => revealCards())}>
             "Reveal Cards"
           </Components.Button>;
         }
       }}
    </div>
    <div className="GameBoard-players">
      {gameState.players
       |> List.filter_map((player: Api.player) => {
            switch (player.typ) {
            | `Spectator => None
            | `Player =>
              Some(
                <Components.BoardCard
                  key={player.id}
                  reveal={gameState.is_revealed}
                  value={
                    switch (player.played_card) {
                    | None => None
                    | Some(Revealed(value)) when gameState.is_revealed =>
                      Some(value)
                    | Some(_) => Some("")
                    }
                  }
                  label={player.name}
                />,
              )
            }
          })
       |> Array.of_list
       |> React.array}
      {gameState.players
       |> List.filter_map((player: Api.player) => {
            switch (player.typ) {
            | `Player => None
            | `Spectator =>
              Some(
                <Components.Spectator key={player.id} label={player.name} />,
              )
            }
          })
       |> Array.of_list
       |> React.array}
    </div>
    <ul className="GameBoard-hand">
      {switch (me.Api.who_am_i_typ) {
       | `Spectator => React.null
       | `Player =>
         game.game_deck
         |> Array.of_list
         |> Js.Array.map(~f=card => {
              let isSelected = myCard === Some(card);

              <li key=card>
                <Components.HandCard
                  onClick={_ =>
                    if (!gameState.is_revealed) {
                      if (isSelected) {
                        revokeCard();
                      } else {
                        playCard(card);
                      };
                    }
                  }
                  isSelected
                  label=card
                />
              </li>;
            })
         |> React.array
       }}
    </ul>
  </section>;
};
