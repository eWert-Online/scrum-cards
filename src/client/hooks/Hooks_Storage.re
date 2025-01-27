let useSessionStorage = (~defaultValue, key) => {
  let (value, setValue) =
    React.useState(() => {
      switch (Dom.Storage.getItem(key, Dom.Storage.sessionStorage)) {
      | None => defaultValue
      | Some(value) => value
      }
    });

  React.useEffect2(
    () => {
      Dom.Storage.setItem(key, value, Dom.Storage.sessionStorage);
      None;
    },
    (value, key),
  );

  (value, setValue);
};

let useLocalStorage = (~defaultValue, key) => {
  let (value, setValue) =
    React.useState(() => {
      switch (Dom.Storage.getItem(key, Dom.Storage.localStorage)) {
      | None => defaultValue
      | Some(value) => value
      }
    });

  React.useEffect2(
    () => {
      Dom.Storage.setItem(key, value, Dom.Storage.localStorage);
      None;
    },
    (value, key),
  );

  (value, setValue);
};
