FROM ocaml/opam:alpine-ocaml-5.1-flambda AS build

WORKDIR /usr/app

RUN sudo apk add --no-cache --update \
	make linux-headers pkgconfig musl-dev gmp-dev libev-dev libressl-dev openssl nodejs npm

ADD --chown=opam scrum-cards.opam package.json package-lock.json ./

RUN npm install
RUN opam install . --yes --deps-only && eval $(opam env)
ADD --chown=opam . .
RUN make build

FROM alpine:latest
WORKDIR /usr/app
COPY --chmod=755 --from=build /usr/app/scrum-cards .
EXPOSE 8080
CMD ["/usr/app/scrum-cards", "serve", "--port=8080"]
