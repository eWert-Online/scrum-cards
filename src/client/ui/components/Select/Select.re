[@mel.module "./Select.css"] external css: string = "default";

module SelectOption = {
  type t('a) = {
    value: 'a,
    text: string,
    disabled: bool,
  };

  let make = (~disabled=false, ~value, ~text, ()) => {
    value,
    text,
    disabled,
  };
};

module Helpers = {
  let scrollElementIntoView =
      (
        ~listbox: React.ref(Js.Nullable.t(Dom.element)),
        ~el: React.ref(Js.Nullable.t(Dom.element)),
      ) => {
    let listbox = listbox.current |> Js.Nullable.toOption;
    let el = el.current |> Js.Nullable.toOption;
    switch (listbox, el) {
    | (Some(listbox), Some(el)) =>
      Bindings.Dom.Element.(
        if (scrollHeight(listbox) > clientHeight(listbox)) {
          let scrollBottom = clientHeight(listbox) + scrollTop(listbox);
          let elementBottom = offsetTop(el) + offsetHeight(el);

          if (elementBottom > scrollBottom) {
            setScrollTop(listbox, elementBottom - clientHeight(listbox));
          } else if (offsetTop(el) < scrollTop(listbox)) {
            setScrollTop(listbox, offsetTop(el));
          } else {
            ();
          };
        }
      )
    | _ => ()
    };
  };
};

