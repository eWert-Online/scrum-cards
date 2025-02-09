.PHONY: all build release install update fmt clean clear
.SILENT: all build release install update fmt clean clear

all: build

run:
	opam exec -- dune exec -- main serve --port 4444

dev:
	opam exec -- dune exec -w -- main serve --port 4444

build:
	opam exec -- dune build @all

release:
	opam exec -- dune build @all --release
	cp -f ./_build/default/src/bin/main.exe ./scrum-cards

install:
	npm install
	if ! [ -e _opam ]; then \
		opam switch create . --empty ; \
	fi
	opam install . --deps-only --with-dev-setup --yes

update:
	opam update
	opam upgrade

fmt:
	opam exec -- dune build @fmt --auto-promote

clean:
	rm -rf _build

clear: clean
	rm -rf _opam

compile:
	dune exec -- main compile --root ./test/templates
