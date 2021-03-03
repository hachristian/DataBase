#include "DataBase.h"

int main()
{
    DataBase dataBase;
    std::string dataBaseName = "";

    bool quit = false;
    bool isopen = false;
    int answer = 0;

    std::cout << "welcome! what would you like to do?\n\n";

    while (!quit)
    {
        std::cout << "\n[1] Create New Database \n";
        std::cout << "[2] Open Database \n";
        std::cout << "[3] Close Database \n";
        std::cout << "[4] Display Record \n";
        std::cout << "[5] Update Record \n";
        std::cout << "[6] Create Report \n";
        std::cout << "[7] Add Record \n";
        std::cout << "[8] Delete Record \n";
        std::cout << "[9] Quit\n";

        std::cin >> answer;

        if (answer == 1)
        {
            bool success = false;

            std::cout << "please enter the database name (Must match .csv file) ";
            std::cin >> dataBaseName;

            success = dataBase.newDataBase(dataBaseName);
            if (!success)
                std::cout << dataBaseName  << ".csv does not exist\n";
            std::cout << "DataBase " << dataBaseName << " has been created.\n";
        }
        else if (answer == 2)
        {
            if (!isopen)
            {
                std::string name = "";
                bool success = false;
                std::cout << "Please enter the Database name you want to open.\n";
                std::cin >> name;
                std::cout << "opening " << dataBaseName << "...\n";
                success = dataBase.openDataBase(name);
                if (success)
                    isopen = true;
            }
            else
                std::cout << "A data base is already open!\n";
        }
        else if (answer == 3)
        {
            std::cout << dataBaseName << " has been closed.\n";
            dataBase.closeDataBase();
            isopen = false;
        }
        else if (answer == 4)
        {
            if (isopen)
            {
                int record;
                std::cout << "Please insert the record number to display.\n";
                std::cin >> record;
                dataBase.displayRecord(record);
            }
            else
                std::cout << "A data base is not open!\n";
        }
        else if (answer == 5)
        {
            if (isopen)
            {
                int record;
                int field;
                std::string change = "";
                bool correct = false;
                std::cout << "Please insert the record number to update.\n";
                std::cin >> record;
                std::cout << "which of the following fields will you update?\n";

                while (!correct)
                {
                    std::cout << "\n\n[1] Region\n";
                    std::cout << "[2] State\n";
                    std::cout << "[3] Code\n";
                    std::cout << "[4] Name\n";
                    std::cout << "[5] Type\n";
                    std::cout << "[6] Visitors\n";

                    std::cin >> field;

                    if (field < 1 || field > 6)
                        std::cout << "Please choose a number between 1 and 6\n";
                    else
                        correct = true;
                }
                std::cout << "type the change now: ";
                //std::ws consumes white space
                std::getline(std::cin >> std::ws, change);

                dataBase.updateRecord(record, field, change);
            }
            else
                std::cout << "A data base is not open!\n\n";
        }
        else if (answer == 6)
        {
            if (isopen)
            {
                std::cout << "Creating report... \n\n";
                dataBase.createReport();
            }
            else
                std::cout << "A data base is not open!\n\n";
        }
        else if (answer == 7)
        {
            if (isopen)
            {
                int record;
                std::cout << "Please insert the record number to add.\n";
                std::cin >> record;
                if (record < 0 || record > 9999999)
                {
                    std::cout << "invalid range a 7 digit record is the max\n";
                    std::cin.clear();
                }

                else
                    dataBase.addRecord(record);
            }
            else
                std::cout << "A data base is not open!\n\n";
        }
        else if (answer == 8)
        {
            if (isopen)
            {
                int record;
                std::cout << "Please insert the record number to delete.\n";
                std::cin >> record;
                dataBase.deleteRecord(record);
            }
            else
                std::cout << "A data base is not open!\n";
        }
        else if (answer == 9)
        {
            std::cout << "Goodbye!\n";
            quit = true;
        }
        else
            std::cout << "Please enter a number between 1 and 9.\n\n";
    }

    //dataBase.displayRecord(1083940);
    return 0;
}
