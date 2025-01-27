let merge = classes => Js.Array.join(~sep=" ", classes);

let merge_list = classes => {
  String.concat(" ", classes);
};
