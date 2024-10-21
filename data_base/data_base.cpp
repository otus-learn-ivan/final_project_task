
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
struct Tin_out_direct
{
    boost::filesystem::path old_path;
    Tin_out_direct(std::string next_direct):old_path(boost::filesystem::current_path()) {
        boost::filesystem::current_path(next_direct.c_str());
    }
    ~Tin_out_direct(){
        boost::filesystem::current_path(old_path);
    }
};

Database_cosem_Error Telem_DB::create_table_elem_DB(std::string name_table){ // создание отдельной таблици(файла) элемента
    boost::filesystem::path file_path = name_table + ".tbl" ;
    std::cout <<"create_table_elem_DB "  << name_elem.c_str() << " \\ " << file_path.string() << "\n";
    try {
        Tin_out_direct go_to_dir {name_elem};
        //std::cout << "Создаем файл, если он не существует\n";
        if (!boost::filesystem::exists(file_path)) {
          std::ofstream (file_path.string());
        }
    } catch (const std::exception& ex) {
      std::cerr << "ERROR Ошибка создания файла: " << ex.what() << std::endl;
      return DATABASE_COSEM_TBL_ELEM_NOT_CREAT;
    }
    return DATABASE_COSEM_OK;
}

Database_cosem_Error Telem_DB::delete_table_elem_DB(std::string name_table){ // удаление отдельной таблици(файла) элемента
    boost::filesystem::path file_path = name_table + ".tbl" ;
    std::cout <<"create_table_elem_DB "  << name_elem.c_str() << " \\ " << file_path.string() << "\n";
    try {
        Tin_out_direct go_to_dir {name_elem};
        if (boost::filesystem::exists(file_path)) {
          std::cout << "Удаляем файл, если он не существует\n";
          boost::filesystem::remove (file_path.string());
        }
    } catch (const std::exception& ex) {
      std::cerr << "ERROR Ошибка удаления файла: " << ex.what() << std::endl;
      return DATABASE_COSEM_TBL_ELEM_NOT_CREAT;
    }
    return DATABASE_COSEM_OK;
}

#include <fstream>
Database_cosem_Error Telem_DB::add_record_to_table_elem_DB(std::string name_table,std::string record){ // удаление отдельной таблици(файла) элемента
    boost::filesystem::path file_path = name_table + ".tbl" ;
    std::cout <<"add_record_to_table_elem_DB "  << name_elem.c_str() << " \\ " << file_path.string() << "\n";
    try {
        Tin_out_direct go_to_dir {name_elem};
        if (boost::filesystem::exists(file_path)) {
          std::ofstream file_(file_path,std::ios::app);
          if(file_.is_open()){
              file_ << record<<"\n";
              file_.close();
          }
        }
    } catch (const std::exception& ex) {
      std::cerr << "ERROR add_record_to_table_elem_DB: " << ex.what() << std::endl;
      return DATABASE_COSEM_TBL_ELEM_NOT_CREAT;
    }
    return DATABASE_COSEM_OK;
}

struct Test_record_for_get{
    std::string& descriptor_request;
    std::string& name_table;
    std::vector<std::string>& out;
    Test_record_for_get (std::string& descriptor_r,std::string& name_t, std::vector<std::string>& out_v):
        descriptor_request(descriptor_r),name_table(name_t),out(out_v){};
    void operator()(std::string& line){
        if(descriptor_request==""){
             out.push_back(line);
            return;
        }

        boost::json::value j = boost::json::parse(descriptor_request);
        std::string name = std::string(j.at("name_colon").get_string());
        std::string type_ =std::string(j.at("type_colon").get_string());

        auto val = TvalueFactorySelector().createValue(type_,boost::json::parse(line).at(name));
        auto upper = TvalueFactorySelector().createValue(type_,j.at("upper"));
        auto lover = TvalueFactorySelector().createValue(type_,j.at("lower"));

        if(*upper == *lover){
            if(*val == *upper){
                out.push_back(line);
            }
        }else if( ((*lover < *val) && (*val< *upper))|| (*lover == *val)||(*upper == *val)){
            out.push_back(line);
        }
    }
};

std::vector<std::string> Telem_DB::get_record_from_table_elem_DB(std::string name_table,std::string descriptor_request){ // удаление отдельной таблици(файла) элемента
     boost::filesystem::path file_path = name_table + ".tbl" ;
     std::cout <<"get_record_from_table_elem_DB "  << name_elem.c_str() << " \\ " << file_path.string() << "\n";
     try {
         Tin_out_direct go_to_dir {name_elem};
         if (boost::filesystem::exists(file_path)) {
             std::ifstream inputFile(file_path);
             if (!inputFile.is_open()) {
                 return std::vector<std::string>{};
             }
             std::string line;
             std::vector<std::string> out;
             while (std::getline(inputFile, line)) {
                Test_record_for_get(descriptor_request,name_table,out)(line);// out.push_back(line);
             }
             return out;
         }
     } catch (const std::exception& ex) {
       std::cerr << "ERROR add_record_to_table_elem_DB: " << ex.what() << std::endl;
       return std::vector<std::string>{};// DATABASE_COSEM_TBL_ELEM_NOT_CREAT;
     }
    // return DATABASE_COSEM_OK;
    return std::vector<std::string>{};
}

//--------------------------------------------

std::map <Database_cosem_Error,std::string> error_to_str{
    {DATABASE_COSEM_OK,"DATABASE_COSEM_OK"},
    {DATABASE_COSEM_NOT_OPEN,"DATABASE_COSEM_NOT_OPEN"},
    {DATABASE_COSEM_ELEM_NOT_CREAT,"DATABASE_COSEM_ELEM_NOT_CREAT"},
    {DATABASE_COSEM_TBL_ELEM_NOT_CREAT,"DATABASE_COSEM_TBL_ELEM_NOT_CREAT"},
    {DATABASE_COSEM_ELEM_NOT_DELETE,"DATABASE_COSEM_ELEM_NOT_DELETE"}
};

std::string Database_cosem_Error_to_str(Database_cosem_Error err){
    return error_to_str[err];
}


}

//bool operator <(database_cosem::Telem_DB first,database_cosem::Telem_DB second){
//    return first.name_elem < second.name_elem;
//}

