open Core
module Api = Shared_api_server.Api

module Game = struct
  type t =
    { is_revealed : bool
    ; players : player_data list
    }
  and player_data =
    { id : Api.player_id
    ; socket : Websocket.Frame.t option -> unit
    ; typ : Api.typ
    ; name : string
    ; selected_value : Api.card_value option
    }

  let storage : (string, t) Hashtbl.t = Hashtbl.create (module String)

  let get_players game_id =
    let game = Hashtbl.find storage game_id in
    match game with
    | None -> []
    | Some game -> game.players
  ;;

  let add_player game_id player =
    Hashtbl.change storage game_id ~f:(function
      | None -> Some { is_revealed = false; players = [ player ] }
      | Some game ->
        if List.exists game.players ~f:(fun { id; _ } -> String.equal id player.id)
        then Some game
        else (
          let players = player :: game.players in
          Some { game with players }))
  ;;

  let update_players game_id f =
    Hashtbl.change storage game_id ~f:(function
      | None -> None
      | Some game -> Some { game with players = List.map game.players ~f })
  ;;

  let update_players_and_return game_id f =
    let updatedPlayers = ref [] in
    Hashtbl.change storage game_id ~f:(function
      | None -> None
      | Some game ->
        let players = List.map game.players ~f in
        updatedPlayers := players;
        Some { game with players });

    !updatedPlayers
  ;;

  let remove_player game_id player_id =
    Hashtbl.change storage game_id ~f:(function
      | None -> None
      | Some game ->
        let players =
          List.filter game.players ~f:(fun data -> not @@ String.equal data.id player_id)
        in
        Some { game with players })
  ;;

  let set_revealed game_id value =
    Hashtbl.change storage game_id ~f:(function
      | None -> None
      | Some game -> Some { game with is_revealed = value })
  ;;

  let is_revealed game_id =
    match Hashtbl.find storage game_id with
    | None -> false
    | Some game -> game.is_revealed
  ;;

  let get_public_game_state game_id =
    match Hashtbl.find storage game_id with
    | None -> Api.create_game_state ~is_revealed:false ~players:[] ()
    | Some game ->
      let is_revealed = game.is_revealed in
      let players =
        List.map game.players ~f:(fun { id; name; typ; selected_value; _ } ->
          let played_card =
            Option.map selected_value ~f:(fun v ->
              if is_revealed then Api.Revealed v else Api.Hidden)
          in
          Api.create_player ~id ~typ ~name ~played_card ())
      in
      Api.create_game_state ~is_revealed ~players ()
  ;;
end

let send ~client_id ?(only_me = false) _request game_id message =
  let to_client =
    match only_me with
    | false -> None
    | true -> Some client_id
  in
  Game.get_players game_id
  |> Lwt_list.iter_p (fun (data : Game.player_data) ->
    Lwt.return
    @@
    match to_client with
    | None -> data.socket (Some (Websocket.Frame.create ~content:message ()))
    | Some id ->
      if String.equal id data.id
      then data.socket (Some (Websocket.Frame.create ~content:message ())))
;;

let send_current_game_state ~client_id ?(only_me = false) request game_id =
  let game_state = Game.get_public_game_state game_id in
  send
    ~client_id
    ~only_me
    request
    game_id
    (Api.string_of_ws_response (UpdateGameState game_state))
;;

let send_current_card ~client_id request game_id =
  let%bind.Lwt card = Player_session.get_played_card request in
  send
    ~client_id
    ~only_me:true
    request
    game_id
    (Api.string_of_ws_response (Api.YourCard card))
;;

let forget ~client_id request game_id =
  Game.remove_player game_id client_id;
  send_current_game_state ~client_id request game_id
;;

let handle_message ~client_id request game_id message =
  match Api.ws_request_of_string message with
  | Api.PlayCard value ->
    let%bind.Lwt () = Player_session.set_played_card request (Some value) in
    let%bind.Lwt () = send_current_card ~client_id request game_id in
    Game.update_players game_id (function
      | data when String.equal data.id client_id ->
        { data with selected_value = Some value }
      | p -> p);
    send_current_game_state ~client_id request game_id
  | Api.RevokeCard ->
    let%bind.Lwt () = Player_session.set_played_card request None in
    let%bind.Lwt () = send_current_card ~client_id request game_id in
    Game.update_players game_id (function
      | data when String.equal data.id client_id -> { data with selected_value = None }
      | p -> p);
    send_current_game_state ~client_id request game_id
  | Api.RevealCards ->
    Game.set_revealed game_id true;
    send_current_game_state ~client_id request game_id
  | Api.InitReset ->
    Game.set_revealed game_id false;
    Game.update_players game_id (fun data -> { data with selected_value = None });
    send ~client_id request game_id (Api.string_of_ws_response Api.Reset)
  | Api.ResetMe ->
    let%bind.Lwt () = Player_session.set_played_card request None in
    let%bind.Lwt () = send_current_card ~client_id request game_id in
    send_current_game_state ~client_id ~only_me:true request game_id
;;

let handle_client ~session request game_id socket message_stream =
  let client_id = session.Player_session.Session.key in
  let%bind.Lwt name = Player_session.get_name request in
  let%bind.Lwt spectator = Player_session.get_spectator request in
  let%bind.Lwt selected_value = Player_session.get_played_card request in

  let typ =
    match spectator with
    | Some true -> `Spectator
    | _ -> `Player
  in

  Game.add_player
    game_id
    Game.
      { id = client_id
      ; typ
      ; socket
      ; name = Option.value name ~default:"Unnamed Player"
      ; selected_value
      };
  let%bind.Lwt () = send_current_game_state ~client_id request game_id in
  let%bind.Lwt () = send_current_card ~client_id request game_id in

  let rec loop () =
    match%bind.Lwt Lwt_stream.get message_stream with
    | Some Websocket.Frame.{ opcode = Opcode.Text; content; _ } ->
      let%bind.Lwt () = handle_message ~client_id request game_id content in
      loop ()
    | Some Websocket.Frame.{ opcode = Opcode.Ping; _ } ->
      let pong = Websocket.Frame.create ~opcode:Websocket.Frame.Opcode.Pong () in
      let () = socket @@ Some pong in
      loop ()
    | Some Websocket.Frame.{ opcode = Opcode.Close; content; _ } ->
      let close =
        if String.length content >= 2
        then (
          let content = String.sub content ~pos:0 ~len:2 in
          Websocket.Frame.create ~opcode:Websocket.Frame.Opcode.Close ~content ())
        else Websocket.Frame.close 1000
      in
      let () = socket @@ Some close in
      forget ~client_id request game_id
    | Some _ -> loop ()
    | None -> forget ~client_id request game_id
  in
  loop ()
;;

let route game_id request =
  let%bind.Lwt session = Player_session.get_session request in
  match session with
  | Some session ->
    let message_stream, push = Lwt_stream.create () in

    let handler frame =
      let open Websocket.Frame in
      let () = push @@ Some frame in
      match frame.opcode with
      | Opcode.Close -> push None
      | _ -> ()
    in

    let%bind.Lwt response, socket =
      Websocket_cohttp_lwt.upgrade_connection request handler
    in
    Lwt.async (fun () -> handle_client ~session request game_id socket message_stream);
    Lwt.return response
  | None ->
    let%bind.Lwt response =
      Cohttp_lwt_unix.Server.respond ~status:`Unauthorized ~body:`Empty ()
    in
    Lwt.return (`Response response)
;;
