Тема: NoSQL база данных. Для хранения показаний счетчиков электрической энергии в формате DLMS/COSEM

Стандарт языка с++ 20 версия (std::span)
Библиотека boost ( в частности boost::asio, boost::json, boost::thread)

Результатом проекта тала программа создающая локальный сервер обрабатывающая следующие команды:
- создание/удаление отдельной база данных (БД).
- создание/удаление отдельного ПУ в БД .
- сохранение/выдача измеренных данных ПУ (параметры запроса выдаваемые данные в формате json)

Развитие проекта .
Разработанный функционал является только прототипом и основой реально функционирующего приложения.
Для его реального функционирования необходимо реализовать значительное число функций: 
 - Система управления доступом 
 - Система прямого опроса ПУ на основе сервера GURUX
 - Система mapper/reducer для пред обработки данных самим сервером.

   В проекте в целом использовались знания полученные в
   процессе обучения что позволило достичь поставленных целей
   
Основные команды
    Старт приложения - ./using_database 9000  - открытие сепрвера приложения на локальном порте 9000
    
    Create_DB - создание базыданных    
    Add_PU - добавление счетчика электроэнергии (прибор учёта далее ПУ) в базу данных
    Delete_PU - удаление счетчика электроэнергии из базы данных
    Create_table_PU - создание таблици хранения показаний ПУ
    Delete_table_PU - удаление таблици хранения показаний ПУ
    Add_record_to_table_PU - добавить запись в таблицу 
    Get_record_from_table_PU - получить данные из таблици
    
   Пример команды чтения показаний ПУ:
   Запрос по величине показаний
   Get_record_from_table_PU Test_counters_base 456798798744434 0.0.98.1.0.255 {"name_colon":"Ap","type_colon":"double","upper":1.07E1,"lower":1.05E1}
   
   
   где 
   
    Get_record_from_table_PU -команда
    Test_counters_base - имя базыданных    
    456798798744434 - номер ПУ
    0.0.98.1.0.255 - таблица хранения
    {"name_colon":"Ap","type_colon":"double","upper":1.07E1,"lower":1.05E1} - join строка запроса.
    
   Запрос за период
   
   Get_record_from_table_PU Test_counters_base 456798798744434 0.0.98.1.0.255 {"name_colon":"time","type_colon":"time","upper":"2023-10-27 14:00:00","lower":"2023-10-26 13:00:00"}
   
   где
   
   Get_record_from_table_PU
   Test_counters_base
   456798798744434
   0.0.98.1.0.255
   {"name_colon":"time","type_colon":"time","upper":"2023-10-27 14:00:00","lower":"2023-10-26 13:00:00"}
