#include "DataBase.h"

DataBase::DataBase()
{
    NumRecord = 0;
    //for some reason record size is 151 if you compile on linux....
    RecordSize = 152;
    IdSize = 7;
    RegionSize = 2;
    StateSize = 2;
    CodeSize = 4;
    NameSize = 83;
    TypeSize = 37;
    VistorsSize = 9;
    //able to change the divider to any char you want
    Divider = ' ';
    DataBaseName = " ";
}

DataBase::~DataBase()
{
    // making sure the files are closed when finished with program.
    if (din.is_open())
        din.close();
    if (dout.is_open())
        dout.close();
}

bool DataBase::newDataBase(std::string dataBaseName)
{
    //updating private variable for other methods
    DataBaseName = dataBaseName;

    //adding .csv to access the file
    std::string temp = dataBaseName + ".csv";
    if (din.is_open())
    {
        std::cout << "there is a database currently open please close it.\n";
        return false;
    }
    else
    {
        din.open(temp.c_str());
        if (!din)
        { 
            std::cout << "Could not open " << temp << std::endl;
            return false;
        }
    }

    temp = dataBaseName + ".data";
    dout.open(temp.c_str());
    //creates a formatted document
    createData();
    dout.close();

    //writing the number of records  to config
    temp = dataBaseName + ".config";
    dout.open(temp.c_str());
    dout << NumRecord;
    dout.close();
    //closes .csv file
    din.close();

    return true;
}

bool DataBase::createData()
{
    std::string str = "";
    std::string temp = "";
    int count = 0;
    int charCount = 0;
    bool inQuote = false;
    //reminder to switch to \n when compiling in linux
    getline(din, str, '\r');

    //reads a line from the csv file and outputs the correct format to file
    while (getline(din,str,'\r'))
    {
        //adds a comma so i can tell when the last field ends
        str += ',';
        //fixes the string to output to file
        for (unsigned int i = 0; i < str.length(); i++)
        {
            if (str[i] != ',')
            {
                if (str[i] == '\"')
                {
                    if (inQuote == false)
                        inQuote = true;
                    else 
                        inQuote = false;
                }
                //makes sure there are no etra lines 
                if (str[i] != '\r' && str[i] != '\n')
                {
                    // makes these fields into one token
                    if ((count == 4 || count == 5) && str[i] == ' ')
                    {
                        temp += '_';
                        charCount++;
                    }
                    else
                    {
                        temp += str[i];
                        charCount++;    
                    }
                }
            }
            else
            {
                if (!inQuote)
                {
                    if (count == 0)
                    {
                        for (int j = 0; j < IdSize - charCount; j++)
                            temp += Divider;

                        //adds space
                        temp += Divider;
                    }
                    else if (count == 1)
                    {
                        for (int j = 0; j < RegionSize - charCount; j++)
                            temp += Divider;

                        //adds space
                        temp += Divider;
                    }
                    else if (count == 2)
                    {
                        for (int j = 0; j < StateSize - charCount; j++)
                            temp += Divider;

                        //adds space
                        temp += Divider;
                    }
                    else if (count == 3)
                    {
                        for (int j = 0; j < CodeSize - charCount; j++)
                            temp += Divider;

                        //adds space
                        temp += Divider;
                    }
                    else if (count == 4)
                    {
                        if (!inQuote)
                            for (int j = 0; j < NameSize - charCount; j++)
                                temp += Divider;

                        //adds space
                        temp += Divider;
                    }
                    else if (count == 5)
                    {
                        for (int j = 0; j < TypeSize - charCount; j++)
                            temp += Divider;

                        //adds space
                        temp += Divider;
                    }
                    else if (count == 6)
                    {
                        for (int j = 0; j < VistorsSize - charCount; j++)
                            temp += Divider;
                    }

                    charCount = 0;
                    count++;
                }
            }
        }
        dout << temp << std::endl;
        NumRecord++;
        insertBlank();
        NumRecord++;
        temp.clear();
        count = 0;
    }

    return true;
}

bool DataBase::openDataBase(std::string name)
{
    DataBaseName = name;
     //opens data file.
    std::string temp = DataBaseName + ".data";

    if (din.is_open())
    {
        std::cout << "there is a database currently open please close it.\n";
        return false;
    }
    else
    {
        din.open(temp.c_str());
        if (!din)
        { 
            std::cout << "Could not open " << temp << std::endl;
            return false;
        }
    }
    return true;
}