module Single = {
  [@react.component]
  let make =
      (
        ~className="",
        ~disabled=false,
        ~label=?,
        ~name,
        ~options: array(SelectOption.t('a)),
        ~value: option('a),
        ~onChange: 'a => unit,
      ) => {
    open Bindings;

    let buttonRef = React.createRef();
    let listboxRef = React.createRef();
    let focusedEl = React.createRef();

    let (keysSoFar, setKeysSoFar) = React.useState(() => "");
    let (focusedOption, setFocusedOption) = React.useState(() => None);
    let (isOpen, setIsOpen) = React.useState(() => None);

    Hooks.ClickAway.useClickAway([listboxRef, buttonRef], _e =>
      setIsOpen(_ => None)
    );

    React.useEffect2(
      () => {
        Helpers.scrollElementIntoView(~listbox=listboxRef, ~el=focusedEl);
        None;
      },
      (listboxRef, focusedEl),
    );

    React.useEffect3(
      () => {
        switch (isOpen) {
        | None => ()
        | Some(true) =>
          switch (listboxRef.current |> Js.Nullable.toOption) {
          | Some(listbox) => Dom.Element.focus(listbox)
          | None => ()
          }
        | Some(false) =>
          buttonRef.current
          |> Js.Nullable.toOption
          |> Option.map(button => button |> Dom.Element.focus)
          |> ignore
        };

        None;
      },
      (isOpen, buttonRef, listboxRef),
    );

    React.useEffect2(
      () => {
        let timeoutId =
          Js.Global.setTimeout(500, ~f=() => setKeysSoFar(_ => ""));

        Some(() => Js.Global.clearTimeout(timeoutId));
      },
      (keysSoFar, setKeysSoFar),
    );

    let handleChange = (option: SelectOption.t('a)) =>
      if (!disabled && !option.disabled) {
        onChange(option.value);
        setIsOpen(_ => Some(false));
      };

    let findMatchInRange = (startIndex, endIndex) => {
      /* Find the first option starting with the keysSoFar substring, searching in*/
      /* the specified range of options*/

      let foundIndex = ref(0);
      let foundOption =
        Js.Array.findi(options, ~f=(option, index) =>
          if (index >= startIndex && index < endIndex) {
            foundIndex := index;

            option.SelectOption.text
            |> Js.String.toUpperCase
            |> Js.String.indexOf(~search=keysSoFar |> Js.String.toUpperCase)
            == 0;
          } else {
            false;
          }
        );

      (foundIndex.contents, foundOption);
    };

    let focusFirstItem = () =>
      switch (Belt.Array.get(options, 0)) {
      | Some(option) => setFocusedOption(_ => Some((0, option)))
      | None => ()
      };

    let focusLastItem = () => {
      let len = options |> Js.Array.length;
      switch (Belt.Array.get(options, len - 1)) {
      | Some(option) => setFocusedOption(_ => Some((len - 1, option)))
      | None => ()
      };
    };

    let findItemToFocus = key => {
      let searchIndex =
        switch (keysSoFar, focusedOption) {
        | ("", Some((_index, focusedOption))) =>
          let index =
            Js.Array.findIndex(options, ~f=option =>
              option.SelectOption.text == focusedOption.SelectOption.text
            );
          if (index == (-1)) {
            0;
          } else {
            index;
          };
        | (_, _) => 0
        };

      setKeysSoFar(currentKeys => currentKeys ++ key);

      let nextMatch =
        findMatchInRange(searchIndex + 1, options |> Js.Array.length);

      switch (nextMatch) {
      | (_, None) => findMatchInRange(0, searchIndex)
      | match => match
      };
    };

    let handleListKeyDown = e => {
      let key = e |> React.Event.Keyboard.key;

      switch (focusedOption) {
      | None => ()
      | Some((index, focusedOption)) =>
        switch (key) {
        | "Esc"
        | "Escape" =>
          e |> React.Event.Keyboard.preventDefault;
          setIsOpen(_ => Some(false));
        | " "
        | "Enter" =>
          e |> React.Event.Keyboard.preventDefault;
          setIsOpen(_ => Some(false));
          handleChange(focusedOption);
        | "Up"
        | "ArrowUp" =>
          e |> React.Event.Keyboard.preventDefault;
          let prevItem = Belt.Array.get(options, index - 1);
          switch (prevItem) {
          | Some(item) => setFocusedOption(_ => Some((index - 1, item)))
          | None => ()
          };
        | "Down"
        | "ArrowDown" =>
          e |> React.Event.Keyboard.preventDefault;
          let nextItem = Belt.Array.get(options, index + 1);
          switch (nextItem) {
          | Some(item) => setFocusedOption(_ => Some((index + 1, item)))
          | None => ()
          };
        | "Home" =>
          e |> React.Event.Keyboard.preventDefault;
          focusFirstItem();
        | "End" =>
          e |> React.Event.Keyboard.preventDefault;
          focusLastItem();
        | key =>
          let itemToFocus = findItemToFocus(key);
          switch (itemToFocus) {
          | (_, None) => ()
          | (index, Some(item)) =>
            setFocusedOption(_ => Some((index, item)))
          };
        }
      };
    };

    let handleButtonKeyUp = e => {
      switch (e |> React.Event.Keyboard.key) {
      | "Up"
      | "ArrowUp"
      | "Down"
      | "ArrowDown" =>
        e |> React.Event.Keyboard.preventDefault;
        setIsOpen(_ => Some(true));
        handleListKeyDown(e);
      | _ => ()
      };
    };

    let handleListFocus = _e =>
      switch (focusedOption) {
      | None => focusFirstItem()
      | Some(_) => ()
      };

    <div
      className={Css.merge([|
        "Select",
        if (disabled) {"is-disabled"} else {""},
        if (isOpen |> Option.value(~default=false)) {
          "is-open";
        } else {
          "";
        },
        className,
      |])}>
      {switch (label) {
       | Some(label) =>
         <InputLabel className="Select-label" text=label htmlFor=name />
       | None => React.null
       }}
      <div className="Select-inputWrapper">
        <button
          type_="button"
          onClick={_e =>
            setIsOpen(isOpen =>
              switch (isOpen) {
              | Some(state) => Some(!state)
              | None => Some(true)
              }
            )
          }
          onKeyUp=handleButtonKeyUp
          ref={ReactDOM.Ref.domRef(buttonRef)}
          id="Select-toggle"
          className="Select-toggle"
          disabled
          /* ariaHaspopup="listbox"*/
          ariaExpanded={
                         if (Option.value(isOpen, ~default=false)) {
                           true;
                         } else {
                           false;
                         }
                       }
          ariaLabelledby="Select-label Select-toggle">
          {switch (value) {
           | None => React.string("Nothing selected...")
           | Some(value) =>
             options
             |> Js.Array.find(~f=opt => opt.SelectOption.value == value)
             |> Option.map(opt => opt.SelectOption.text)
             |> Option.value(~default="Nothing selected...")
             |> React.string
           }}
        </button>
        <Images.Icons.ChevronDown className="Select-arrow" />
      </div>
      <div className="Select-optionListWrapper">
        <ul
          ref={ReactDOM.Ref.domRef(listboxRef)}
          className={Css.merge([|
            "Select-optionList",
            if (!(isOpen |> Option.value(~default=false))) {
              "is-hidden";
            } else {
              "";
            },
          |])}
          role="listbox"
          tabIndex=(-1)
          ariaLabelledby="Select-label"
          ariaActivedescendant={
            focusedOption
            |> Option.map(((_index, option)) =>
                 name ++ "-" ++ option.SelectOption.text
               )
            |> Option.value(~default="")
          }
          onKeyDown=handleListKeyDown
          onFocus=handleListFocus>
          {options
           |> Js.Array.mapi(~f=(option, index) => {
                let id = name ++ "-" ++ option.SelectOption.text;
                let isFocused =
                  focusedOption
                  |> Option.map(((_index, el)) =>
                       el.SelectOption.text == option.SelectOption.text
                     )
                  |> Option.value(~default=false);

                let isSelected =
                  switch (value) {
                  | None => false
                  | Some(value) => value == option.value
                  };
                let isDisabled = option.disabled;

                <li
                  ref={ReactDOM.Ref.callbackDomRef(el =>
                    if (isFocused) {
                      focusedEl.current = el;
                    }
                  )}
                  className={Css.merge([|
                    "Select-option",
                    if (isFocused) {"is-focused"} else {""},
                    if (isSelected) {"is-selected"} else {""},
                    if (isDisabled) {"is-disabled"} else {""},
                  |])}
                  onMouseOver={_ =>
                    setFocusedOption(_ => Some((index, option)))
                  }
                  onClick={_ => handleChange(option)}
                  role="option"
                  key={option.text}
                  id
                  ariaSelected={if (isSelected) {true} else {false}}>
                  {React.string(option.text)}
                  {if (isSelected) {
                     <Images.Icons.Checkmark className="Select-optionIcon" />;
                   } else {
                     React.null;
                   }}
                </li>;
              })
           |> React.array}
        </ul>
      </div>
    </div>;
  };
};

module Multi = {
  [@react.component]
  let make =
      (
        ~className="",
        ~disabled=false,
        ~label=?,
        ~name,
        ~options: array(SelectOption.t('a)),
        ~minItems=?,
        ~maxItems=?,
        ~value: array('a),
        ~onChange: array('a) => unit,
        ~selectedText=_ => "selected item(s)",
      ) => {
    open Bindings;

    let buttonRef = React.createRef();
    let listboxRef = React.createRef();
    let focusedEl = React.createRef();

    let (keysSoFar, setKeysSoFar) = React.useState(() => "");
    let (focusedOption, setFocusedOption) = React.useState(() => None);
    let (isOpen, setIsOpen) = React.useState(() => None);

    Hooks.ClickAway.useClickAway([listboxRef, buttonRef], _e =>
      setIsOpen(_ => None)
    );

    React.useEffect2(
      () => {
        Helpers.scrollElementIntoView(~listbox=listboxRef, ~el=focusedEl);
        None;
      },
      (listboxRef, focusedEl),
    );

    React.useEffect3(
      () => {
        switch (isOpen) {
        | None => ()
        | Some(true) =>
          switch (listboxRef.current |> Js.Nullable.toOption) {
          | Some(listbox) => Dom.Element.focus(listbox)
          | None => ()
          }
        | Some(false) =>
          buttonRef.current
          |> Js.Nullable.toOption
          |> Option.iter(button => button |> Dom.Element.focus)
        };

        None;
      },
      (isOpen, buttonRef, listboxRef),
    );

    React.useEffect2(
      () => {
        let timeoutId =
          Js.Global.setTimeout(500, ~f=() => setKeysSoFar(_ => ""));

        Some(() => Js.Global.clearTimeout(timeoutId));
      },
      (keysSoFar, setKeysSoFar),
    );

    let handleChange = option =>
      if (!disabled && !option.SelectOption.disabled) {
        if (value |> Js.Array.includes(~value=option.SelectOption.value)) {
          let minValuesReached =
            switch (minItems) {
            | Some(limit) => value |> Js.Array.length <= limit
            | None => false
            };
          if (!minValuesReached) {
            onChange @@
            (value |> Js.Array.filter(~f=value => value != option.value));
          };
        } else {
          let maxValuesReached =
            switch (maxItems) {
            | Some(limit) => value |> Js.Array.length >= limit
            | None => false
            };
          if (!maxValuesReached) {
            onChange(value |> Js.Array.concat(~other=[|option.value|]));
          };
        };
      };

    let findMatchInRange = (startIndex, endIndex) => {
      /* Find the first option starting with the keysSoFar substring, searching in*/
      /* the specified range of options*/

      let foundIndex = ref(0);
      let foundOption =
        options
        |> Js.Array.findi(~f=(option, index) =>
             if (index >= startIndex && index < endIndex) {
               foundIndex := index;

               option.SelectOption.text
               |> Js.String.toUpperCase
               |> Js.String.startsWith(
                    ~prefix=keysSoFar |> Js.String.toUpperCase,
                  );
             } else {
               false;
             }
           );

      (foundIndex.contents, foundOption);
    };

    let focusFirstItem = () =>
      switch (Belt.Array.get(options, 0)) {
      | Some(option) => setFocusedOption(_ => Some((0, option)))
      | None => ()
      };

    let focusLastItem = () => {
      let len = options |> Js.Array.length;
      switch (Belt.Array.get(options, len - 1)) {
      | Some(option) => setFocusedOption(_ => Some((len - 1, option)))
      | None => ()
      };
    };

    let findItemToFocus = key => {
      let searchIndex =
        switch (keysSoFar, focusedOption) {
        | ("", Some((_index, focusedOption))) =>
          let index =
            Js.Array.findIndex(options, ~f=option =>
              option.SelectOption.text == focusedOption.SelectOption.text
            );
          if (index == (-1)) {
            0;
          } else {
            index;
          };
        | (_, _) => 0
        };

      setKeysSoFar(currentKeys => currentKeys ++ key);

      let nextMatch =
        findMatchInRange(searchIndex + 1, options |> Js.Array.length);

      switch (nextMatch) {
      | (_, None) => findMatchInRange(0, searchIndex)
      | match => match
      };
    };

    let handleListKeyDown = e => {
      let key = e |> React.Event.Keyboard.key;

      switch (focusedOption) {
      | None => ()
      | Some((index, focusedOption)) =>
        switch (key) {
        | "Esc"
        | "Escape" =>
          e |> React.Event.Keyboard.preventDefault;
          setIsOpen(_ => Some(false));
        | " "
        | "Enter" =>
          e |> React.Event.Keyboard.preventDefault;
          handleChange(focusedOption);
        | "Up"
        | "ArrowUp" =>
          e |> React.Event.Keyboard.preventDefault;
          let prevItem = Belt.Array.get(options, index - 1);
          switch (prevItem) {
          | Some(item) => setFocusedOption(_ => Some((index - 1, item)))
          | None => ()
          };
        | "Down"
        | "ArrowDown" =>
          e |> React.Event.Keyboard.preventDefault;
          let nextItem = Belt.Array.get(options, index + 1);
          switch (nextItem) {
          | Some(item) => setFocusedOption(_ => Some((index + 1, item)))
          | None => ()
          };
        | "Home" =>
          e |> React.Event.Keyboard.preventDefault;
          focusFirstItem();
        | "End" =>
          e |> React.Event.Keyboard.preventDefault;
          focusLastItem();
        | key =>
          let itemToFocus = findItemToFocus(key);
          switch (itemToFocus) {
          | (_, None) => ()
          | (index, Some(item)) =>
            setFocusedOption(_ => Some((index, item)))
          };
        }
      };
    };

    let handleButtonKeyUp = e => {
      let key = e |> React.Event.Keyboard.key;

      switch (key) {
      | "Up"
      | "ArrowUp"
      | "Down"
      | "ArrowDown" =>
        e |> React.Event.Keyboard.preventDefault;
        setIsOpen(_ => Some(true));
        handleListKeyDown(e);
      | _ => ()
      };
    };

    let handleListFocus = _e =>
      switch (focusedOption) {
      | None => focusFirstItem()
      | Some(_) => ()
      };

    <div
      className={Css.merge([|
        "Select",
        if (disabled) {"is-disabled"} else {""},
        if (Option.value(isOpen, ~default=false)) {
          "is-open";
        } else {
          "";
        },
        className,
      |])}>
      {switch (label) {
       | Some(label) =>
         <InputLabel className="Select-label" text=label htmlFor=name />
       | None => React.null
       }}
      <div className="Select-inputWrapper">
        <button
          type_="button"
          onClick={_e =>
            setIsOpen(isOpen =>
              switch (isOpen) {
              | Some(state) => Some(!state)
              | None => Some(true)
              }
            )
          }
          onKeyUp=handleButtonKeyUp
          ref={ReactDOM.Ref.domRef(buttonRef)}
          id="Select-toggle"
          className="Select-toggle"
          disabled
          /* ariaHaspopup="listbox"*/
          ariaExpanded={
                         if (Option.value(isOpen, ~default=false)) {
                           true;
                         } else {
                           false;
                         }
                       }
          ariaLabelledby="Select-label Select-toggle">
          {value |> selectedText |> React.string}
        </button>
        <Images.Icons.ChevronDown className="Select-arrow" />
      </div>
      <div className="Select-optionListWrapper">
        <ul
          ref={ReactDOM.Ref.domRef(listboxRef)}
          className={Css.merge([|
            "Select-optionList",
            if (!Option.value(isOpen, ~default=false)) {
              "is-hidden";
            } else {
              "";
            },
          |])}
          role="listbox"
          tabIndex=(-1)
          ariaLabelledby="Select-label"
          ariaActivedescendant={
            focusedOption
            |> Option.map(((_index, option)) =>
                 name ++ "-" ++ option.SelectOption.text
               )
            |> Option.value(~default="")
          }
          onKeyDown=handleListKeyDown
          onFocus=handleListFocus>
          {options
           |> Js.Array.mapi(~f=(option, index) => {
                let id = name ++ "-" ++ option.SelectOption.text;
                let isFocused =
                  focusedOption
                  |> Option.map(((_index, el)) =>
                       el.SelectOption.text == option.SelectOption.text
                     )
                  |> Option.value(~default=false);

                let isSelected =
                  value |> Js.Array.includes(~value=option.SelectOption.value);
                let isDisabled = option.SelectOption.disabled;

                <li
                  ref={ReactDOM.Ref.callbackDomRef(el =>
                    if (isFocused) {
                      focusedEl.current = el;
                    }
                  )}
                  className={Css.merge([|
                    "Select-option",
                    if (isFocused) {"is-focused"} else {""},
                    if (isSelected) {"is-selected"} else {""},
                    if (isDisabled) {"is-disabled"} else {""},
                  |])}
                  onMouseOver={_ =>
                    setFocusedOption(_ => Some((index, option)))
                  }
                  onClick={_ => handleChange(option)}
                  role="option"
                  key={option.text}
                  id
                  ariaSelected={if (isSelected) {true} else {false}}>
                  {React.string(option.text)}
                  {if (isSelected) {
                     <Images.Icons.Checkmark className="Select-optionIcon" />;
                   } else {
                     React.null;
                   }}
                </li>;
              })
           |> React.array}
        </ul>
      </div>
    </div>;
  };
};
