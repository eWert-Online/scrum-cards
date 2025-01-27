[@mel.module "./Input.css"] external css: string = "default";

type t = [
  | `Text
  | `Password
  | `Textarea
];

[@react.component]
let make =
    (
      ~className="",
      ~typ=`Text,
      ~placeholder="",
      ~autoComplete="",
      ~rows=2,
      ~description=?,
      ~label,
      ~name,
      ~value,
      ~onChange=?,
    ) => {
  let handleChange = evt => {
    switch (onChange) {
    | None => ()
    | Some(onChange) =>
      let value = evt->React.Event.Form.target##value;
      onChange(value);
    };
  };

  <div className={Css.merge([|"Input", className|])}>
    <InputLabel text=label htmlFor=name>
      {switch (description) {
       | Some(desc) =>
         <aside className="Input-description"> desc->React.string </aside>
       | None => React.null
       }}
      <div className="Input-fieldWrapper">
        {switch (typ) {
         | `Text =>
           <input
             type_="text"
             className="Input-field"
             id=name
             placeholder
             value
             autoComplete
             readOnly={
               switch (onChange) {
               | Some(_) => false
               | None => true
               }
             }
             onChange={evt => handleChange(evt)}
           />
         | `Password =>
           <input
             type_="password"
             className="Input-field"
             id=name
             placeholder
             value
             autoComplete
             readOnly={
               switch (onChange) {
               | Some(_) => false
               | None => true
               }
             }
             onChange={evt => handleChange(evt)}
           />
         | `Textarea =>
           <textarea
             className="Input-field"
             id=name
             placeholder
             value
             autoComplete
             rows
             readOnly={
               switch (onChange) {
               | Some(_) => false
               | None => true
               }
             }
             onChange={evt => handleChange(evt)}
           />
         }}
      </div>
    </InputLabel>
  </div>;
};