bool DataBase::getRecord(const int index, std::string &id, std::string &region, std::string &state, std::string &code, std::string &name, std::string &type, std::string &visitors)
{
    //clears dins flag if any
    din.clear();
    if ((index >= 0) && (index < NumRecord))
    {
        din.seekg(index * RecordSize, std::ios::beg);
        din >> id >> region >> state >> code >> name >> type >> visitors;
        return true;
    }

    return false;
}

int DataBase::binarySearch(const int recordNum, std::string &id, std::string &region, std::string &state, std::string &code, std::string &name, std::string &type, std::string &visitors)
{
    int low = 0;
    int high = NumRecord - 1;
    int middle;
    int prevBlank = 0;
    std::string middleId;
    bool failure = false;

    bool found = false;
    bool blankFound = false;
    while (!found && (high >= low) && !failure)
    {
        if (!blankFound)
        {
            middle = (low + high) / 2;
            
            if (middle == prevBlank)
                return prevBlank * -1;
        }
        if (getRecord(middle, middleId, region, state, code, name, type, visitors))
        {
            if (middleId[0] == 'i')
            {
                prevBlank = middle;
                if (middle == NumRecord - 1)
                    return -middle;
                middle = (middle + 1) % NumRecord;
                blankFound = true;
            }
            else
            {
                blankFound = false;
                // easier to compare ints rather than strings
                std::cout << "middle id length = " << middleId.length() << std::endl;
                std::cout << "middle id = " << middleId << std::endl;
                std::cout << "index = " << middle << std::endl;
                int temp = stoi(middleId);
                if (temp == recordNum)
                {
                    id = middleId;
                    found = true;
                }
                else if (temp < recordNum)
                {
                    low = middle + 1;
                }
                else
                {
                    high = middle - 1;
                }
            }
        }
        else
        {
            std::cout << "Could not get record " << recordNum << std::endl;
            failure = true;
        }
    }
    //making sure id is updated.
    id = middleId;
    std::cout << "index = " << middle << std::endl;
    if (found)
        return middle;

    if (middle == 0)
        return -(NumRecord * 2);
    
    return middle * -1;
}

bool DataBase::closeDataBase()
{
    if (din.is_open())
    {
        din.close();
        return true;
    }
    else
        std::cout << "There is no dataBase currently open." << std::endl;
   
    return true;
}

bool DataBase::displayRecord(const int record)
{
    int index = 0;
    std::string id = "", region = "", state = "", code = "", name = "", type = "", visitors = "";
    //getRecord(record, id, region, state, code, name, type, visitors);
    index = binarySearch(record, id, region, state, code, name, type, visitors);

    //fixing name and type
    for (unsigned int i = 0; i < name.length(); i++)
        if (name[i] == '_')
            name[i] = ' ';

   for (unsigned int i = 0; i < type.length(); i++)
        if (type[i] == '_')
            type[i] = ' ';

    if (index >= 0)
        std::cout << "ID: " << id << std::endl << "Region: " << region << std::endl << "State: " << state << std::endl
             << "Code: " << code << std::endl << "Name: " << name << std::endl << "Type: " << type << std::endl << "Visitors: " << visitors << std::endl;
    else
        std::cout << "Record " << id << " was not found.\n"; 

    return true;
}

