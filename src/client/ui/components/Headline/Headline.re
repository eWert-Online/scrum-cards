[@mel.module "./Headline.css"] external css: string = "default";

module Tag = {
  type t = [
    | `h1
    | `h2
    | `h3
    | `h4
    | `h5
    | `h6
  ];

  let render = (tag, children) =>
    switch (tag) {
    | `h1 => <h1> children </h1>
    | `h2 => <h2> children </h2>
    | `h3 => <h3> children </h3>
    | `h4 => <h4> children </h4>
    | `h5 => <h5> children </h5>
    | `h6 => <h6> children </h6>
    };
};

module Primary = {
  [@react.component]
  let make = (~className="", ~tag: Tag.t=`h2, ~children) =>
    <div
      className={Css.merge([|"Headline", "Headline--primary", className|])}>
      {Tag.render(tag, children)}
    </div>;
};

module Secondary = {
  [@react.component]
  let make = (~className="", ~tag: Tag.t=`h2, ~children) =>
    <div
      className={Css.merge([|"Headline", "Headline--secondary", className|])}>
      {Tag.render(tag, children)}
    </div>;
};

module Subheadline = {
  [@react.component]
  let make = (~className="", ~tag: Tag.t=`h2, ~children) =>
    <div
      className={Css.merge([|
        "Headline",
        "Headline--subheadline",
        className,
      |])}>
      {Tag.render(tag, children)}
    </div>;
};
