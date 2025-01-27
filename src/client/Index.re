[@mel.module "./css/main.css"] external css: string = "default";

switch (ReactDOM.querySelector("#app")) {
| Some(root) =>
  ReactDOM.Client.render(
    ReactDOM.Client.createRoot(root),
    <Context.ConfirmContext.Provider>
      <App />
    </Context.ConfirmContext.Provider>,
  )
| None => ()
};

let () = {
  open Bindings.Dom;
  let setTheme = darkTheme =>
    document
    ->documentElement
    ->Element.setAttribute("data-theme", darkTheme ? "dark" : "light");
  let color_theme =
    Window.(window->matchMedia("(prefers-color-scheme: dark)"));
  switch (Dom.Storage.(localStorage |> getItem("theme"))) {
  | None =>
    Dom.Storage.(localStorage |> setItem("theme", "auto"));
    setTheme(color_theme.matches);
  | Some("auto") =>
    color_theme->Window.onMatchMediaEvent(`change, theme =>
      setTheme(theme.matches)
    );
    setTheme(color_theme.matches);
  | Some(theme) => setTheme(theme == "dark")
  };
};
