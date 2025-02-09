let handler _socket request body =
  let open Shared_api_server.Routes.Builder in
  let target = Cohttp.Request.resource request in

  match
    match'
      ~target
      (one_of
         [ (s "public" /? wildcard) @--> Routes.static
         ; Shared_api_server.Routes.join_game () @--> Routes.join_game
         ; Shared_api_server.Routes.Api.create_game () @--> Routes.Api.create_game body
         ; Shared_api_server.Routes.Api.get_game () @--> Routes.Api.get_game
         ; Shared_api_server.Routes.Api.join_game () @--> Routes.Api.join_game body
         ; Shared_api_server.Routes.Api.leave_game () @--> Routes.Api.leave_game
         ; Shared_api_server.Routes.Api.me () @--> Routes.Api.who_am_i
         ; Shared_api_server.Routes.Api.websocket () @--> Websocket_handler.route
         ])
  with
  | NoMatch -> Routes.home request
  | FullMatch r -> r request
  | MatchWithTrailingSlash r -> r request
;;

let start ?(port = 80) () =
  print_newline ();
  Logs.info (fun m -> m "Starting server on port %i" port);
  let () = Mirage_crypto_rng_unix.use_default () in
  Lwt_main.run
    (Cohttp_lwt_unix.Server.make_response_action ~callback:handler ()
     |> Cohttp_lwt_unix.Server.create ~mode:(`TCP (`Port port)))
;;
