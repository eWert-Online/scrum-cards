[@mel.module "./Button.css"] external css: string = "default";

[@react.component]
let make =
    (
      ~className="",
      ~variant=`Primary,
      ~danger=false,
      ~buttonRef=?,
      ~disabled=false,
      ~action,
      ~icon=?,
      ~iconPosition=`Left,
      ~children,
    ) => {
  let fallbackRef = React.useRef(Js.Nullable.null);

  let variantClass =
    switch (variant) {
    | `Primary => "Button--primary"
    | `Secondary => "Button--secondary"
    | `Cta => "Button--cta"
    };

  let dangerClass = danger ? "Button--danger" : "";

  let icon = className =>
    switch (icon) {
    | None => React.null
    | Some(icon) =>
      icon(~className=Css.merge_list(["Button-icon", className]), ())
    };

  let classes =
    Css.merge_list(["Button", dangerClass, variantClass, className]);

  let contents =
    switch (iconPosition) {
    | `Left =>
      <>
        {icon("Button-icon--left")}
        <span className="Button-text"> {React.string(children)} </span>
      </>
    | `Right =>
      <>
        <span className="Button-text"> {React.string(children)} </span>
        {icon("Button-icon--right")}
      </>
    };

  switch (action) {
  | `Href(href) =>
    <a
      className=classes
      disabled
      href
      ref={
        switch (buttonRef) {
        | None => ReactDOM.Ref.domRef(fallbackRef)
        | Some(r) => r
        }
      }>
      contents
    </a>
  | `Fn(onClick) =>
    <button
      className=classes
      disabled
      onClick
      ref={
        switch (buttonRef) {
        | None => ReactDOM.Ref.domRef(fallbackRef)
        | Some(r) => r
        }
      }>
      contents
    </button>
  | `Submit =>
    <button
      className=classes
      type_="submit"
      disabled
      ref={
        switch (buttonRef) {
        | None => ReactDOM.Ref.domRef(fallbackRef)
        | Some(r) => r
        }
      }>
      contents
    </button>
  };
};
