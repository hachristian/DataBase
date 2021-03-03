#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

class DataBase
{
    public:
        //constructor and destructor
        DataBase();
        ~DataBase();

        bool newDataBase(std::string name);
        bool createData();
        bool openDataBase(std::string name);
        bool closeDataBase();
        bool displayRecord(const int record);
        bool updateRecord(const int record, int field, std::string change);
        bool addRecord(const int record);
        bool deleteRecord(const int record);
        int getSize();
        void createReport();

    private:
        std::ifstream din;
        std::ofstream dout;
        int NumRecord, RecordSize, IdSize, RegionSize, StateSize, CodeSize, NameSize, TypeSize, VistorsSize;
        char Divider;
        std::string DataBaseName;

        //private methods
        bool getRecord(const int index, std::string &id, std::string &region, std::string &state, std::string &code, std::string &name, std::string &type, std::string &visitors);
        std::string fixStr(std::string id, std::string region, std::string state, std::string code, std::string name, std::string type, std::string visitors);
        int binarySearch(const int recordNum, std::string &id, std::string &region, std::string &state, std::string &code, std::string &name, std::string &type, std::string &visitors);
        void getInfo(std::string &region, std::string &state, std::string &code, std::string &name, std::string &type, std::string &visitors);
        void insertBlank();

};

#endif
