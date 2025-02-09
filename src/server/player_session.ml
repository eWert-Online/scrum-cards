module Session = struct
  module Backend = struct
    include Session.Lift.IO (Lwt) (Session.Memory)
    let create () = Session.Memory.create ()
  end
  include Session_cohttp_lwt.Make (Backend)
end

let name = "scrum-cards.session"

let store = Session.Backend.create ()

let get_session request =
  let%bind.Lwt session =
    request |> Cohttp.Request.headers |> Session.of_header store name
  in
  match session with
  | Error _ | Ok None -> Lwt.return_none
  | Ok (Some session) -> Lwt.return_some session
;;

let is_valid request =
  match%bind.Lwt get_session request with
  | None -> Lwt.return_false
  | Some session ->
    (try
       let () = ignore @@ Session_b.payload_of_string session.Session.value in
       Lwt.return_true
     with
     | _ -> Lwt.return_false)
;;

let invalidate request =
  let%bind.Lwt session =
    request |> Cohttp.Request.headers |> Session.of_header store name
  in
  match session with
  | Error _ | Ok None -> Lwt.return_unit
  | Ok (Some session) -> Session.clear store session
;;

let get_key request =
  match%bind.Lwt get_session request with
  | None -> Lwt.return_none
  | Some session -> Lwt.return_some session.Session.key
;;

let get_spectator request =
  match%bind.Lwt get_session request with
  | None -> Lwt.return_none
  | Some session ->
    (try
       let payload = Session_b.payload_of_string session.Session.value in
       Lwt.return_some payload.spectator
     with
     | _ -> Lwt.return_none)
;;

let get_name request =
  match%bind.Lwt get_session request with
  | None -> Lwt.return_none
  | Some session ->
    (try
       let payload = Session_b.payload_of_string session.Session.value in
       Lwt.return_some payload.name
     with
     | _ -> Lwt.return_none)
;;

let get_played_card request =
  match%bind.Lwt get_session request with
  | None -> Lwt.return_none
  | Some session ->
    (try
       let payload = Session_b.payload_of_string session.Session.value in
       Lwt.return payload.played_card
     with
     | _ -> Lwt.return_none)
;;

let set_played_card request played_card =
  match%bind.Lwt get_session request with
  | None -> Lwt.return_unit
  | Some session ->
    (try
       let payload = Session_b.payload_of_string session.Session.value in
       let value = Session_b.string_of_payload { payload with played_card } in
       Session.set store ~value session
     with
     | _ -> Lwt.return_unit)
;;

let set_payload request payload =
  match%bind.Lwt get_session request with
  | None -> Lwt.return ()
  | Some session ->
    (try
       let value = Session_b.string_of_payload payload in
       Session.set store ~value session
     with
     | _ -> Lwt.return ())
;;
