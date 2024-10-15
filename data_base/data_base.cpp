
#include "../database_cosem.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>
namespace database_cosem {

Telem_DB_empty empty_elem_DB{};

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
                   if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                        continue;
                    }
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

decltype(std::declval<std::deque<Telem_DB>>().begin()) DB::find_elem_in_DB(std::string name_elem_DB){
    auto full_name =  create_full_path_to_elem_DB(name_elem_DB);
    auto iter = std::find_if(this->elems_DB->begin(),this->elems_DB->end(),[&full_name](const auto& el){
         return el.name_elem == full_name;
     }
    );
    return iter;
}
Database_cosem_Error DB::add_elem_DB(std::string name_elem_DB){
    auto elm = find_elem_in_DB(name_elem_DB);
    if(elm==elems_DB->end()){
        Telem_DB el{create_full_path_to_elem_DB(name_elem_DB)};
        auto answer = el.create_elem_DB();
        if(!answer)this->elems_DB->push_back(el);
        return answer;
    }
    return DATABASE_COSEM_OK;
}

Database_cosem_Error DB::delete_elem_DB(std::string name_elem_DB){
    auto elm = find_elem_in_DB(name_elem_DB);
    if(elm!=elems_DB->end()){
        elm->delete_elem_DB();
        this->elems_DB->erase(elm);
    }
    return DATABASE_COSEM_OK;
}
std::string DB::get_list_elems_DB(){
    std::stringstream  name_elems_DB;
    for(auto const & elem:*elems_DB){
        name_elems_DB << (elem.name_elem +" \n");
    }
    return name_elems_DB.str();
}
Database_cosem_Error DB::delete_DB(){
    boost::filesystem::path directory_path(path_to_DB);
    if (!boost::filesystem::remove_all(directory_path)) {
        return DATABASE_COSEM_ELEM_NOT_DELETE;
    }
    return DATABASE_COSEM_OK;
}

Database_cosem_Error Telem_DB::create_table_elem_DB(std::string name_table){ // создание отдельной таблици(файла) элемента
    boost::filesystem::path file_path = name_elem + "\\" + name_table + ".tdl" ;
    std::cout <<"create_table_elem_DB " << file_path.string() << "\n";
    try {
      // Создаем файл, если он не существует
      if (!boost::filesystem::exists(file_path)) {
        std::ofstream file(file_path.string());
      }
    } catch (const std::exception& ex) {
      std::cerr << "Ошибка создания файла: " << ex.what() << std::endl;
      return DATABASE_COSEM_TBL_ELEM_NOT_CREAT;
    }
    return DATABASE_COSEM_OK;
}


}

//bool operator <(database_cosem::Telem_DB first,database_cosem::Telem_DB second){
//    return first.name_elem < second.name_elem;
//}

