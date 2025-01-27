[@mel.module "./InputLabel.css"] external css: string = "default";

[@react.component]
let make =
    (
      ~className="",
      ~text,
      ~htmlFor,
      ~inputPosition=`Bottom,
      ~children=React.null,
    ) => {
  let pos =
    switch (inputPosition) {
    | `None => ""
    | `Bottom => "InputLabel--top"
    | `Left => "InputLabel--right"
    | `Right => "InputLabel--left"
    | `Top => "InputLabel--bottom"
    };

  <label className={Css.merge([|"InputLabel", pos, className|])} htmlFor>
    <span className="InputLabel-text"> {React.string(text)} </span>
    children
  </label>;
};
