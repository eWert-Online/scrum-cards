[@mel.module "./HandCard.css"] external css: string = "default";

[@react.component]
let make = (~label, ~className="", ~isSelected=false, ~onClick=?) => {
  let selectedClass = isSelected ? "is-selected" : "";

  <button
    className={Css.merge([|"HandCard", selectedClass, className|])} ?onClick>
    {React.string(label)}
  </button>;
};
