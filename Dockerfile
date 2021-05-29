FROM alpine:latest as build
RUN apk add git build-base upx
WORKDIR /src
ADD . .
RUN make
RUN upx -o atto bin/atto
RUN echo "atto:x:100:101:/" > passwd

FROM scratch
COPY --from=build /src/passwd /etc/passwd
COPY --from=build --chown=100:101 /src/atto .
USER atto
CMD ["./atto"]