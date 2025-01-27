type nonrec iconProp = (~className: string=?, unit) => React.element;

module ChevronDown = {
  [@react.component]
  let make = (~className="") =>
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      width="24"
      height="24"
      viewBox="-11 0 470 512">
      <path
        fill="currentColor"
        d="M4 181q5 -5 12 -6q6 0 11 4l197 181l197 -181q12 -9 23 1q9 12 -1 23l-208 192q-11 8 -22 0l-208 -192q-10 -10 -1 -22v0z"
      />
    </svg>;

  let prop = (~className=?, ()) => make({"className": className});
};

module Checkmark = {
  [@react.component]
  let make = (~className="") =>
    <svg
      className
      width="24"
      height="24"
      xmlns="http://www.w3.org/2000/svg"
      viewBox="0 0 24 24"
      fillRule="evenodd"
      clipRule="evenodd">
      <path
        fill="currentColor"
        d="M21 6.285l-11.16 12.733-6.84-6.018 1.319-1.49 5.341 4.686 9.865-11.196 1.475 1.285z"
      />
    </svg>;

  let prop = (~className=?, ()) => make({"className": className});
};

module ExclamationCircle = {
  [@react.component]
  let make = (~className="") =>
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      fill="none"
      viewBox="0 0 24 24"
      strokeWidth="1.5"
      stroke="currentColor">
      <path
        strokeLinecap="round"
        strokeLinejoin="round"
        d="M12 9v3.75m9-.75a9 9 0 1 1-18 0 9 9 0 0 1 18 0Zm-9 3.75h.008v.008H12v-.008Z"
      />
    </svg>;

  let prop = (~className=?, ()) => make({"className": className});
};

module ExclamationTriangle = {
  [@react.component]
  let make = (~className="") =>
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      fill="none"
      viewBox="0 0 24 24"
      strokeWidth="1.5"
      stroke="currentColor">
      <path
        strokeLinecap="round"
        strokeLinejoin="round"
        d="M12 9v3.75m-9.303 3.376c-.866 1.5.217 3.374 1.948 3.374h14.71c1.73 0 2.813-1.874 1.948-3.374L13.949 3.378c-.866-1.5-3.032-1.5-3.898 0L2.697 16.126ZM12 15.75h.007v.008H12v-.008Z"
      />
    </svg>;

  let prop = (~className=?, ()) => make({"className": className});
};

module Eye = {
  [@react.component]
  let make = (~className="") =>
    <svg
      xmlns="http://www.w3.org/2000/svg"
      fill="none"
      viewBox="0 0 24 24"
      strokeWidth="1.5"
      stroke="currentColor"
      className>
      <path
        strokeLinecap="round"
        strokeLinejoin="round"
        d="M2.036 12.322a1.012 1.012 0 0 1 0-.639C3.423 7.51 7.36 4.5 12 4.5c4.638 0 8.573 3.007 9.963 7.178.07.207.07.431 0 .639C20.577 16.49 16.64 19.5 12 19.5c-4.638 0-8.573-3.007-9.963-7.178Z"
      />
      <path
        strokeLinecap="round"
        strokeLinejoin="round"
        d="M15 12a3 3 0 1 1-6 0 3 3 0 0 1 6 0Z"
      />
    </svg>;

  let prop = (~className=?, ()) => make({"className": className});
};

module Info = {
  [@react.component]
  let make = (~className="") =>
    <svg
      className
      xmlns="http://www.w3.org/2000/svg"
      fill="none"
      viewBox="0 0 24 24"
      strokeWidth="1.5"
      stroke="currentColor">
      <path
        strokeLinecap="round"
        strokeLinejoin="round"
        d="m11.25 11.25.041-.02a.75.75 0 0 1 1.063.852l-.708 2.836a.75.75 0 0 0 1.063.853l.041-.021M21 12a9 9 0 1 1-18 0 9 9 0 0 1 18 0Zm-9-3.75h.008v.008H12V8.25Z"
      />
    </svg>;

  let prop = (~className=?, ()) => make({"className": className});
};
