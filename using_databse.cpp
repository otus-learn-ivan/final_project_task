

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

#include <boost/json.hpp>




int main(){
    database_cosem::DB db_counters("Test_counters_base");
    if(db_counters.open_database()!=database_cosem::Database_cosem_Error::DATABASE_COSEM_OK){
        std::cout << "База не создана\n\r";
        return 1;
    }
    // db_counters.print_passport_DB_log();

//     db_counters.add_elem_DB("456798798744431");
// //    db_counters.print_passport_DB_log();
//     db_counters.add_elem_DB("456798798744431");
//     db_counters.add_elem_DB("456798798744432");
//     db_counters.add_elem_DB("456798798744433");
//     db_counters.add_elem_DB("456798798744434");

//     // std::cout <<  db_counters.get_list_elems_DB();

//     db_counters.delete_elem_DB("456798798744431");
//     // db_counters.delete_elem_DB("456798798744431");
//     db_counters.delete_elem_DB("456798798744432");
//     // db_counters.delete_elem_DB("456798798744433");
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
    Ttable_description hour{
        "0.0.98.0.1.255",
        4,
        {"double","double","double","double"}
    };
    db_counters.get_elem_in_DB("456798798744433").add_record_to_table_elem_DB("0.0.98.1.0.255",hour.to_json());
     // db_counters.get_elem_in_DB("456798798744433").delete_table_elem_DB("0.0.98.1.0.255");
     // db_counters.get_elem_in_DB("456798798744434").delete_table_elem_DB("0.0.98.1.0.255");
     // db_counters.get_elem_in_DB("_456798798744434").delete_table_elem_DB("tabklpckev;k");
     // db_counters.get_elem_in_DB("456798798744434").delete_table_elem_DB("tabklpckev;k");


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