bool DataBase::updateRecord(const int record, int field, std::string change)
{
    int index = 0;
    std::string id = "", region = "", state = "", code = "", name = "", type = "", visitors = "";

    index = binarySearch(record, id, region, state, code, name, type, visitors);

    if (index >= 0)
        std::cout << "ID: " << id << std::endl << "Region: " << region << std::endl << "State: " << state << std::endl
             << "Code: " << code << std::endl << "Name: " << name << std::endl << "Type: " << type << std::endl << "Visitors: " << visitors << std::endl;
    else
    {
        std::cout << "Record " << id << " was not found.\n";
        return false;
    }


    if (field == 1)
    {
        //makes sure change is the right size.
        if (change.length() > (unsigned int)RegionSize)
            change.resize(RegionSize);
        
        std::cout << "Region has been changed to " << change << std::endl;
        region = change;
    }
    else if (field == 2)
    {
        //makes sure change is the right size.
        if (change.length() > (unsigned int)StateSize)
            change.resize(StateSize);
        
        std::cout << "State has been changed to " << change << std::endl;
        state = change;
        
    }
    else if (field == 3)
    {
        //makes sure change is the right size.
        if (change.length() > (unsigned int)CodeSize)
            change.resize(CodeSize);
        
        std::cout << "Code has been changed to " << change << std::endl;
        code = change;
    }
    else if (field == 4)
    {
        //makes sure change is the right size.
        if (change.length() > (unsigned int)NameSize)
            change.resize(NameSize);

        // changing spaces to _
        for (unsigned int i = 0; i < change.length(); i++)
            if (change[i] == ' ')
                change[i] = '_';

        //name.resize(change.length());
        std::cout << "Name has been changed to " << change << std::endl;
        name = change;
    }
    else if (field == 5)
    {
        //makes sure change is the right size.
        if ((int)change.length() > TypeSize)
            change.resize(TypeSize);

        // changing spaces to _
        for (unsigned int i = 0; i < change.length(); i++)
            if (change[i] == ' ')
                change[i] = '_';

        std::cout << "Type has been changed to " << change << std::endl;
        type = change;
    }
    else if (field == 6)
    {
        //makes sure change is the right size
        if (change.length() > (unsigned)VistorsSize)
            change.resize(VistorsSize);

        std::cout << "Visitors has been changed to " << change << std::endl;
        visitors = change;
    }

    std::string str = "";
    str = fixStr(id, region, state, code, name, type, visitors);
    //closes file and opens so i can write to it
    din.close();
    std::string fileName = DataBaseName + ".data";
    //ios::in means that im inputting to the file
    dout.open(fileName.c_str(), std::ios::in);
    dout.seekp(index * RecordSize, std::ios::beg);
    dout << str << std::endl;
    dout.close();
    //makes sure the file is able to be read again
    din.open(fileName);

    return true;
}

bool DataBase::addRecord(const int record)
{
    std::string id = "", region = "", state = "", code = "", name = "", type = "", visitors = "";
    bool blankFound = true, recordExists = false;
    int index = 0;
    index = binarySearch(record, id, region, state, code, name, type, visitors);
    
    std::cout << "id = " << id << std::endl;
    std::cout << "id size = " << id.length() << std::endl;

    //-(NumRecord + 1) is returned if no record is found and record is less than first
    if (index == -(NumRecord * 2))
        index = 0;

    if (index < 0)
        index *= -1;

    std::cout << "index = " << index << std::endl;
    //checks if the id from binary is not empty if it is then there is a record there.
    if (id.length() > 0)
    {
        int temp = std::stoi(id);
        std::cout << "temp from stoi = " << temp << std::endl;
        if (record == temp)
            recordExists = true;
        // else if (record > temp)
        //     index++;
        // else
        //     if (index != 0)
        //         index--;

        blankFound = false;
    }

    id = std::to_string(record);
    if (id.length() > (unsigned int)IdSize)
        id.resize(IdSize);
        
    if (blankFound)
    {
        getInfo(region, state, code, name, type, visitors);

        std::string str = "";
        str = fixStr(id, region, state, code, name, type, visitors);

        //closes file to add a record to it.
        std::string fileName = DataBaseName + ".data";
        dout.open(fileName, std::ios::in);
        dout.seekp(index * RecordSize, std::ios::beg);
        dout << str << std::endl;
        dout.close();
        din.open(fileName);

        return true;
    }
    else if (!blankFound && !recordExists)
    {
        std::string fileName = DataBaseName + ".data";
        std::string junkFileName = "junk.data";
        std::string str = "";
        int count = 0;

        std::cout << "index final = " << index << std::endl;
        std::cout << "id = " << id << std::endl;

        if (id.length() > (unsigned int)IdSize)
            id.resize(IdSize);

        getInfo(region, state, code, name, type, visitors);
        str = fixStr(id, region, state, code, name, type, visitors);
        
        din.close();
        rename(fileName.c_str(), junkFileName.c_str());
        din.open(junkFileName.c_str());
        dout.open(fileName);

        if (index == 0)
        {
            dout << str << std::endl;
            insertBlank();
            count += 2;
        }

        std::string temp = "";
        while (getline(din, temp, '\n'))
        {
            if (temp[0] == 'i')
            {
                //skips the line
            }
            else if (count == index)
            {
                std::cout << "count == index\n";
                dout << str << std::endl;
                insertBlank();

                //inserts the currently read recoord as well.
                dout << temp << std::endl;
                insertBlank();
                count += 2;
            }
            else
            {
                dout << temp << std::endl;
                insertBlank();
                count++;
            }
        }
        if (index == (NumRecord - 1) && !blankFound)
        {
            dout << str << std::endl;
            insertBlank();
            count++;
        }

        din.close();
        dout.close();

        NumRecord = count * 2;
        temp = DataBaseName + ".config";
        dout.open(temp.c_str());
        dout << NumRecord;
        dout.close();

        //removes and opens dataBase.data again
        remove(junkFileName.c_str());
        din.open(fileName.c_str());
    }
    else 
    {
        std::cout << "record " << record << " already exists!\n";
    }

    return false;
}

