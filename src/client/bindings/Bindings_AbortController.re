/*
   const toBase64 = file => new Promise((resolve, reject) => {
     const reader = new FileReader();
     reader.readAsDataURL(file);
     reader.onload = () => resolve(reader.result);
     reader.onerror = error => reject(error);
   });
 */
type t;

[@mel.new] external make: unit => t = "AbortController";
[@mel.send] external abort: t => unit = "abort";
[@mel.get] external signal: t => Fetch.signal = "signal";
