[@mel.module "./CreateGameForm.css"] external css: string = "default";

[@react.component]
let make = (~onSubmit) => {
  let (name, setName) = React.useState(() => "");
  let (deck, setDeck) = React.useState(() => Some(`Fibonacci));
  let (canEveryoneReveal, setCanEveryoneReveal) = React.useState(() => true);

  let createGame = evt => {
    React.Event.Form.preventDefault(evt);
    onSubmit(~name, ~deck, ~canEveryoneReveal);
  };

  <form className="CreateGameForm" onSubmit=createGame>
    <Components.Headline.Secondary className="CreateGameForm-title">
      {React.string("Create a new Game")}
    </Components.Headline.Secondary>
    <Components.Input
      label="Game name"
      name="name"
      required=true
      value=name
      onChange=setName
    />
    <Components.Select.Single
      label="Card Deck"
      name="deck"
      options=[|
        Components.Select.SelectOption.make(
          ~disabled=false,
          ~value=`Fibonacci,
          ~text=
            Printf.sprintf(
              {j|Fibonacci (%s)|j},
              String.concat(
                ", ",
                Shared_api_client.Game.Deck.make(`Fibonacci),
              ),
            ),
          (),
        ),
        Components.Select.SelectOption.make(
          ~disabled=false,
          ~value=`Tshirt,
          ~text=
            Printf.sprintf(
              {j|T-Shirt (%s)|j},
              String.concat(", ", Shared_api_client.Game.Deck.make(`Tshirt)),
            ),
          (),
        ),
        Components.Select.SelectOption.make(
          ~disabled=false,
          ~value=`FiveFingers,
          ~text=
            Printf.sprintf(
              {j|5-Fingers (%s)|j},
              String.concat(
                ", ",
                Shared_api_client.Game.Deck.make(`FiveFingers),
              ),
            ),
          (),
        ),
      |]
      value=deck
      onChange={value => {setDeck(_ => Some(value))}}
    />
    <Components.Switch
      label="Only spectators can reveal cards"
      description="Only spectators will be allowed to flip cards and show results. This only applies, if at least one spectator is in the game."
      name="reveal"
      checked={!canEveryoneReveal}
      onChange={value => setCanEveryoneReveal(_ => !value)}
    />
    <Components.Button className="CreateGameForm-button" action=`Submit>
      "Create new Game"
    </Components.Button>
  </form>;
};
