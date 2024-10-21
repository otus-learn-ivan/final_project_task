

//int main(int argc, char ** argv){
#include <iostream>
#include "database_cosem.h"
#if 1
#include <GXDLMS.h>
#include <GXDLMSData.h>
#include <GXDLMSVariant.h>
#include <GXDLMSSettings.h>
#include <GXDLMSValueEventArg.h>
#include <GXBytebuffer.h>
#endif

#include <string>
#include <boost/json.hpp>
#include <boost/version.hpp>
#include <sstream>
#include<span>

struct Create_DB{
    std::string operator()(std::span<std::string> name_base){
        database_cosem::DB db_counters(name_base[0]);
        return Database_cosem_Error_to_str(db_counters.open_database());
    }
};

using Tcomand_answer_err = database_cosem::Database_cosem_Error (database_cosem::DB::*)(std::string command);

std::string f_comand_answer_err(std::span<std::string>& command_str,Tcomand_answer_err handler){
    database_cosem::DB db_counters(command_str[0]);
    auto rez = db_counters.open_database();
    if(rez !=database_cosem::Database_cosem_Error::DATABASE_COSEM_OK){
        return Database_cosem_Error_to_str(rez);
    }
    return Database_cosem_Error_to_str((db_counters.*handler)(command_str[1]));
}
struct Add_PU{
    std::string operator()(std::span<std::string> command_str){
        return f_comand_answer_err(command_str,&database_cosem::DB::add_elem_DB);
    }
};
struct Delete_PU{
    std::string operator()(std::span<std::string> command_str){
        return f_comand_answer_err(command_str,&database_cosem::DB::delete_elem_DB);
    }
};

using Tcomand_table_err = database_cosem::Database_cosem_Error (database_cosem::Telem_DB::*)(std::string command);
struct Handler_table_PU{
    const Tcomand_table_err handler;
    Handler_table_PU(Tcomand_table_err h):handler(h){}
    std::string operator()(std::span<std::string> command_str){
        database_cosem::DB db_counters(command_str[0]);
        auto rez = db_counters.open_database();
        if(rez !=database_cosem::Database_cosem_Error::DATABASE_COSEM_OK){
            return Database_cosem_Error_to_str(rez);
        }
        return Database_cosem_Error_to_str((db_counters.get_elem_in_DB(command_str[1]).*handler)(command_str[2]));
    }
};
struct Create_table_PU:public Handler_table_PU{
    Create_table_PU():Handler_table_PU(&database_cosem::Telem_DB::create_table_elem_DB){}
};
struct Delete_table_PU:public Handler_table_PU{
    Delete_table_PU():Handler_table_PU(&database_cosem::Telem_DB::delete_table_elem_DB){}
};
struct Add_record_to_table{
    std::string operator()(std::span<std::string> command_str){
        database_cosem::DB db_counters(command_str[0]);
        auto rez = db_counters.open_database();
        if(rez !=database_cosem::Database_cosem_Error::DATABASE_COSEM_OK){
            return Database_cosem_Error_to_str(rez);
        }
        return Database_cosem_Error_to_str(db_counters.get_elem_in_DB(command_str[1]).
                add_record_to_table_elem_DB(command_str[2],command_str[3]));
    }
};
struct Get_record_from_table_PU{
    std::string operator()(std::span<std::string> command_str){
        std::cout << "Get_record_from_table_PU" << "\n";
        for(const auto& str:command_str){
           std::cout << str << "\n";
        }
        std::cout << "Get_record_from_table_PU command_str end" << "\n";
        database_cosem::DB db_counters(command_str[0]);
        auto rez = db_counters.open_database();
        if(rez !=database_cosem::Database_cosem_Error::DATABASE_COSEM_OK){
            return Database_cosem_Error_to_str(rez);
        }
        auto v_ansver = db_counters.get_elem_in_DB(command_str[1]).
                 get_record_from_table_elem_DB(command_str[2],command_str[3]);
        std::stringstream  answer;
        for(const auto& str:v_ansver){
           answer << str << "\n";
        }
        return answer.str();
    }
};

std::map<std::string,std::function<std::string(std::span<std::string>)>> Comands{
    {"Create_DB", Create_DB()},
    {"Add_PU",Add_PU()},
    {"Delete_PU",Delete_PU()},
    {"Create_table_PU",Create_table_PU()},
    {"Delete_table_PU",Delete_table_PU()},
    {"Add_record_to_table_PU",Add_record_to_table()},
    {"Get_record_from_table_PU",Get_record_from_table_PU()}
};



struct Trequest_to_PU{
   std::string BD_name;
   std::string PU_name;
   std::string TBL_name;
   std::string request;
   std::string to_json(){
       boost::json::object j;
       j["BD_name"] = BD_name;
       j["PU_name"] = PU_name;
       j["TBL_name"] = TBL_name;
       j["request"] = request;
       return boost::json::serialize(j);
   }
};
std::vector<std::string>  str_to_vector(std::string cmd){
    std::vector<std::string> out_vector;
    std::istringstream iss(cmd);
    while(iss){
        std::string s;
        iss >> s;
        out_vector.push_back(s);
    }
    return out_vector;
}
std::string parser_db_command(std::string cmd){
    std::cout << cmd << std::endl;
    auto vk_str = str_to_vector(cmd);
    return Comands.at(vk_str[0])(std::span<std::string> {vk_str.begin()+1,vk_str.end()});
}

