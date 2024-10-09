

//int main(int argc, char ** argv){
#include <iostream>
#include "database_cosem.h"
#include <GXDLMS.h>
#include <GXDLMSData.h>
#include <GXDLMSVariant.h>
#include <GXDLMSSettings.h>
#include <GXDLMSValueEventArg.h>
#include <GXBytebuffer.h>
int main(){
    database_cosem::DB db_counters("Test_counters_base");
//    db_counters.open_database();
    if(db_counters.open_database()!=database_cosem::Database_cosem_Error::DATABASE_COSEM_OK){
        std::cout << "База не создана\n\r";
        return 1;
    }

//    std::string dlmsData = "01 09 02 07 09 0C 07 E8 0A 08 02 09 0B 2D 1E FE 98 34 09 08 30 30 31 39 37 31 32 31 11 04 11 05 06 09 08 07 06 10 05 04 0F AA 02 07 09 0C 07 E8 0A 08 02 09 0B 2D 1E FE 98 34 09 08 30 30 31 39 37 31 32 31 11 04 11 05 06 09 08 07 06 10 05 04 0F AA 02 07 09 0C 07 E8 0A 08 02 09 0D 02 1A FE 98 34 09 08 30 30 30 30 30 33 33 37 11 04 11 05 06 09 08 07 06 10 05 04 0F AA 02 07 09 0C 07 E8 0A 08 02 09 0D 02 1A FE 98 34 09 08 30 30 30 30 30 33 33 37 11 04 11 05 06 09 08 07 06 10 05 04 0F AA 02 07 09 0C 07 E8 0A 08 02 09 0D 02 1A FE 98 34 09 08 30 30 30 30 30 33 33 37 11 04 11 05 06 09 08 07 06 10 05 04 0F AA 02 07 09 0C 07 E8 0A 08 02 09 37 3A 19 FE 98 34 09 08 30 30 30 30 30 33 33 38 11 04 11 05 06 09 08 07 06 10 05 04 0F AA 02 07 09 0C 07 E8 0A 08 02 09 37 3A 19 FE 98 34 09 08 30 30 30 30 30 33 33 38 11 04 11 05 06 09 08 07 06 10 05 04 0F AA 02 07 09 0C 07 E8 0A 08 02 09 37 3A 19 FE 98 34 09 08 30 30 30 30 30 33 33 38 11 04 11 05 06 09 08 07 06 10 05 04 0F AA";
//    CGXDLMSData data(dlmsData);
//    std::cout << data.GetAttributeCount() << std::endl;
//    //GXDLMSData
//    //data.GetDescription()

//    CGXDLMSSettings settings(false);
//    CGXDLMSValueEventArg e(nullptr,0);


//    CGXDLMSVariant x = data.GetValue(settings,e);

//    std::cout << x.GetSize() << std::endl;

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


    //data
}
