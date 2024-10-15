#ifndef DATABASE_COSEM_H
#define DATABASE_COSEM_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <memory>
#include <dirent.h>
#include <sys/stat.h>
#include <boost/json.hpp>
#include <boost/filesystem.hpp>

struct counter{
    std::string number_counter;
};


namespace database_cosem {

    enum Database_cosem_Error{DATABASE_COSEM_OK,
                              DATABASE_COSEM_NOT_OPEN=1,
                              DATABASE_COSEM_ELEM_NOT_CREAT =2,
                              DATABASE_COSEM_TBL_ELEM_NOT_CREAT =3,
                              DATABASE_COSEM_ELEM_NOT_DELETE =4,
                             };
    struct Telem_DB{
        std::string name_elem;
        //Telem_DB():name_elem(""){}
        Telem_DB(std::string name_elem):name_elem{name_elem}{}
        virtual Database_cosem_Error create_elem_DB(){ //создание каталога элемента
           DIR *dir;
           dir = opendir(name_elem.c_str()); // "." - текущая директория
           if (dir == nullptr) {
               if (mkdir(name_elem.c_str(), 0755) != 0) {
                // Обработка ошибки
                    std::cerr << "Ошибка открытия директории\n";
                    return DATABASE_COSEM_ELEM_NOT_CREAT;
                }
            }
           return DATABASE_COSEM_OK;
        }
        virtual Database_cosem_Error delete_elem_DB(){ //создание каталога элемента
            boost::filesystem::path directory_path(name_elem.c_str());
            if (!boost::filesystem::remove_all(directory_path)) {
                return DATABASE_COSEM_ELEM_NOT_DELETE;
            }
            return DATABASE_COSEM_OK;
        }
        virtual Database_cosem_Error create_table_elem_DB(std::string name_tadle); // создание отдельной таблици(файла) элемента
        virtual Database_cosem_Error delete_table_elem_DB(std::string name_table){ // создание отдельной таблици(файла) элемента
            return DATABASE_COSEM_TBL_ELEM_NOT_CREAT;
        }
        friend bool operator <(database_cosem::Telem_DB& first,database_cosem::Telem_DB& second){
            return first.name_elem < second.name_elem;
        }
    };

    struct Telem_DB_empty:public Telem_DB{
        Telem_DB_empty():Telem_DB(""){}
        virtual Database_cosem_Error create_elem_DB(){ //создание каталога элемента
            std::cerr << "Error create_elem_DB DATABASE_COSEM_ELEM_NOT_CREAT\n";
            return DATABASE_COSEM_ELEM_NOT_CREAT;
        }
        virtual Database_cosem_Error delete_elem_DB(){ //создание каталога элемента
            std::cerr << "Error delete_elem_DB DATABASE_COSEM_ELEM_NOT_CREAT\n";
            return DATABASE_COSEM_ELEM_NOT_CREAT;
        }
        virtual Database_cosem_Error create_table_elem_DB(std::string name_table){ // создание отдельной таблици(файла) элемента
            std::cerr << "Error create_table_elem_DB DATABASE_COSEM_ELEM_NOT_CREAT " << name_table << "\n";
            return DATABASE_COSEM_ELEM_NOT_CREAT;
        }
        virtual Database_cosem_Error delete_table_elem_DB(std::string name_table){ // создание отдельной таблици(файла) элемента
            std::cerr << "Error delete_table_elem_DB DATABASE_COSEM_ELEM_NOT_CREAT " << name_table << "\n";
            return DATABASE_COSEM_ELEM_NOT_CREAT;
        }
    };

    extern  Telem_DB_empty empty_elem_DB;

    class DB{
        std::string path_to_DB;
        // std::unique_ptr<std::vector<Telem_DB>> elems_DB;
        std::unique_ptr<std::deque<Telem_DB>> elems_DB;
        std::string create_full_path_to_elem_DB(std::string name_elem_DB);
        decltype(std::declval<std::deque<Telem_DB>>().begin()) find_elem_in_DB(std::string name_elem_DB);
    public:
        DB(std::string path):path_to_DB(path){
            elems_DB = std::make_unique<std::deque<Telem_DB>>();
//            elems_DB = std::make_unique<std::vector<Telem_DB>>();
        }
        Database_cosem_Error open_database();// путь к катологу database
        Database_cosem_Error add_elem_DB(std::string name_elem_DB); // добавляет католог счетчика в базу
        Database_cosem_Error delete_elem_DB(std::string name_elem_DB); // удаляет католог счетчика из базы
        Database_cosem_Error delete_DB(); // удаляет католог счетчика из базы
        std::string get_list_elems_DB(); // добавляет католог счетчика в базу
        Telem_DB& get_elem_in_DB(std::string name_elem_DB){
            auto it = find_elem_in_DB(name_elem_DB);
            if(it == elems_DB->end()){
                return empty_elem_DB;
            }
            return *it;
        };
        void print_passport_DB_log(){
            std::cout << "start\n";
            std::cout <<  "path_to_DB - " << path_to_DB << std::endl;
            std::cout <<  "size elems_DB - " << elems_DB->size() << std::endl;
            for(const auto& e:*elems_DB ){
                std::cout << e.name_elem <<"\n";
            }
            std::cout << "end\n";
        }

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



