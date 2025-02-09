[@mel.module "./PlayGameHeader.css"] external css: string = "default";

module ApiRoutes = Shared_api_client.Routes;

[@react.component]
let make = (~game: Shared_api_client.Api.load_game_response) => {
  <Components.Header className="PlayGameHeader">
    <Components.Headline.Secondary>
      <Components.Logo className="PlayGameHeader-logo" />
    </Components.Headline.Secondary>
    <Components.Headline.Secondary>
      {React.string(game.game_name)}
    </Components.Headline.Secondary>
    <div className="PlayGameHeader-actions">
      <Components.Button
        danger=true
        variant=`Primary
        action={
                 `Href(
                   ApiRoutes.(
                     Builder.sprintf(Api.leave_game(), game.game_id)
                   ),
                 )
               }>
        "Leave Game"
      </Components.Button>
    </div>
  </Components.Header>;
};
