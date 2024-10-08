

//int main(int argc, char ** argv){
#include <iostream>
#include "database_cosem.h"

int main(){
    database_cosem::DB db_counters("Test_counters_base");
    if(db_counters.open_database()!=database_cosem::Database_cosem_Error::DATABASE_COSEM_OK){
        std::cout << "База не создана\n\r";
        return 1;
    }

}
