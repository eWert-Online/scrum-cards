open Cohttp

let not_found body =
  Cohttp_lwt_unix.Server.respond ~status:`Not_found ~body:(`String body) ()
  |> Lwt.map (fun r -> `Response r)
;;
let forbidden body =
  Cohttp_lwt_unix.Server.respond ~status:`Forbidden ~body:(`String body) ()
  |> Lwt.map (fun r -> `Response r)
;;
let error body =
  Cohttp_lwt_unix.Server.respond ~status:`Internal_server_error ~body:(`String body) ()
  |> Lwt.map (fun r -> `Response r)
;;

let json body =
  Cohttp_lwt_unix.Server.respond
    ~status:`OK
    ~body:(`String body)
    ~headers:(Header.of_list [ "Content-Type", "application/json" ])
    ()
  |> Lwt.map (fun r -> `Response r)
;;

let empty status =
  Cohttp_lwt_unix.Server.respond ~status ~body:`Empty () |> Lwt.map (fun r -> `Response r)
;;
