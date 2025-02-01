open Core

let session_handler next_handler request =
  let id = Dream.param request "id" in

  let name = Dream.session_field request "name" in
  let spectator = Dream.session_field request "spectator" in

  match name, spectator with
  | Some _, Some _ ->
    let%bind.Lwt response = next_handler request in
    Lwt.return response
  | _ ->
    let%bind.Lwt () = Dream.invalidate_session request in
    Dream.redirect ~status:`See_Other request (Printf.sprintf "/join-game/%s" id)
;;

let loader _root path _request =
  match Assets.read path with
  | None -> Dream.empty `Not_Found
  | Some asset -> Dream.respond asset
;;

let api_routes =
  [ Dream.post "/game/create" @@ Routes.create_game
  ; Dream.get "/game/get/:id" @@ Routes.get_game
  ; Dream.post "/game/join/:id" @@ Routes.join_game
  ; Dream.get "/game/leave/:id" @@ Routes.leave_game
  ; Dream.get "/who-am-i" @@ Routes.who_am_i
  ; Dream.get "/game/websocket/:id" @@ session_handler @@ Websocket.route
  ]
;;

let routes =
  [ Dream.get "/public/**" @@ Dream.static ~loader ""
  ; Dream.scope "/api" [] api_routes
  ; Dream.get "/join-game/:id" @@ Routes.home
  ; Dream.get "/play/:id" @@ session_handler @@ Routes.home
  ; Dream.get "**" @@ Routes.home
  ]
;;

let start ?(port = 80) () =
  let secret = Sys.getenv_exn "SESSION_SECRET" in

  Dream.run ~interface:"0.0.0.0" ~port
  @@ Dream.logger
  @@ Dream.set_secret secret
  @@ Dream.memory_sessions
  @@ Dream.router routes
;;