bool DataBase::deleteRecord(const int record)
{
    int index = 0;
    std::string id = "", region = "", state = "", code = "", name = "", type = "", visitors = "";

    index = binarySearch(record, id, region, state, code, name, type, visitors);

    if (index >= 0)
    {
        std::string fileName = DataBaseName + ".data";

        std::cout << "deleting record " << record << "...\n";
        //closing file before writing to it
        din.close();
        dout.open(fileName.c_str(), std::ios::in);
        dout.seekp(index * RecordSize, std::ios::beg);
        insertBlank();
        dout.close();
        din.open(fileName.c_str());
        
        return true;
    }
    else
        std::cout << "File does not exist.\n";

    return false;
}

int DataBase::getSize()
{
    return NumRecord;
}

std::string DataBase::fixStr(std::string id, std::string region, std::string state, std::string code, std::string name, std::string type, std::string visitors)
{
    std::string str = "";
    //fixes spacing
    str += id;
    for (unsigned int i = 0; i < IdSize - id.length(); i++)
        str += Divider;
    //addsspace
    str += Divider;

    str += region;
    for (unsigned int i = 0; i < RegionSize - region.length(); i++)
        str += Divider;
    //adds space
    str += Divider;

    str += state;
    for (unsigned int i = 0; i < StateSize - state.length(); i++)
        str += Divider;
    //adds space
    str += Divider;
    
    str += code;
    for (unsigned int i = 0; i < CodeSize - code.length(); i++)
        str += Divider;
    //adds space
    str += Divider;
    
    str += name;
    for (unsigned int i = 0; i < NameSize - name.length(); i++)
        str += Divider;
    //adds space
    str += Divider;

    str += type;
    for (unsigned int i = 0; i < TypeSize - type.length(); i++)
        str += Divider;
    //adds space
    str += Divider;

    str += visitors;
    for (unsigned int i = 0; i < VistorsSize - visitors.length(); i++)
        str += Divider;

    return str;
}

void DataBase::createReport()
{
    int count = 1;
    int i = 0;
    std::string id = "", region = "", state = "", code = "", name = "", type = "", visitors = "";

    while (count <= 10)
    {
        if (getRecord(i,id, region, state, code, name, type, visitors))
        {
            if (id[0] != 'i')
            {
                int temp = std::stoi(id);
                displayRecord(temp);
                std::cout << std::endl;
                count++;
                i++;
            }
            else
                i++;
        }

    }
}

void DataBase::getInfo(std::string &region, std::string &state, std::string &code, std::string &name, std::string &type, std::string &visitors)
{
    std::cout << "Enter the Region: ";
    std::cin >> region;
    if (region.length() > (unsigned int)RegionSize)
        region.resize(RegionSize);

    std::cout << "Enter the State: ";
    std::cin >> state;
    if (state.length() > (unsigned int)StateSize)
        state.resize(StateSize);

    std::cout << "Enter the Code: ";
    std::cin >> code;
    if (code.length() > (unsigned int)CodeSize)
        code.resize(CodeSize);
            
    std::cout << "Enter the Name: ";
    std::getline(std::cin >> std::ws, name);
    if (name.length() > (unsigned int)NameSize)
        name.resize(NameSize);
    for (unsigned int i = 0; i < name.length(); i++)
        if (name[i] == ' ')
            name [i] = '_';

    std::cout << "Enter the Type: ";
    std::getline(std::cin >> std::ws, type);
    if (type.length() > (unsigned int)TypeSize)
        type.resize(TypeSize);
    for (unsigned int i = 0; i < type.length(); i++)
        if (type[i] == ' ')
            type[i] = '_';

    std::cout << "Enter the Visitors: ";
    std::cin >> visitors;
    if (visitors.length() > (unsigned int)VistorsSize)
        visitors.resize(VistorsSize);
}

void DataBase::insertBlank()
{
    std::string blank = "";
    blank += "iiiiiii";
    blank += Divider;
    blank += "rr";
    blank += Divider;
    blank += "ss";
    blank += Divider;
    blank += "cccc";
    blank += Divider;
    blank += "nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn";
    blank += Divider;
    blank += "ttttttttttttttttttttttttttttttttttttt";
    blank += Divider;
    blank += "vvvvvvvvv";

    dout << blank << std::endl;
}