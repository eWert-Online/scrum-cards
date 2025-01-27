[@mel.module "./Switch.css"] external css: string = "default";

[@react.component]
let make =
    (~className="", ~checked, ~onChange, ~description="", ~label, ~name) => {
  let handleChange = evt => {
    let value: bool = evt->React.Event.Form.target##checked;
    onChange(value);
  };

  let has_description = description != "";

  <label
    className={Css.merge([|
      "Switch",
      has_description ? "Switch--described" : "",
      className,
    |])}
    htmlFor=name>
    <input
      type_="checkbox"
      id=name
      name
      checked
      onChange=handleChange
      className="Switch-input"
    />
    <span className="Switch-switch" />
    <div className="Switch-info">
      <div className="Switch-title"> label->React.string </div>
      {if (has_description) {
         <aside className="Switch-description">
           description->React.string
         </aside>;
       } else {
         React.null;
       }}
    </div>
  </label>;
};
