#include <iostream>
#include <string>
#include "../produser_consumer.h"
#include "../mapper_all.h"
#include <boost/property_tree/json_parser.hpp>

struct Thandler_mapper_awerage{
  size_t number_of_numbers;
  long long summ_of_numbers;
  Thandler_mapper_awerage():number_of_numbers{}, summ_of_numbers{}{}
  void operator()(boost::json::value strm){
    double number = strm.get_double();
    summ_of_numbers += static_cast<long long>(number*100);
    number_of_numbers++;
  }
  Thandler_mapper_awerage(Thandler_mapper_awerage&& oner) = default;
  Thandler_mapper_awerage(Thandler_mapper_awerage& oner) = default;
};

const size_t default_numbers_of_thread =5;

void procces_insaid_start (size_t number_threads, std::string vector_data ){
     std::stringstream strm (vector_data);
     auto v_hand_mapp_awerag = procces_insaid<T_string_queue,Tthread_mapper_json<Thandler_mapper_awerage>>(number_threads,strm);
//     auto v_hand_mapp_awerag = procces_insaid<T_string_queue,Thandler_mapper_awerage>(number_threads,vector_data);
    for(auto& m_handler_av: *v_hand_mapp_awerag){
        std::cout << m_handler_av->get_handler().number_of_numbers << " "
             << m_handler_av->get_handler().summ_of_numbers   << std::endl;
    }
}
