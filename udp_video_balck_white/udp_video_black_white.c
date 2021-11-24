#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>
#include <glib.h>

int main(int argc, char *argv[]){

    if(argc != 3) {
        g_printerr ("Usage: UP PORT\n");
        return -1;
    }

    GstElement *pipeline;
    GError *err = NULL;
    GMainLoop *loop;

    gst_init(&argc, &argv);
    loop = g_main_loop_new(NULL, FALSE);

    char* pipeline_string;

    asprintf(&pipeline_string, "udpsrc port=5000 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" \
    ! rtph264depay \
    ! decodebin \
    ! videobalance saturation=0 \
    ! x264enc \
    ! rtph264pay \
    ! udpsink host=%s port=%d", argv[1], atoi(argv[2]));

    pipeline = gst_parse_launch(pipeline_string, &err);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    g_main_loop_run(loop);

    return 0;
}