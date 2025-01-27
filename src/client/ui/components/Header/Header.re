[@mel.module "./Header.css"] external css: string = "default";

[@react.component]
let make = (~className="", ~children) => {
  <header className={Css.merge_list(["Header", className])}>
    children
  </header>;
};
