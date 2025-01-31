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
  conf-openssl
  crunch
  dream
  dune
  irmin
  irmin-fs
  ppx_irmin
  lwt
  (melange
   (= 4.0.0-51))
  melange-atdgen-codec-runtime
  melange-fetch
  melange-json
  melange-webapi
  ppx_fields_conv
  reason
  reason-react
  reason-react-ppx
  randomconv
  (ocamlformat :with-dev-setup)
  (ocaml-lsp-server :with-dev-setup)))
