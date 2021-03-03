//-----------------------------------------------------
// Example code to read from fixed length records (random access file
// Author:  Susan Gauch
// Last updated:  Feb. 2, 2020
//-----------------------------------------------------
#include <iostream>
#include <fstream>
using namespace std;

const int RECORD_SIZE = 71;
const int NUM_RECORDS = 4111;

// Get record number n (Records numbered from 0 to NUM_RECORDS-1)
bool getRecord(ifstream &Din, const int RecordNum,
               string &Id, int &Experience, string &Married, float &Wage, string &Industry)
{
bool Success = false;

   if ((0 <= RecordNum) && (RecordNum < NUM_RECORDS))
   {
      Din.seekg(RecordNum * RECORD_SIZE, ios::beg);
      Din >> Id >> Experience >> Married >> Wage >> Industry;
      Success = true;
   }
   else
      cout << "Record " << RecordNum << " out of range.\n";

   return Success;
}

// Binary Search by record id
int binarySearch (ifstream &Din, const string Id,
                  int &Experience, string &Married, float &Wage, string &Industry)
{
   int Low = 0;
   int High = NUM_RECORDS-1;
   int Middle;
   string MiddleId;
   bool Failure = false;

   bool Found = false;
   while (!Found && (High >= Low) && !Failure)
   {
      Middle = (Low + High) / 2;
      if (getRecord(Din, Middle, MiddleId, Experience, Married, Wage, Industry))
      {
         if (MiddleId == Id)
            Found = true;
         else if (MiddleId < Id)
            Low = Middle + 1;
         else
            High = Middle -1;
      }
      else
      {
         cout << "Could not get record " << Middle << endl;
         Failure = true;
      }
   }
   if (Found)
      return Middle;  // the record number of the record
    else
      return -1;
}

int main()
{
ifstream Din;
string Filename = "input.txt";
string Id = "ID";
int Experience = -1;
string Married = "UNKNOWN";
float Wage = -1.0;
string Industry = "INDUSTRY";
int recordNum;

   Din.open(Filename.c_str());
   if (!Din)
      cout << "Could not open " << Filename << ".\n\n";
   else
   {
      cout << "\n------------- Testing getRecord ------------\n";
      // record out of range
      recordNum = NUM_RECORDS;
      if (getRecord(Din, recordNum, Id, Experience, Married, Wage, Industry))
         cout << "Record " << recordNum << ": " << Id << ", " << Experience
              << ", " << Married << ", " << Wage << ", " << Industry << "\n\n";
      else
         cout << "Could not get record " << recordNum << ".\n\n";

      // first record in the file
      recordNum = 0;
      if (getRecord(Din, recordNum, Id, Experience, Married, Wage, Industry))
         cout << "Record " << recordNum << ": " << Id << ", " << Experience
              << ", " << Married << ", " << Wage << ", " << Industry << "\n\n";
      else
         cout << "Could not get record " << recordNum << ".\n\n";

      // last record in the file
      recordNum = NUM_RECORDS-1;
      if (getRecord(Din, recordNum, Id, Experience, Married, Wage, Industry))
         cout << "Record " << recordNum << ": " << Id << ", " << Experience
              << ", " << Married << ", " << Wage << ", " << Industry << "\n\n";
      else
         cout << "Could not get record " << recordNum << ".\n\n";

      // some other record in the file
      recordNum = 5;
      if (getRecord(Din, recordNum, Id, Experience, Married, Wage, Industry))
         cout << "Record " << recordNum << ": " << Id << ", " << Experience
              << ", " << Married << ", " << Wage << ", " << Industry << "\n\n";
      else
         cout << "Could not get record " << recordNum << ".\n\n";

      cout << "\n------------- Testing binarySearch ------------\n";
      // non-existant record
      Id = "00100";
      recordNum = binarySearch(Din, Id, Experience, Married, Wage, Industry);
      if (recordNum != -1)
         cout << "Id " << Id << " found at record " << recordNum << ": "
              << Experience << ", " << Married << ", " << Wage << ", " << Industry << "\n\n";
      else
         cout << "Record " << Id << " not found.\n\n";

      // first record
      Id = "00000";
      recordNum = binarySearch(Din, Id, Experience, Married, Wage, Industry);
      if (recordNum != -1)
         cout << "Id " << Id << " found at record " << recordNum << ": "
              << Experience << ", " << Married << ", " << Wage << ", " << Industry << "\n\n";
      else
         cout << "Record " << Id << " not found.\n\n";

      // last record
      Id = "00099";
      recordNum = binarySearch(Din, Id, Experience, Married, Wage, Industry);
      if (recordNum != -1)
         cout << "Id " << Id << " found at record " << recordNum << ": "
              << Experience << ", " << Married << ", " << Wage << ", " << Industry << "\n\n";
      else
         cout << "Record " << Id << " not found.\n\n";

      // some other record
      Id = "00006";
      recordNum = binarySearch(Din, Id, Experience, Married, Wage, Industry);
      if (recordNum != -1)
         cout << "Id " << Id << " found at record " << recordNum << ": "
              << Experience << ", " << Married << ", " << Wage << ", " << Industry << "\n\n";
      else
         cout << "Record " << Id << " not found.\n\n";


      Din.close();   //close the file when done
   }
   return 0;
}
