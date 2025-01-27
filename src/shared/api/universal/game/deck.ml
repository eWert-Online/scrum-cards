type t = string list

let coffee = {j|☕️|j}

let make = function
  | `Fibonacci ->
    [ "0"; "1"; "2"; "3"; "5"; "8"; "13"; "21"; "34"; "55"; "89"; "?"; coffee ]
  | `Tshirt -> [ "XS"; "S"; "M"; "L"; "XL"; "?"; coffee ]
  | `FiveFingers -> [ "0"; "1"; "2"; "3"; "5"; "?"; coffee ]
;;
