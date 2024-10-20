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
#include <map>

#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <functional>
struct counter{
    std::string number_counter;
};

struct Tpasport_counter{
    std::string number_counter;
    std::string name_table;
};

struct Ttable_description{
    std::string name_table;
    const size_t number_colon;
    std::vector<std::string>  descriptir_colon;
    std::string to_json() {
        boost::json::object j;
        j["name_table"] = name_table;
        j["number_colon"] = number_colon;
        for(int i=0 ;i<number_colon;i++){
            j["colon"+std::to_string(i+1)] = descriptir_colon.at(i);
        }
        return boost::json::serialize(j);
    }
};

class Tvalue_g {
public:
  virtual int get_value() const = 0; // Виртуальная функция
};
class TvalueImpl : public Tvalue_g {
public:
  int get_value() const override {
    return 42;
  }
};

class Tvalue{
protected:
public:
    boost::json::value js;
    Tvalue(boost::json::value js_):js(js_){}
    virtual bool operator ==(const Tvalue& other) const = 0;
    virtual bool operator < (const Tvalue& other) const = 0;
    virtual bool operator > (const Tvalue& other) const = 0;
    virtual void print() const = 0;
};

class TvalueFactory {
public:
    virtual std::unique_ptr<Tvalue> createValue(boost::json::value js_) const = 0;
};

struct Tdouble_join:public Tvalue{
public:
    Tdouble_join(boost::json::value js_):Tvalue(js_){}
    bool operator ==(const Tvalue& other) const override{
        return js.get_double() == other.js.get_double();
    }
    bool operator < (const Tvalue& other) const override{
        return js.get_double() < other.js.get_double();
    }
    bool operator > (const Tvalue& other) const override{
        return js.get_double() < other.js.get_double();
    }
    void print() const override {
        std::cout << js.get_double() << std::endl;
    }
};

class doubleFactory : public TvalueFactory {
public:
    std::unique_ptr<Tvalue> createValue(boost::json::value js_) const override {
        return std::make_unique<Tdouble_join>(js_);
    }
};

// class Ttime_join:public Tvalue{
//     virtual Tvalue init(const boost::json::value&) const;
//     bool operator ==(const Tvalue& other) const override;
//     bool operator < (const Tvalue& other) const override;
//     virtual void print();
// };

// class timeFactory : public TvalueFactory {
// public:
//     std::unique_ptr<Tvalue> createValue() const override {
//         return std::make_unique<Ttime_join>();
//     }
// };

class TvalueFactorySelector {
private:
    std::map<std::string, std::unique_ptr<TvalueFactory>> factories;

public:
    TvalueFactorySelector() {
        factories["double"] = std::make_unique<doubleFactory>();
     //   factories["time"] = std::make_unique<timeFactory>();
    }

    std::unique_ptr<Tvalue> createValue(const std::string& type,boost::json::value js_) {
        auto it = factories.find(type);
        if (it != factories.end()) {
            return it->second->createValue(js_);
        } else {
            std::cerr << "Неизвестный тип транспортного средства: " << type << std::endl;
            return nullptr; // Или бросить исключение
        }
    }
};

struct Tdescriptor_request_title{
    std::string name_colon;
    std::string type_colon;
};

template<typename T>
struct Tdescriptor_request{
    Tdescriptor_request_title title;
    T upper;
    T lower;
    std::string to_json(){
        boost::json::object j;
        j["name_colon"] = title.name_colon;
        j["type_colon"] = title.type_colon;
        j["upper"] = upper;
        j["lower"] = lower;
        return  boost::json::serialize(j);
    }
};

struct Trecord_to_tbl_counter_hour{
    std::string time;
    double Ap;
    double Am;
    double Qp;
    double Qm;
    // Функция преобразования структуры в строку JSON
    std::string to_json() {
        boost::json::object j;
        j["time"] = time;
        j["Ap"] = Ap;
        j["Am"] = Am;
        j["Qp"] = Qp;
        j["Qm"] = Qm;
        return boost::json::serialize(j);
    }
    // Функция преобразования строки JSON в структуру
    Trecord_to_tbl_counter_hour& from_json(const std::string& json_str) {
        boost::json::value j = boost::json::parse(json_str);
        time = j.at("time").emplace_string(); //get<std::string>();
         Ap = j.at("Ap").emplace_double();
         Ap = j.at("Am").emplace_double();
         Qp = j.at("Qp").emplace_double();
         Qm = j.at("Qm").emplace_double();
        // Am = j["Am"].get<double>();
        // Qp = j["Qp"].get<double>();
        // Qm = j["Qm"].get<double>();
        return *this;
    }
    void print(){
        std::cout << to_json() <<"\n";
    }
};

//const std::string r_1_0_0_98_0_0_255= "\"time\":\"10.10.2024 13:45\",\"Ap\": 0.2344,";

namespace database_cosem {

    enum Database_cosem_Error{DATABASE_COSEM_OK,
                              DATABASE_COSEM_NOT_OPEN=1,
                              DATABASE_COSEM_ELEM_NOT_CREAT =2,
                              DATABASE_COSEM_TBL_ELEM_NOT_CREAT =3,
                              DATABASE_COSEM_ELEM_NOT_DELETE =4,
                             };
    std::string Database_cosem_Error_to_str(Database_cosem_Error err);
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
        virtual Database_cosem_Error delete_table_elem_DB(std::string name_table); // elfktybt отдельной таблици(файла) элемента
        virtual Database_cosem_Error add_record_to_table_elem_DB(std::string name_table,std::string record);
        virtual std::vector<std::string> get_record_from_table_elem_DB(std::string name_table,std::string);
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
        virtual Database_cosem_Error delete_table_elem_DB(std::string name_table){
            std::cerr << "Error delete_table_elem_DB DATABASE_COSEM_ELEM_NOT_CREAT " << name_table << "\n";
            return DATABASE_COSEM_ELEM_NOT_CREAT;
        }
        virtual Database_cosem_Error add_record_to_table_elem_DB(std::string name_table,std::string record){
        std::cerr << "Error delete_table_elem_DB DATABASE_COSEM_ELEM_NOT_CREAT " << name_table << "\n";
        return DATABASE_COSEM_ELEM_NOT_CREAT;
        }
        virtual std::vector<std::string> get_records_from_table_elem_DB(std::string name_table,std::string){
            std::cerr << "Error get_record_from_table_elem_DB DATABASE_COSEM_ELEM_NOT_CREAT " << name_table << "\n";
            return std::vector<std::string>{};
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



