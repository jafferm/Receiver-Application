#include "../header/ReceiverPipelineMgr.h"

void ReceiverPipelineMgr::init(){
    std::cout<<"ReceiverPipelineMgr::init() starting threads"<<std::endl;
    streaming_thread = new std::thread(&ReceiverPipelineMgr::StartPlaying, this);
    error_thread = new std::thread(&ReceiverPipelineMgr::MonitorError, this);
    temp_timer = new std::thread(&ReceiverPipelineMgr::FixedTimer, this);
}

void ReceiverPipelineMgr::FixedTimer()
{
    auto start_time = std::chrono::steady_clock::now();
    int counter = 250;
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        std::cout<<"FixedTimer: time since start: "<<counter<<" (ms)"<<std::endl;
        counter+=250;
        auto curr_time = std::chrono::steady_clock::now();
        if(curr_time - start_time >= std::chrono::seconds(3)){
            std::cout<<"FixedTimer: 3 secs passed stop playing"<<std::endl;
            gst_element_set_state(pipeline, GST_STATE_PAUSED);
            done_playing.store(true);
            break;
        }
    } 
}

bool ReceiverPipelineMgr::CreateGPipeline(const Options &opt)
{
    source = gst_element_factory_make (opt.source_type.c_str(), "source");
    sink = gst_element_factory_make (opt.sink_type.c_str(), "sink");

    pipeline = gst_pipeline_new("test-pipeline");

    /* Build the pipeline */
    gst_bin_add_many (GST_BIN (pipeline), source, sink, NULL);
    if (gst_element_link (source, sink) != TRUE) {
        g_printerr ("Elements could not be linked.\n");
        gst_object_unref (pipeline);
        return false;
    }

    if(!source || !sink){ 
        std::cout<<"Failed to initialize source or sink"<<std::endl;
    }
    else{
        std::cout<<"Source and sink initialized"<<std::endl;
        return true;
    }
    return false;
}

void ReceiverPipelineMgr::StartPlaying()
{
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if(ret == GST_STATE_CHANGE_FAILURE){
        std::cout<<"StartPlaying: Unable to set pipline to playing state"<<std::endl;
        gst_object_unref (pipeline);
    }
   //std::cout<<"StartPlaying: reached end"<<std::endl; 
}

void ReceiverPipelineMgr::MonitorError()
{
    bus = gst_element_get_bus(pipeline);

    while(true)
    {
        msg = gst_bus_timed_pop_filtered(bus, 100 * GST_MSECOND, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
        if (msg != NULL) {
            GError *err;
            gchar *debug_info;

            switch (GST_MESSAGE_TYPE (msg)) {
                case GST_MESSAGE_ERROR:
                    gst_message_parse_error (msg, &err, &debug_info);
                    g_printerr ("Error received from element %s: %s\n",
                        GST_OBJECT_NAME (msg->src), err->message);
                    g_printerr ("Debugging information: %s\n",
                        debug_info ? debug_info : "none");
                    g_clear_error (&err);
                    g_free (debug_info);
                    break;
                case GST_MESSAGE_EOS:
                    g_print ("End-Of-Stream reached.\n");
                    break;
                default:
                    /* We should not reach here because we only asked for ERRORs and EOS */
                    g_printerr ("Unexpected message received.\n");
                    break;
            }
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(250));

        if(done_playing.load())
        {
            std::cout<<"MonitorError: Done Playing exiting loop"<<std::endl;
            break;
        }
    }
    std::cout<<"MonitorError: Exited loop"<<std::endl;
    gst_message_unref (msg);
}