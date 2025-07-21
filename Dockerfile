FROM scratch

COPY scrum-cards /

CMD ["/scrum-cards", "serve", "--port=80"]
