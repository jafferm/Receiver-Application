#include "Receiver.h"
#include<thread>
#include<atomic>

class ReceiverPipelineMgr : public Receiver
{
    GstElement *source, *sink, *pipeline;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    std::thread *streaming_thread, *error_thread;
    std::atomic<bool> done_playing{false};

    public:
    bool CreateGPipeline(const Options &opt);
    void StartPlaying();
    void init();
    void MonitorError();
    ~ReceiverPipelineMgr(){
        std::cout<<"Destructor called";
        done_playing.store(false);
        streaming_thread->join();
        error_thread->join();
    }
};