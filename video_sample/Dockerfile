FROM alpine:3.10.0

RUN apk add --update --no-cache \
    bash \
    build-base \
    gstreamer gstreamer-dev \
    gst-plugins-base \
    gst-plugins-good \
    gst-plugins-ugly

COPY ./Makefile ./Makefile
COPY ./video_sample.c ./video_sample.c

COPY ./start.sh ./start.sh

USER root

ENTRYPOINT ["./start.sh"]