[@mel.module "./Spectator.css"] external css: string = "default";

[@react.component]
let make = (~label, ~className="") => {
  <div className={Css.merge([|"Spectator", className|])}>
    <div className="Spectator-card">
      <Images.Icons.Eye className="Spectator-icon" />
    </div>
    <p className="Spectator-label"> {React.string(label)} </p>
  </div>;
};
