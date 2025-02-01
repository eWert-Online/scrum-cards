module Api = Shared_api_server.Api

let home _request =
  Dream.html
    {|
      <html lang="en">
        <head>
          <meta charset="UTF-8">
          <meta name="viewport" content="width=device-width, initial-scale=1.0">
          <title>Scrum Cards</title>
          <link rel="icon" type="image/png" href="/public/favicon-96x96.png" sizes="96x96" />
          <link rel="icon" type="image/svg+xml" href="/public/favicon.svg" />
          <link rel="shortcut icon" href="/public/favicon.ico" />
          <link rel="apple-touch-icon" sizes="180x180" href="/public/apple-touch-icon.png" />
          <link rel="manifest" href="/public/site.webmanifest" />
          <link rel="stylesheet" href="/public/dist/bundle.css">
        </head>
        <body>
          <div id="app"></div>
          <script src="/public/dist/bundle.js"></script>
        </body>
      </html>
    |}
;;

let create_game request =
  let%bind.Lwt body = Dream.body request in
  let payload = Api.create_game_request_of_string body in

  let name = payload.create_game_name in
  let deck = payload.create_game_deck in
  let reveal = payload.create_game_reveal in

  let%bind.Lwt game_id = Game.create ~name ~deck ~reveal in

  Api.create_create_game_response ~created_game_id:game_id ()
  |> Api.string_of_create_game_response
  |> Dream.json
;;

let get_game request =
  let id = Dream.param request "id" in

  let%bind.Lwt game = Game.load id in

  match game with
  | None -> Dream.empty `Not_Found
  | Some game ->
    Api.create_load_game_response
      ~game_id:id
      ~game_name:game.Storage.Contents.name
      ~game_deck:game.Storage.Contents.deck
      ~game_reveal:game.Storage.Contents.reveal
      ()
    |> Api.string_of_load_game_response
    |> Dream.json
;;

let join_game request =
  let%bind.Lwt body = Dream.body request in
  let payload = Api.join_game_request_of_string body in

  let name = payload.join_game_name in
  let spectator =
    match payload.join_game_typ with
    | `Spectator -> "true"
    | `Player -> "false"
  in

  let%bind.Lwt () = Dream.set_session_field request "name" name in
  let%bind.Lwt () = Dream.set_session_field request "spectator" spectator in

  Dream.empty `OK
;;

let leave_game request =
  let%bind.Lwt () = Dream.invalidate_session request in
  Dream.redirect
    ~status:`See_Other
    request
    (Printf.sprintf "/join-game/%s" (Dream.param request "id"))
;;

let who_am_i request =
  let id = Dream.session_id request in
  let name = Dream.session_field request "name" in
  let spectator = Dream.session_field request "spectator" in

  match name, spectator with
  | Some name, Some "true" ->
    Dream.json
    @@ Api.string_of_who_am_i_response
    @@ Api.create_who_am_i_response
         ~who_am_i_id:id
         ~who_am_i_typ:`Spectator
         ~who_am_i_name:name
         ()
  | Some name, Some "false" ->
    Dream.json
    @@ Api.string_of_who_am_i_response
    @@ Api.create_who_am_i_response
         ~who_am_i_id:id
         ~who_am_i_typ:`Player
         ~who_am_i_name:name
         ()
  | _ -> Dream.empty `Not_Found
;;

let not_found _request = Dream.empty `Not_Found
