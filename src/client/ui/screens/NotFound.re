[@react.component]
let make = () => {
  <Components.ConfirmModal
    variant=`Info
    isOpen=true
    title="Page not found"
    text="The URL you entered does not lead to anything we know."
    onConfirm=("Create new game", () => Router.replace(Router.Route.Home))
  />;
};
