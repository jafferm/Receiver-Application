#include <gst/gst.h>
#include <glib.h>
#if defined(G_OS_UNIX)
  #include <glib-unix.h>  // g_unix_signal_add
#endif
#include <iostream>
#include <sstream>
#include <string>

class Receiver{
  public:
    typedef struct{
        int port;         // UDP listen port
        std::string source_type;
        std::string sink_type; 
    }Options;
};