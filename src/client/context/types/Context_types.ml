module Confirm = struct
  type request =
    { variant : [ `Info | `Warning | `Danger ] option
    ; title : string
    ; text : string
    ; onConfirm : string * (unit -> unit Js.Promise.t)
    ; onCancel : (string * (unit -> unit)) option
    }

  type t = request option -> unit

  let context : t React.Context.t = React.createContext ignore
end
