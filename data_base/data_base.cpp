
#include "../database_cosem.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <algorithm>
namespace database_cosem {


//Если папка базы данных отсутствует то создается если папка уже существует то заполняется вектор существующих счетчиков
Database_cosem_Error DB::open_database(){
    DIR *dir;
       struct dirent *ent;
       dir = opendir(this->path_to_DB.c_str()); // "." - текущая директория
       std::vector<std::string> ;

       if (dir != nullptr) {
           while ((ent = readdir(dir)) != nullptr) {
               if (ent->d_type == DT_DIR) {
                   this->path_to_dir_pu.push_back(ent->d_name); // Добавляем директорию счетчика  в вектор
               }
           }
           closedir(dir);
       } else {
           if (mkdir(this->path_to_DB.c_str(), 0755) != 0) {
            // Обработка ошибки
            std::cerr << "Ошибка открытия директории\n";
            return Database_cosem_Error::DATABASE_COSEM_NOT_OPEN;
       }
       std::sort(this->path_to_dir_pu.begin(),this->path_to_dir_pu.end());
    }
    return Database_cosem_Error::DATABASE_COSEM_OK;
}

}
