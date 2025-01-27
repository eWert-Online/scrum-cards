[@react.component]
let make = () => {
  let url = Router.useUrl();

  <React.Suspense fallback=React.null>
    {switch (url.route) {
     | Home => <Screens.CreateGame />
     | JoinGame(gameId) => <Screens.JoinGame gameId />
     | Play(gameId) => <Screens.Play gameId />
     | NotFound => <Screens.NotFound />
     }}
  </React.Suspense>;
};
