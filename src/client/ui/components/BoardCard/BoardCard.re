[@mel.module "./BoardCard.css"] external css: string = "default";

[@react.component]
let make = (~label, ~reveal=false, ~value=None, ~className="") => {
  let cssVariables =
    React.useMemo0(() => {
      let delay = Js.Math.random_int(50, 800);
      let translateY = Js.Math.random_int(-12, 8);
      let rotate = Js.Math.random_int(-8, 8);
      Js.Dict.fromList([
        ("--BoardCard-inline-transitionDelay", string_of_int(delay) ++ "ms"),
        ("--BoardCard-inline-translateY", string_of_int(translateY) ++ "px"),
        ("--BoardCard-inline-rotate", string_of_int(rotate) ++ "deg"),
      ]);
    });

  <div
    className={Css.merge([|
      "BoardCard",
      reveal ? "is-flipped" : "",
      className,
    |])}>
    <div
      style={ReactDOM.Style._dictToStyle(cssVariables)}
      className="BoardCard-cardWrapper">
      {switch (value) {
       | None => React.null
       | Some(value) =>
         <div className="BoardCard-card">
           <div className="BoardCard-face BoardCard-cardBack" />
           <div className="BoardCard-face BoardCard-cardFront">
             {if (reveal) {
                React.string(value);
              } else {
                React.null;
              }}
           </div>
         </div>
       }}
    </div>
    <p className="BoardCard-label"> {React.string(label)} </p>
  </div>;
};
