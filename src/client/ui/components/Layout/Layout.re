[@mel.module "./Layout.css"] external css: string = "default";

module Header = {
  [@react.component]
  let make = (~className="", ~children) => {
    <header className={Css.merge([|"Layout-header", className|])}>
      children
    </header>;
  };
};

module Content = {
  [@react.component]
  let make = (~className="", ~children) => {
    <main className={Css.merge([|"Layout-content", className|])}>
      children
    </main>;
  };
};

[@react.component]
let make = (~className="", ~children) => {
  <div className={Css.merge([|"Layout", className|])}> children </div>;
};
