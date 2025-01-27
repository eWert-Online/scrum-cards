[@mel.module "./BarChart.css"] external css: string = "default";

[@react.component]
let make = (~title, ~entries, ~className="") => {
  <div className={Css.merge([|"BarChart", className|])}>
    <Headline.Secondary className="BarChart-headline">
      {React.string(title)}
    </Headline.Secondary>
    <ul className="BarChart-chart">
      {React.array @@
       Js.Array.map(entries, ~f=((label, value)) => {
         <li
           className="BarChart-entry"
           style={
             ReactDOM.Style._dictToStyle @@
             Js.Dict.fromList([
               ("--BarChart-inline-span", string_of_int(value)),
             ])
           }>
           <span className="BarChart-entryLabel">
             {React.string(label)}
           </span>
           <span className="BarChart-entryValue"> {React.int(value)} </span>
         </li>
       })}
    </ul>
  </div>;
};
