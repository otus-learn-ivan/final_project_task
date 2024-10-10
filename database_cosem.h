#ifndef DATABASE_COSEM_H
#define DATABASE_COSEM_H

#include <string>
#include <memory>
#include <vector>
#include <memory>
struct counter{
    std::string number_counter;
};


namespace database_cosem {

    enum Database_cosem_Error{DATABASE_COSEM_OK,
                              DATABASE_COSEM_NOT_OPEN=1,
                              DATABASE_COSEM_ELEM_NOT_CREAT =2,
                              DATABASE_COSEM_TBL_ELEM_NOT_CREAT =3
                             };
    struct Telem_DB{
        std::string name_elem;
        Telem_DB(std::string name_elem):name_elem{name_elem}{}
        virtual Database_cosem_Error create_elem_DB(){ //создание каталога элемента
            return DATABASE_COSEM_ELEM_NOT_CREAT;
        }
        virtual Database_cosem_Error delete_elem_DB(){ //создание каталога элемента
            return DATABASE_COSEM_ELEM_NOT_CREAT;
        }
        virtual Database_cosem_Error create_table_elem_DB(std::string name_tadle){ // создание отдельной таблици(файла) элемента
            return DATABASE_COSEM_TBL_ELEM_NOT_CREAT;
        }
        virtual Database_cosem_Error delete_table_elem_DB(std::string name_fable){ // создание отдельной таблици(файла) элемента
            return DATABASE_COSEM_TBL_ELEM_NOT_CREAT;
        }
        friend bool operator <(database_cosem::Telem_DB& first,database_cosem::Telem_DB& second){
            return first.name_elem < second.name_elem;
        }
    };

    class DB{
        std::string path_to_DB;
        std::unique_ptr<std::vector<Telem_DB>> elems_DB;
        std::string create_full_path_to_elem_DB(std::string name_elem_DB);
    public:
        DB(std::string path):path_to_DB(path){
            elems_DB = std::make_unique<std::vector<Telem_DB>>();
        }
        Database_cosem_Error open_database();// путь к катологу database
        Database_cosem_Error add_elem_DB(std::string name_elem_DB); // добавляет католог счетчика в базу
        Database_cosem_Error delete_elem_DB(std::string name_elem_DB); // добавляет католог счетчика в базу
        Database_cosem_Error get_list_elem_DB(); // добавляет католог счетчика в базу
        //std::unique_ptr<DB> create_DB(path);
    };
}



//struct Tcounter:public database_cosem::Telem_DB{
//    Tcounter(std::string name_elem):Telem_DB{name_elem}{}
//      database_cosem::Database_cosem_Error create_elem_DB(std::string name_elem) override; //создание каталога элемента
//      virtual Database_cosem_Error delete_elem_DB(){ //создание каталога элемента
//          return DATABASE_COSEM_ELEM_NOT_CREAT;
//      }
//      virtual Database_cosem_Error create_table_elem_DB(std::string name_tadle){ // создание отдельной таблици(файла) элемента
//          return DATABASE_COSEM_TBL_ELEM_NOT_CREAT;
//      }
//      virtual Database_cosem_Error delete_table_elem_DB(std::string name_fable){ // создание отдельной таблици(файла) элемента
//          return DATABASE_COSEM_TBL_ELEM_NOT_CREAT;
//      }
//};
#endif // DATABASE_COSEM_H



