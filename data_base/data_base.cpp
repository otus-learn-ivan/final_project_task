
#include "../database_cosem.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
namespace database_cosem {

std::string DB::create_full_path_to_elem_DB(std::string name_elem_DB){
    return path_to_DB +"/"+name_elem_DB;
}

//Если папка базы данных отсутствует то создается если папка уже существует то заполняется вектор существующих счетчиков
Database_cosem_Error DB::open_database(){
    DIR *dir;
       struct dirent *ent;
       dir = opendir(this->path_to_DB.c_str()); // "." - текущая директория

       if (dir != nullptr) {
           while ((ent = readdir(dir)) != nullptr) {
               if (ent->d_type == DT_DIR) {
                   this->elems_DB->push_back(Telem_DB{create_full_path_to_elem_DB(ent->d_name)}); // Добавляем директорию счетчика  в вектор
               }
           }
           closedir(dir);
       } else {
           if (mkdir(this->path_to_DB.c_str(), 0755) != 0) {
            // Обработка ошибки
            std::cerr << "Ошибка открытия директории\n";
            return Database_cosem_Error::DATABASE_COSEM_NOT_OPEN;
       }
       std::sort(this->elems_DB->begin(),this->elems_DB->end());
    }
    return Database_cosem_Error::DATABASE_COSEM_OK;
}

Database_cosem_Error DB::add_elem_DB(std::string name_elem_DB){
   auto iter = std::find_if(this->elems_DB->begin(),this->elems_DB->end(),[&name_elem_DB](const auto& el){
        return el.name_elem == name_elem_DB;
    }
   );
    if(iter==this->elems_DB->end()){
       Telem_DB el{create_full_path_to_elem_DB(name_elem_DB)};
       auto answer = el.create_elem_DB();
       if(!answer)this->elems_DB->push_back(el);
       return answer;
    }
    return DATABASE_COSEM_OK;
}

}

//bool operator <(database_cosem::Telem_DB first,database_cosem::Telem_DB second){
//    return first.name_elem < second.name_elem;
//}

