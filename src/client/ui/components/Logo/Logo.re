[@mel.module "./Logo.css"] external css: string = "default";

[@react.component]
let make = (~className="") => {
  <div className={Css.merge([|"Logo", className|])}>
    <img
      className="Logo-image"
      width="28"
      height="28"
      src="/public/images/logo.svg"
    />
  </div>;
};
