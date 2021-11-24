#include <stdio.h>
#include <stdlib.h>
#include <gst/gst.h>
#include <glib.h>

int main(int argc, char *argv[]){

    if(argc != 3) {
        g_printerr ("Usage: IP PORT\n");
        return -1;
    }

    GstElement *pipeline;
    GError *err = NULL;
    GMainLoop *loop;

    gst_init(&argc, &argv);
    loop = g_main_loop_new(NULL, FALSE);

    char* pipeline_string;

    //THE PORT IS OF THE VIRTUAL DEVICE

    asprintf(&pipeline_string, "udpsrc port=50002 caps = \"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96\" \
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

/*=====================================

>sudo make
>sudo ./udp_video_black_white 127.0.0.1 50003

====SHOW BLACK AND WHITE =====
>sudo gst-launch-1.0 \
    udpsrc port=50003 caps= "application/x-rtp, media=(string)video, clock-rate=(int) 90000, encoding-name=(string)H264, payload=(int)96" \
    ! rtph264depay \
    ! decodebin \
    ! videoconvert \
    ! autovideosink

=======================================*/