module Api = Shared_api_client.Api;
module ApiRoutes = Shared_api_client.Routes;

[@react.component]
let make = (~gameId) => {
  let joinGame = (~typ, ~name) => {
    let data =
      Js.Json.stringify @@
      Api.write_join_game_request @@
      Api.{
        join_game_typ: typ,
        join_game_name: name,
      };

    Fetch.fetchWithInit(
      ApiRoutes.(Builder.sprintf(join_game(), gameId)),
      Fetch.RequestInit.make(
        ~credentials=SameOrigin,
        ~method_=Post,
        ~body=Fetch.BodyInit.make(data),
        (),
      ),
    )
    |> Js.Promise.then_(_ =>
         Js.Promise.resolve @@ Router.push(Router.Route.Play(gameId))
       )
    |> ignore;
  };

  <Components.Layout>
    <Components.Layout.Content>
      <Components.Modal isOpen=true onClose=None>
        <Patterns.JoinGameForm onSubmit=joinGame />
      </Components.Modal>
    </Components.Layout.Content>
  </Components.Layout>;
};
