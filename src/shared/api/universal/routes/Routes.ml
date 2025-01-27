module Builder = Routes_Builder
open Builder

let create_game () = s "api" / s "game" / s "create" /? nil
let get_game () = s "api" / s "game" / s "get" / str /? nil
let join_game () = s "api" / s "game" / s "join" / str /? nil
let leave_game () = s "api" / s "game" / s "leave" / str /? nil
let me () = s "api" / s "who-am-i" /? nil
let websocket () = s "api" / s "game" / s "websocket" / str /? nil
