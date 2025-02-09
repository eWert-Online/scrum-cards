module Payload = Shared_api_server.Api

let html_body =
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

let home ?(headers = []) request =
  let run =
    Cohttp_lwt_unix.Server.respond
      ~status:`OK
      ~body:(`String html_body)
      ~headers:(Cohttp.Header.of_list (headers @ [ "Content-Type", "text/html" ]))
      ()
    |> Lwt.map (fun r -> `Response r)
  in
  match Cohttp.Request.meth request with
  | `GET -> run
  | _ -> Respond.empty `Method_not_allowed
;;

let join_game _id request =
  let run =
    let open Player_session in
    let request_headers = Cohttp.Request.headers request in
    let%bind.Lwt session = Session.of_header_or_create store name "" request_headers in
    let headers = Session.to_cookie_hdrs ~path:"/" ~http_only:true name session in
    home ~headers request
  in

  match Cohttp.Request.meth request with
  | `GET -> run
  | _ -> Respond.empty `Method_not_allowed
;;

let static path request =
  let mime_lookup filename =
    let content_type =
      match Magic_mime.lookup filename with
      | "text/html" -> "text/html; charset=utf-8"
      | content_type -> content_type
    in
    Cohttp.Header.of_list [ "Content-Type", content_type ]
  in
  let run =
    let path = Shared_api_server.Routes.Builder.Parts.wildcard_match path in
    match Assets.read path with
    | None -> Respond.not_found ""
    | Some asset ->
      Cohttp_lwt_unix.Server.respond
        ~status:`OK
        ~body:(`String asset)
        ~headers:(mime_lookup path)
        ()
      |> Lwt.map (fun r -> `Response r)
  in
  match Cohttp.Request.meth request with
  | `GET -> run
  | _ -> Respond.empty `Method_not_allowed
;;

module Api = struct
  let create_game body request =
    let run =
      let%bind.Lwt body = Cohttp_lwt.Body.to_string body in
      let payload = Payload.create_game_request_of_string body in

      let name = payload.create_game_name in
      let deck = payload.create_game_deck in
      let reveal = payload.create_game_reveal in

      let%bind.Lwt game_id = Game.create ~name ~deck ~reveal in

      Payload.create_create_game_response ~created_game_id:game_id ()
      |> Payload.string_of_create_game_response
      |> Respond.json
    in

    match Cohttp.Request.meth request with
    | `POST -> run
    | _ -> Respond.empty `Method_not_allowed
  ;;

  let get_game id request =
    let run =
      let%bind.Lwt game = Game.load id in

      match game with
      | None -> Respond.empty `Not_found
      | Some game ->
        Payload.create_load_game_response
          ~game_id:id
          ~game_name:game.Storage.Contents.name
          ~game_deck:game.Storage.Contents.deck
          ~game_reveal:game.Storage.Contents.reveal
          ()
        |> Payload.string_of_load_game_response
        |> Respond.json
    in

    match Cohttp.Request.meth request with
    | `GET -> run
    | _ -> Respond.empty `Method_not_allowed
  ;;

  let join_game body _id request =
    let run =
      let%bind.Lwt body = Cohttp_lwt.Body.to_string body in
      let payload = Payload.join_game_request_of_string body in

      let name = payload.join_game_name in
      let spectator =
        match payload.join_game_typ with
        | `Spectator -> true
        | `Player -> false
      in

      let%bind.Lwt () =
        Player_session.set_payload request (Session_v.create_payload ~name ~spectator ())
      in

      Respond.empty `OK
    in
    match Cohttp.Request.meth request with
    | `POST -> run
    | _ -> Respond.empty `Method_not_allowed
  ;;

  let leave_game id request =
    let run =
      let%bind.Lwt () = Player_session.invalidate request in
      Cohttp_lwt_unix.Server.respond_redirect
        ~uri:(Uri.of_string Shared_api_server.Routes.(Builder.sprintf (join_game ()) id))
        ()
      |> Lwt.map (fun r -> `Response r)
    in
    match Cohttp.Request.meth request with
    | `GET -> run
    | _ -> Respond.empty `Method_not_allowed
  ;;

  let who_am_i request =
    let run =
      let%bind.Lwt id = Player_session.get_key request in
      let%bind.Lwt name = Player_session.get_name request in
      let%bind.Lwt spectator = Player_session.get_spectator request in

      match id, name, spectator with
      | Some id, Some name, Some true ->
        Respond.json
        @@ Payload.string_of_who_am_i_response
        @@ Payload.create_who_am_i_response
             ~who_am_i_id:id
             ~who_am_i_typ:`Spectator
             ~who_am_i_name:name
             ()
      | Some id, Some name, Some false ->
        Respond.json
        @@ Payload.string_of_who_am_i_response
        @@ Payload.create_who_am_i_response
             ~who_am_i_id:id
             ~who_am_i_typ:`Player
             ~who_am_i_name:name
             ()
      | _ -> Respond.empty `Not_found
    in
    match Cohttp.Request.meth request with
    | `GET -> run
    | _ -> Respond.empty `Method_not_allowed
  ;;
end
