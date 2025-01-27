[@mel.module "./ConfirmModal.css"] external css: string = "default";

[@react.component]
let make =
    (
      ~title,
      ~text,
      ~className="",
      ~variant=`Danger,
      ~isOpen=false,
      ~onCancel=?,
      ~onConfirm,
      ~loading=false,
    ) => {
  let (confirmText, onConfirm) = onConfirm;

  <Modal
    className={Css.merge([|
      "ConfirmModal",
      switch (variant) {
      | `Info => "ConfirmModal--info"
      | `Warning => "ConfirmModal--warning"
      | `Danger => "ConfirmModal--danger"
      },
      className,
    |])}
    isOpen
    onClose={Option.map(snd, onCancel)}>
    <div className="ConfirmModal-main">
      {switch (variant) {
       | `Info => <Images.Icons.Info className="ConfirmModal-icon" />
       | `Warning =>
         <Images.Icons.ExclamationTriangle className="ConfirmModal-icon" />
       | `Danger =>
         <Images.Icons.ExclamationCircle className="ConfirmModal-icon" />
       }}
      <div className="ConfirmModal-content">
        <Headline.Secondary tag=`h2 className="ConfirmModal-title">
          {React.string(title)}
        </Headline.Secondary>
        <p className="ConfirmModal-text"> {React.string(text)} </p>
      </div>
    </div>
    <ul className="ConfirmModal-actions">
      {switch (onCancel) {
       | None => React.null
       | Some((text, fn)) =>
         <li className="ConfirmModal-action">
           <Button variant=`Secondary action={`Fn(_ => fn())}> text </Button>
         </li>
       }}
      <li className="ConfirmModal-action">
        <Button
          danger={variant == `Danger}
          variant=`Primary
          action={`Fn(_ => onConfirm())}
          disabled=loading>
          confirmText
        </Button>
      </li>
    </ul>
  </Modal>;
};
