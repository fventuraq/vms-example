#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>
#include <glib.h>

int main( int argc, char *argv[]){

    if(argc != 3){
        g_printerr ("Usage:IP_DEST PORT \n");
        return -1;
    }

    GstElement *pipeline;
    GError *err = NULL;
    GMainLoop *loop;

    gst_init(&argc, &argv);
    loop = g_main_loop_new(NULL, FALSE);

    char* pipeline_string;

    asprintf(&pipeline_string, "videomixer name=m sink_1::xpos=10 sink_1::ypos=10 sink_2::xpos=320 sink_2::ypos=10  \
    ! x264enc \
    ! rtph264pay \
    ! queue2 \
    ! udpsink host=%s port=%d \
    videotestsrc pattern=white \
    ! video/x-raw, format=I420, framerate=5/1, width=630, height=320 \
    ! m. \
    udpsrc port=50003 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" \
    ! queue2 \
    ! rtph264depay \
    ! decodebin \
    ! videoconvert \
    ! videoscale \
    ! video/x-raw,width=300,height=300 \
    ! queue2 \
    ! m. \
    udpsrc port=50001 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" \
    ! queue2 \
    ! rtph264depay \
    ! decodebin \
    ! videoconvert \
    ! videoscale \
    ! video/x-raw,width=300,height=300 \
    ! queue2 \
    ! m.",argv[1], atoi(argv[2]));

    pipeline = gst_parse_launch(pipeline_string, &err);
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    g_main_loop_run(loop);

    return 0;
}

/*==============================

>sudo make
>sudo ./video_merge 127.0.0.1 50005


>sudo docker build . -t video_merge
>sudo docker run video_merge 172.17.0.1 5000
================================*/