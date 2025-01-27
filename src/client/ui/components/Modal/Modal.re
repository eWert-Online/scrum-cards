[@mel.module "./Modal.css"] external css: string = "default";

[@react.component]
let make = (~className="", ~isOpen, ~onClose, ~children) => {
  let modalRef = React.useRef(Js.Nullable.null);
  Hooks.ClickAway.useClickAway([modalRef], _ => {
    switch (onClose) {
    | Some(onClose) => onClose()
    | None => ()
    }
  });

  let portal =
    React.useMemo0(() =>
      Bindings.Dom.document->Bindings.Dom.createElement("div")
    );

  React.useEffect2(
    () =>
      if (isOpen) {
        let body = Bindings.Dom.document->Bindings.Dom.body;
        body->Bindings.Dom.appendChild(portal);
        Some(() => body->Bindings.Dom.removeChild(portal));
      } else {
        None;
      },
    (isOpen, portal),
  );

  if (isOpen) {
    ReactDOM.createPortal(
      <div className="Modal-wrapper is-active">
        <div className="Modal" ref={ReactDOM.Ref.domRef(modalRef)}>
          {switch (onClose) {
           | Some(onClose) =>
             <button
               type_="button" className="Modal-close" onClick={_ => onClose()}>
               {React.string("close window")}
             </button>
           | None => React.null
           }}
          <div className={Css.merge_list(["Modal-content", className])}>
            children
          </div>
        </div>
      </div>,
      portal,
    );
  } else {
    React.null;
  };
};
