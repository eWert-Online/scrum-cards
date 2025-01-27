open Bindings.Dom;

let useClickAway = (elementRefs, callback) => {
  React.useEffect2(
    () => {
      let handleClick = evt => {
        let clickedInside =
          List.exists(
            (elementRef: React.ref(Js.Nullable.t(Dom.element))) => {
              switch (elementRef.current->Js.Nullable.toOption) {
              | Some(node) =>
                let targetElement = evt->target;
                node->contains(targetElement);
              | None => false
              }
            },
            elementRefs,
          );

        if (!clickedInside) {
          callback(evt);
        } else {
          ();
        };
      };

      document->addEventListener(`mouseup(handleClick));
      Some(() => document->removeEventListener(`mouseup(handleClick)));
    },
    (callback, elementRefs),
  );
};
