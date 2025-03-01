(lang dune 3.15)

(using action-plugin 0.1)

(using melange 0.1)

(using directory-targets 0.1)

(name scrum-cards)

(generate_opam_files true)

(source
 (github ewert-online/scrum-cards))

(license MIT)

(authors "Torben Ewert")

(maintainers "torben@ewert-online.com")

(package
 (name scrum-cards)
 (synopsis "A scrum estimation poker application")
 (depends
  ocaml
  core
  atdgen
  cmdliner
  crunch
  cohttp-lwt-unix
  session-cohttp-lwt
  websocket-lwt-unix
  dune
  irmin
  irmin-fs
  ppx_irmin
  lwt
  (melange
   (= 5.0.1-53))
  melange-atdgen-codec-runtime
  melange-fetch
  melange-json
  melange-webapi
  reason
  reason-react
  reason-react-ppx
  randomconv
  (ocamlformat :with-dev-setup)
  (ocaml-lsp-server :with-dev-setup)))
