module Api = Shared_api_client.Api;
module ApiRoutes = Shared_api_client.Routes;

[@react.component]
let make = () => {
  let createGame = (~name, ~deck, ~canEveryoneReveal) => {
    let create_game_name = name;
    let create_game_deck =
      switch (deck) {
      | None
      | Some(`Fibonacci) => `Fibonacci
      | Some(`FiveFingers) => `FiveFingers
      | Some(`Tshirt) => `Tshirt
      };
    let create_game_reveal =
      switch (canEveryoneReveal) {
      | false => `SpectatorsOnly
      | true => `Everyone
      };

    let data =
      Js.Json.stringify @@
      Api.write_create_game_request @@
      Api.{
        create_game_name,
        create_game_reveal,
        create_game_deck,
      };

    Fetch.fetchWithInit(
      ApiRoutes.(Builder.sprintf(create_game())),
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=Fetch.BodyInit.make(data),
        (),
      ),
    )
    |> Js.Promise.then_(Fetch.Response.json)
    |> Js.Promise.then_(data =>
         Js.Promise.resolve @@ Api.read_create_game_response(data)
       )
    |> Js.Promise.then_((data: Api.create_game_response) =>
         Js.Promise.resolve @@
         Router.push(Router.Route.Play(data.created_game_id))
       )
    |> ignore;
  };

  <Components.Layout>
    <Components.Layout.Content>
      <Components.Modal isOpen=true onClose=None>
        <Patterns.CreateGameForm onSubmit=createGame />
      </Components.Modal>
    </Components.Layout.Content>
  </Components.Layout>;
};
