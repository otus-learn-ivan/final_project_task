#ifndef DATABASE_COSEM_H
#define DATABASE_COSEM_H

#include <string>
#include <memory>
#include <vector>

namespace database_cosem {

    enum Database_cosem_Error{DATABASE_COSEM_OK,
                              DATABASE_COSEM_NOT_OPEN=1};

    class DB{
        std::string path_to_DB;
        std::vector<std::string> path_to_dir_pu;
    public:
        DB(std::string path):path_to_DB(path),path_to_dir_pu{}{}
        Database_cosem_Error open_database();// путь к катологу database
        //Database_cosem_Error add_PU(std::string number_pu); // добавляет католог счетчика в базу
        //std::unique_ptr<DB> create_DB(path);
    };


}

#endif // DATABASE_COSEM_H



