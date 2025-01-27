open Core
module Api = Shared_api_server.Api

module Game = struct
  type t =
    { is_revealed : bool
    ; players : player_data list
    }
  and player_data =
    { id : Api.player_id
    ; socket : Dream.websocket
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
      | Some game -> Some { game with players = player :: game.players })
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

let send ?(only_me = false) request game_id message =
  let to_client =
    match only_me with
    | false -> None
    | true -> Some (Dream.session_id request)
  in
  Game.get_players game_id
  |> Lwt_list.iter_p (fun (data : Game.player_data) ->
    match to_client with
    | None -> Dream.send data.socket message
    | Some id ->
      if String.equal id data.id then Dream.send data.socket message else Lwt.return ())
;;

let send_current_game_state ?(only_me = false) request game_id =
  let game_state = Game.get_public_game_state game_id in
  send ~only_me request game_id (Api.string_of_ws_response (UpdateGameState game_state))
;;

let send_current_card request game_id =
  let card = Dream.session_field request "played_card" in
  send ~only_me:true request game_id (Api.string_of_ws_response (Api.YourCard card))
;;

let forget request game_id =
  let client_id = Dream.session_id request in
  Game.remove_player game_id client_id;
  send_current_game_state request game_id
;;

let handle_message request game_id client_id message =
  match Api.ws_request_of_string message with
  | Api.PlayCard value ->
    let%bind.Lwt () = Dream.set_session_field request "played_card" value in
    let%bind.Lwt () = send_current_card request game_id in
    Game.update_players game_id (function
      | data when String.equal data.id client_id ->
        { data with selected_value = Some value }
      | p -> p);
    send_current_game_state request game_id
  | Api.RevokeCard ->
    let%bind.Lwt () = Dream.drop_session_field request "played_card" in
    let%bind.Lwt () = send_current_card request game_id in
    Game.update_players game_id (function
      | data when String.equal data.id client_id -> { data with selected_value = None }
      | p -> p);
    send_current_game_state request game_id
  | Api.RevealCards ->
    Game.set_revealed game_id true;
    send_current_game_state request game_id
  | Api.InitReset ->
    Game.set_revealed game_id false;
    Game.update_players game_id (fun data -> { data with selected_value = None });
    send request game_id (Api.string_of_ws_response Api.Reset)
  | Api.ResetMe ->
    let%bind.Lwt () = Dream.drop_session_field request "played_card" in
    let%bind.Lwt () = send_current_card request game_id in
    send_current_game_state ~only_me:true request game_id
;;

let handle_client request game_id socket =
  let client_id = Dream.session_id request in
  let name = Dream.session_field request "name" in
  let typ =
    Dream.session_field request "spectator" |> function
    | Some "true" -> `Spectator
    | _ -> `Player
  in
  let selected_value = Dream.session_field request "played_card" in

  Game.add_player
    game_id
    Game.
      { id = client_id
      ; typ
      ; socket
      ; name = Option.value name ~default:"Unnamed Player"
      ; selected_value
      };
  let%bind.Lwt () = send_current_game_state request game_id in
  let%bind.Lwt () = send_current_card request game_id in

  let rec loop () =
    match%bind.Lwt Dream.receive socket with
    | Some message ->
      let%bind.Lwt () = handle_message request game_id client_id message in
      loop ()
    | None ->
      let%bind.Lwt () = forget request game_id in
      Dream.close_websocket socket
  in
  loop ()
;;

let route request =
  let game_id = Dream.param request "id" in
  Dream.websocket (handle_client request game_id)
;;
