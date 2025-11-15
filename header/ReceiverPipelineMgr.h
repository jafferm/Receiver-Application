#include "Receiver.h"
#include<thread>
#include<chrono>
#include<atomic>

class ReceiverPipelineMgr : public Receiver
{
    GstElement *source, *sink, *pipeline;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    std::thread *streaming_thread, *error_thread, *temp_timer;
    std::atomic<bool> done_playing{false};

    public:
    bool CreateGPipeline(const Options &opt);
    void StartPlaying();
    void init();
    void MonitorError();
    void FixedTimer();
    ~ReceiverPipelineMgr(){
        std::cout<<"Destructor called"<<std::endl;
        //done_playing.store(false);
        temp_timer->join();
        streaming_thread->join();
        error_thread->join();
        delete streaming_thread;
        delete error_thread;
        delete temp_timer;
        std::cout<<"All threads done"<<std::endl;
    }
};