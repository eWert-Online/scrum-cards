FROM ocaml/opam:alpine-ocaml-5.3-flambda AS build

WORKDIR /usr/app

RUN opam repository add git https://github.com/ocaml/opam-repository.git --all-switches --set-default && \
	sudo apk add --no-cache --update \
	make linux-headers pkgconfig musl-dev gmp-dev libev-dev sqlite-dev nodejs npm

ADD --chown=opam scrum-cards.opam package.json package-lock.json ./

RUN npm install
RUN opam update
RUN opam install . --yes --deps-only && eval $(opam env)
ADD --chown=opam . .
RUN make release

FROM alpine:latest

WORKDIR /usr/app

COPY --chmod=755 --from=build /usr/app/scrum-cards .

EXPOSE 80
CMD ["/usr/app/scrum-cards", "serve", "--port=80"]
