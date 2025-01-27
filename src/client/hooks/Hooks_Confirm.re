let useConfirm = () => {
  let context = React.useContext(Context_types.Confirm.context);

  React.useCallback1(
    (~title, ~text, ~variant=?, ~onConfirm, ~onCancel=?, ()) =>
      context(
        Some({
          onConfirm,
          onCancel,
          title,
          text,
          variant,
        }),
      ),
    [|context|],
  );
};
