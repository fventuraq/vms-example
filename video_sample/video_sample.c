#include <stdlib.h>
#include <gst/gst.h>
#include <glib.h>

int main ( int argc, char *argv[] ) {
    GMainLoop *loop;
    GstElement *pipeline, *videosrc, *enc, *pay, *udp;

    gst_init ( &argc, &argv );

    lopp = g_main_loop_new (NULL, FALSE);

    if( argc != 4 ) {
        g_printerr ( "Usage: IP PORT PATTERN\n" );
        return -1;
    }

    pipeline    = gst_pipeline_new ( "video-sample" );
    videosrc    = gst_element_factory_make ( "videotestsrc", "source" );
    enc         = gst_element_factory_make ( "x264enc", "enc" );
    pay         = gst_element_factory_make ( "rtph264pay", "rtph264pay" );
    udp         = gst_element_factory_make ( "udpsink", "udp" );

    g_object_set ( G_OBJECT ( videosrc ), "pattern", atoi( argv[3]), NULL);
    g_object_set ( G_OBJECT ( upd ), "host", argv[1], NULL);
    g_object_set ( G_OBJECT ( upd ), "port", atoi(argv[2]), NULL);

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

    void gst_bin_add_many( GstBin *bin, GstElement *element_1, ...)
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