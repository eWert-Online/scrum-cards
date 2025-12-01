[@mel.module "./JoinGameForm.css"] external css: string = "default";

[@react.component]
let make = (~onSubmit) => {
  let (typ, setTyp) = React.useState(() => `Player);
  let (name, setName) = React.useState(() => "");

  let joinGame = evt => {
    React.Event.Form.preventDefault(evt);
    onSubmit(~typ, ~name);
  };

  <form className="JoinGameForm" onSubmit=joinGame>
    <Components.Headline.Secondary className="JoinGameForm-title">
      {React.string("Join Game...")}
    </Components.Headline.Secondary>
    <Components.Input
      label="Your Name"
      name="name"
      value=name
      required=true
      onChange={name => setName(_ => name)}
    />
    <Components.Switch
      label="Join as Spectator"
      description="You will not be able to play cards."
      name="spectator"
      checked={typ === `Spectator}
      onChange={
        fun
        | false => setTyp(_ => `Player)
        | true => setTyp(_ => `Spectator)
      }
    />
    <Components.Button className="JoinGameForm-button" action=`Submit>
      "Join Game"
    </Components.Button>
  </form>;
};
