#include <stdlib.h>
#include <gst/gst.h>
#include <glib.h>

int main ( int argc, char *argv[] ) {
    GMainLoop *loop;
    GstElement *pipeline, *videosrc, *enc, *pay, *udp;

    gst_init ( &argc, &argv );

    loop = g_main_loop_new (NULL, FALSE);

    if( argc != 4 ) {
        g_printerr ( "Usage: IP PORT PATTERN\n" );
        return -1;
    }

    pipeline = gst_pipeline_new ("video-player");
    videosrc = gst_element_factory_make ("videotestsrc", "source");
    enc = gst_element_factory_make ("x264enc", "enc"); //ERROR NOT INSTALL gstreamer1.0-plugins-ugly
    pay = gst_element_factory_make ("rtph264pay", "rtph264pay");
    udp = gst_element_factory_make ("udpsink", "udp");
   
    g_object_set ( G_OBJECT ( udp ), "host", argv[1], NULL);
    g_object_set ( G_OBJECT ( udp ), "port", atoi(argv[2]), NULL);
    g_object_set ( G_OBJECT ( videosrc ), "pattern", atoi( argv[3]), NULL);

    if ( !pipeline ) {
        g_printerr ("Pipeline.\n");
        return -1;
    }

    if ( !videosrc ) {
        g_printerr ("Source.\n");
        return -1;
    }   

    if ( !enc ) {
        g_printerr ("Enc.\n");
        return -1;
    }

    if ( !pay ) {
        g_printerr ("Pay.\n");
        return -1;
    }

    if ( !udp ) {
        g_printerr ("udp.\n");
        return -1;
    }

   
    
    gst_bin_add_many( GST_BIN ( pipeline ), videosrc, enc, pay, udp, NULL);

    if ( gst_element_link_many ( videosrc, enc, pay, udp, NULL ) != TRUE) {
        return -1;
    }

    gst_element_set_state ( pipeline, GST_STATE_PLAYING );

    g_main_loop_run ( loop );

    gst_element_set_state ( pipeline, GST_STATE_NULL );
    gst_object_unref ( GST_OBJECT ( pipeline ));
    g_main_loop_unref ( loop );

    return 0;
}

/*
INSTALL IN TERMINAL

>sudo apt-get install libgstreamer1.0-dev

>sudo apt-get install gstreamer1.0-plugins-ugly
*/

/*
COMPILE

> sudo make

*/

/*
>sudo ./video_sample 127.0.0.0 50002 0

(program IP port event(0,1...12))
*/

/*
SHOW VODEO_SAMPLE

gst-launch-1.0 \
    udpsrc port=50009 caps= "application/x-rtp, media=(string)video, clock-rate=(int) 90000, encoding-name=(string)H264, payload=(int)96" \
    ! rtph264depay \
    ! decodebin \
    ! videoconvert \
    ! autovideosink
*/


//======================================//

/*=================

EXECUTE DOCKER

>sudo docker build . -t video_sample 
>ifconfig //ip docker
>sudo docker run video_sample 172.17.240.1 50009 13

============================*/