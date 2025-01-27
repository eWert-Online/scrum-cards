open Core

module Contents = struct
  type t =
    { name : string
    ; deck : string list
    ; reveal : [ `Everyone | `SpectatorsOnly ]
    }
  [@@deriving irmin]

  let merge = Irmin.Merge.(option (idempotent t))
end

module Store = Irmin_fs_unix.KV.Make (Contents)

let config = Irmin_fs.config (Filename.concat (Stdlib.Sys.getcwd ()) "db")

let main_branch () =
  let%bind.Lwt repo = Store.Repo.v config in
  Store.main repo
;;

module Info = Irmin_fs_unix.Info (Store.Info)
