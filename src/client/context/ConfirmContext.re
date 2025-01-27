open Context_types.Confirm;

module Provider = {
  [@react.component]
  let make = (~children) => {
    let (confirmDialog, setConfirmDialog) = React.useState(() => None);
    let (isConfirming, setIsConfirming) = React.useState(() => false);
    let (isOpen, setIsOpen) = React.useState(() => false);

    let showConfirmDialog =
      React.useCallback2(
        confirmDialog => {
          setConfirmDialog(_ => confirmDialog);
          setIsOpen(_ => true);
        },
        (setIsOpen, setConfirmDialog),
      );

    let handleCancel =
      React.useCallback3(
        () =>
          if (!isConfirming) {
            setIsOpen(_ => false);

            switch (confirmDialog) {
            | None => ()
            | Some(confirmDialog) =>
              switch (confirmDialog.onCancel) {
              | None => ()
              | Some((_, fn)) => fn()
              }
            };
          },
        (confirmDialog, isConfirming, setIsOpen),
      );

    let handleConfirm =
      React.useCallback3(
        () => {
          setIsConfirming(_ => true);

          let confirmation =
            switch (confirmDialog) {
            | None => Js.Promise.resolve()
            | Some(confirmDialog) =>
              let (_, fn) = confirmDialog.onConfirm;
              fn();
            };

          confirmation
          |> Js.Promise.then_(() => {
               setIsConfirming(_ => false);
               setIsOpen(_ => false);
               Js.Promise.resolve();
             });
        },
        (confirmDialog, setIsConfirming, setIsOpen),
      );

    React.createElement(
      React.Context.provider(context),
      {
        "value": showConfirmDialog,
        "children":
          <>
            children
            {switch (confirmDialog) {
             | None => React.null
             | Some(request) =>
               <Ui.Components.ConfirmModal
                 isOpen
                 title={request.title}
                 text={request.text}
                 variant=?{request.variant}
                 onCancel=?{
                   switch (request.onCancel) {
                   | None => None
                   | Some((text, _)) => Some((text, handleCancel))
                   }
                 }
                 onConfirm=(
                   fst(request.onConfirm),
                   (() => ignore @@ handleConfirm()),
                 )
                 loading=isConfirming
               />
             }}
          </>,
      },
    );
  };
};