extern int main_client_server(const unsigned short g_port_num_);
int main(int argc, char* argv[]){
    if(argc == 1){return 0;}
    std::cout << "open port: " << argv[1] << " " <<std::endl;
    main_client_server(atoi(argv[1]));
#if 0
    std::stringstream  request;

    Tdescriptor_request<double> request_get_record = {"Ap","double",10.7,10.4};

    request << "Get_record_from_table_PU " << "Test_counters_base " << "456798798744434 "
            << "0.0.98.1.0.255 " << request_get_record.to_json()<<std::endl;
    //std::cout << "1: " << request.str() <<std::endl;
    auto answer = parser_join(request.str());
    std::cout << answer << "\n";
#endif
#if 0

    auto comm = str_to_vector(request.str());
    auto answer = Comands[comm[0]](std::span<std::string> {comm.begin()+1,comm.end()});
    std::cout << answer << "\n";
#endif
#if 0

    std::cout << "11 " << request.str() <<std::endl;

    Trequest_to_PU request_to_PU{
        "Test_counters_base",
        "456798798744433",
        "0.0.98.1.0.255",
        request_get_record.to_json()
    };


    request << "Get_record_from_table_PU " << request_to_PU.to_json();
    std::cout << "2" <<std::endl;

    for (const auto& str: str_to_vector(request.str())){
        std::cout<<str << std::endl;
    }
    std::cout << "3" <<std::endl;
#endif

#if 0
    database_cosem::DB db_counters("Test_counters_base");
    if(db_counters.open_database()!=database_cosem::Database_cosem_Error::DATABASE_COSEM_OK){
        std::cout << "База не создана\n\r";
        return 1;
    }
    // db_counters.print_passport_DB_log();
//     db_counters.add_elem_DB("456798798744434");
//     // std::cout <<  db_counters.get_list_elems_DB();
//     // db_counters.delete_elem_DB("456798798744434");

     std::cout <<  db_counters.get_list_elems_DB();

     // db_counters.get_elem_in_DB("456798798744433").create_table_elem_DB("0.0.98.1.0.255");
     // db_counters.get_elem_in_DB("456798798744434").create_table_elem_DB("0.0.98.1.0.255");

     // Trecord_to_tbl_counter_hour record1 = {"2023-10-26 12:00:00", 10.5, 12.3, 5.2, 7.8};
     // Trecord_to_tbl_counter_hour record2 = {"2023-10-26 13:00:00", 10.6, 12.3, 5.2, 7.8};
     // Trecord_to_tbl_counter_hour record3 = {"2023-10-26 14:00:00", 10.7, 12.3, 5.2, 7.8};
     // db_counters.get_elem_in_DB("456798798744434").add_record_to_table_elem_DB("0.0.98.1.0.255",record1.to_json());
     // db_counters.get_elem_in_DB("456798798744434").add_record_to_table_elem_DB("0.0.98.1.0.255",record2.to_json());
     // db_counters.get_elem_in_DB("456798798744434").add_record_to_table_elem_DB("0.0.98.1.0.255",record3.to_json());

     Tdescriptor_request<double> request = {"Ap","double",10.7,10.4};
     auto v_ansver = db_counters.get_elem_in_DB("456798798744434").get_record_from_table_elem_DB("0.0.98.1.0.255",request.to_json());

     for(const auto& str:v_ansver){
         std::cout << str << "\n";
     }

    // Ttable_description hour{
    //     "0.0.98.0.1.255",
    //     4,
    //     {"double","double","double","double"}
    // };
    // db_counters.get_elem_in_DB("456798798744433").add_record_to_table_elem_DB("0.0.98.1.0.255",hour.to_json());
     // db_counters.get_elem_in_DB("456798798744433").delete_table_elem_DB("0.0.98.1.0.255");
     // db_counters.get_elem_in_DB("456798798744434").delete_table_elem_DB("0.0.98.1.0.255");
     // db_counters.get_elem_in_DB("_456798798744434").delete_table_elem_DB("tabklpckev;k");
     // db_counters.get_elem_in_DB("456798798744434").delete_table_elem_DB("tabklpckev;k");

#endif

#if 0
    std::string dlmsTime_str = "090C07E80A0802090B2D1EFE9834";

    const unsigned char dlmsTime_ch[] = {25,0x07,0xE8,0x0A,0x08,0x02,0x09,0x0B,0x2D,0x1E,0xFE,0x98,0x34,0x00};



    CGXDLMSSettings settings1(false);
    CGXDLMSValueEventArg e1(nullptr,2);
    CGXDLMSVariant value;
    CGXByteBuffer xB;
    CGXDataInfo info;
    for(int i =0;i<sizeof (dlmsTime_ch);i++){
        xB.SetInt8(dlmsTime_ch[i]);
    }
    auto rez = GXHelpers::GetData(&settings1,xB,info, value);

    std::string str_time;
    value.dateTime.GetSystemDateTimeFormat(str_time);

    std::cout << str_time.c_str() << std::endl;

    std::cout << rez << std::endl;

    CGXDLMSData dataTime(dlmsTime_str);
    std::cout << dataTime.GetAttributeCount() << std::endl;
    CGXDLMSVariant xT = dataTime.GetValue(settings1,e1);
    std::cout << xT.GetSize() << std::endl;

#endif

    //data
}
