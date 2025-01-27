open Cmdliner

let serve port = Server.start ~port ()

let serve_cmd =
  let port =
    let doc = "The port to serve from" in
    let env = Cmd.Env.info "PORT" ~doc in
    Arg.(value & opt int 80 & info [ "port"; "p" ] ~docv:"PORT" ~doc ~env)
  in
  let doc = "Starts the Server" in
  let info = Cmd.info "serve" ~doc in
  Cmd.v info Term.(const serve $ port)
;;

let main_cmd =
  let doc = "" in
  let info = Cmd.info "scrum-cards" ~version:"%%VERSION%%" ~doc in
  let default = Term.(ret (const (`Help (`Pager, None)))) in
  Cmd.group info ~default [ serve_cmd ]
;;

let () = exit (Cmd.eval ~catch:false main_cmd)
