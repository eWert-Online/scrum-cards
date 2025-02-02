open Core

module RandomId = struct
  let make ~len () =
    let string n = Mirage_crypto_rng.generate n in
    let seq gen = Seq.unfold (fun () -> Some (gen (), ())) () in
    let list gen n = seq gen |> Seq.take n |> Stdlib.List.of_seq in
    let alphanum () =
      match Randomconv.int ~bound:10 string >= 5 with
      | false -> Char.of_int_exn (48 + Randomconv.int ~bound:9 string)
      | true -> Char.of_int_exn (97 + Randomconv.int ~bound:25 string)
    in

    let random_list = list alphanum len in
    let id = String.of_char_list random_list in

    id
  ;;
end

type t = Storage.Contents.t

let create ~name ~deck ~reveal =
  let deck = Shared_api_server.Game.Deck.make deck in
  let game = Storage.Contents.{ name; deck; reveal } in

  let%bind.Lwt store = Storage.main_branch () in

  let rec insert () =
    let id = RandomId.make ~len:9 () in
    let%bind.Lwt exists = Storage.Store.mem store [ id ] in
    match exists with
    | true -> insert ()
    | false ->
      let info = Storage.Info.v ~author:"Example" "Create game with id %S" id in
      (try
         let%bind.Lwt () = Storage.Store.set_exn ~info store [ id ] game in
         Lwt.return id
       with
       | _ -> insert ())
  in

  insert ()
;;

let load id =
  let%bind.Lwt store = Storage.main_branch () in
  let%bind.Lwt game = Storage.Store.find store [ id ] in
  Lwt.return game
;;
