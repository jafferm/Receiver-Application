#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <string>
#include <filesystem>
//#include "../header/ReceiverMain.h"
#include "../header/ReceiverPipelineMgr.h"

int main(int argc, char** argv) {
  //get ini file path
  std::string ini_path = std::filesystem::absolute("../resource/ReceiverApp.ini").string();
  boost::property_tree::ptree ini_file;
  Receiver::Options opt;
  ReceiverPipelineMgr * rcvr_pipln_mgr = new ReceiverPipelineMgr();

  try{
    boost::property_tree::ini_parser::read_ini(ini_path, ini_file);
    opt.port = ini_file.get<int>("Network.port");
    opt.source_type = ini_file.get<std::string>("Source.sourceType");
    opt.sink_type = ini_file.get<std::string>("Sink.sinkType");
  }
  catch(std::exception& e){
    std::cerr << "Error loading INI: "<< e.what() <<std::endl;
  }

  gst_init(&argc, &argv);
  //bool done = true;
  //bool done = rcvr_pipln_mgr->CreateGPipeline(opt);
  if(rcvr_pipln_mgr->CreateGPipeline(opt)){
    rcvr_pipln_mgr->init();
  }
  delete rcvr_pipln_mgr;
  return 0;
}
