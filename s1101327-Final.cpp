#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;
using std::setfill;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ios;

#include <vector>
using std::vector;

struct Date
{
   int year;
   int month;
   int day;
   int hour;
};

struct MemberRecord
{
   char idNumber[ 12 ];   // account number
   char password[ 24 ];   // password
   char name[ 8 ];        // name
};

struct ReservationRecord
{
   char idNumber[ 12 ]; // account number
   int branchCode;      // branch code
   Date date;           // reservation date
   int numCustomers;    // number of customers
};

char branchNames[ 19 ][ 24 ] = { "", "Taipei Dunhua South",   "Taipei Zhongxiao",
                                     "Taipei Songjiang",    "Taipei Nanjing",
                                     "Taipei Linsen",       "Taipei Zhonghua New",
                                     "Banqiao Guanqian",    "Yonghe Lehua",
                                     "Taoyuan Zhonghua",    "Taoyuan Nankan",
                                     "Zhongli Zhongyang",   "Hsinchu Beida",
                                     "Taichung Ziyou",      "Chiayi Ren'ai",
                                     "Tainan Ximen",        "Kaohsiung Zhonghua New",
                                     "Kaohsiung Jianxing",  "Pingtung Kending" };

// read all memberDetails from the file Members.dat
void loadMemberDetails( vector< MemberRecord > &memberDetails );

// read all reservations that are not out of date, from the file Reservations.dat
void loadReservations( vector< ReservationRecord > &reservations );

// compute the current date
Date compCurrentDate();

// return true if and only if date1 <= date2
bool lessEqual( const Date &date1, const Date &date2 );

// input an integer in the range [ begin, end ]
int inputAnInteger( int begin, int end );

// login and display the submenu
void login( const vector< MemberRecord > &memberDetails,
            vector< ReservationRecord > &reservations );

// there exists a member with specified idNumber and password
bool valid( char idNumber[], char password[],
            const vector< MemberRecord > &memberDetails );

// add a new reservation for the member with specified IDnumber
void reservation( char idNumber[], vector< ReservationRecord > &reservations );

// compute 7 dates which is starting from the current date
void compAvailableDates( Date availableDates[] );

// display all fields of reservation
void output( ReservationRecord reservation );

// display all reservations for the member with specified IDnumber,
// then let the member to choose one of them to delete
void queryDelete( char idNumber[], vector< ReservationRecord > &reservations );

// add a new member
void registration( vector< MemberRecord > &memberDetails );

// return true if idNumber is a legal ID number
//bool legalID( char idNumber[] );

// return true if idNumber belongs to memberDetails
bool existingID( char idNumber[], const vector< MemberRecord > &memberDetails );

// write all memberDetails into the file Members.dat
void saveMemberDetails( const vector< MemberRecord > &memberDetails );

// write all reservations that are not out of date, into the file Reservations.dat
void saveReservations( const vector< ReservationRecord > &reservations );

int main()
{
   vector< MemberRecord > memberDetails; // member details for all members
   vector< ReservationRecord > reservations; // all reservations

   loadMemberDetails( memberDetails );
   loadReservations( reservations );

   cout << "Welcome to the Cashbox Party World!\n\n";

   int choice;
   while( true )
   {
      cout << "1 - Login\n";
      cout << "2 - Registration\n";
      cout << "3 - End\n";

      do cout << "\nEnter your choice (1~3): ";
      while( ( choice = inputAnInteger( 1, 3 ) ) == -1 );
      cout << endl;

      switch ( choice )
      {
      case 1:
         login( memberDetails, reservations );
         break;

      case 2:
         registration( memberDetails );
         break;

      case 3:
         saveMemberDetails( memberDetails );
         saveReservations( reservations );
         cout << "Thank you! Goodbye!\n\n";
         system( "pause" );
         return 0;

      default:
         cout << "\nIncorrect choice!\n";
         break;
      }
   }

   system( "pause" );
}

void loadMemberDetails( vector< MemberRecord > &memberDetails )
{
    ifstream inFile("Members.dat", ios::binary);
    if (!inFile)
    {
        cout << "Members.dat could not be opened.\n";
        exit(1);
    }

    inFile.seekg(0, ios::end);
    int num = inFile.tellg() / sizeof(MemberRecord);
    inFile.seekg(0, ios::beg);
    MemberRecord temp;
    for (int i = 1; i <= num; i++)
    {
        inFile.read(reinterpret_cast<char*>(&temp), sizeof(MemberRecord));
        memberDetails.push_back(temp);
    }
    inFile.close();


}

void loadReservations( vector< ReservationRecord > &reservations )
{

    ifstream inFile("Reservations.dat", ios::binary);
    if (!inFile)
    {
        cout << "Reservations.dat could not be opened.\n";
        exit(1);
    }

    inFile.seekg(0, ios::end);
    int num = inFile.tellg() / sizeof(ReservationRecord);
    inFile.seekg(0, ios::beg);
    ReservationRecord temp;
    Date currentdate = compCurrentDate();
    
    for (int i = 1; i <= num; i++)
    {
        inFile.read(reinterpret_cast<char*>(&temp), sizeof(ReservationRecord));
        if (lessEqual(currentdate, temp.date))
            reservations.push_back(temp);
    }
    inFile.close();


}

Date compCurrentDate()
{
   Date currentDate;
   tm structuredTime;
   time_t rawTime = time( 0 );
   localtime_s( &structuredTime, &rawTime );

   currentDate.year = structuredTime.tm_year + 1900;
   currentDate.month = structuredTime.tm_mon + 1;
   currentDate.day = structuredTime.tm_mday;
   currentDate.hour = structuredTime.tm_hour;

   return currentDate;
}

bool lessEqual( const Date &date1, const Date &date2 )
{

    if (date2.year > date1.year)
        return 1;
    else if (date2.year == date1.year && date2.month > date1.month)
        return 1;
    else if (date2.year == date1.year && date2.month == date1.month && date2.day >= date1.day)
        return 1;
    return 0;


}

int inputAnInteger( int begin, int end )
{
   char string[ 80 ];
   cin.getline( string, 80, '\n' );

   if( strlen( string ) == 0 )
      exit( 0 );

   for( unsigned int i = 0; i < strlen( string ); i++ )
      if( string[ i ] < '0' || string[ i ] > '9' )
         return -1;

   int number = atoi( string );
   if( number >= begin && number <= end )
      return number;
   else
      return -1;
}

void login( const vector< MemberRecord > &memberDetails,
            vector< ReservationRecord > &reservations )
{
   char idNumber[ 12 ] = "";
   char password[ 24 ] = "";

   do {
      cout << "Please enter your ID number: ";
      cin >> idNumber;
      cout << "Enter your password: ";
      cin >> password;
   } while( !valid( idNumber, password, memberDetails ) );

   cin.ignore();

   int choice;

   while( true )
   {      
      cout << "\n1 - Make Reservation\n";
      cout << "2 - Reservation Enquiry/Canceling\n";
      cout << "3 - End\n";

      do cout << "\nEnter your choice (1~3): ";
      while( ( choice = inputAnInteger( 1, 3 ) ) == -1 );
      cout << endl;

      switch ( choice )
      {
      case 1:
         reservation( idNumber, reservations );
         break;

      case 2:
         queryDelete( idNumber, reservations );
         break;

      case 3:
         return;

      default:
         cout << "\nIncorrect choice!\n";
         break;
      }
   }
}

bool valid( char idNumber[], char password[], const vector< MemberRecord > &memberDetails )
{

    for (int i = 0; i < memberDetails.size(); i++)
        if (strcmp(idNumber, memberDetails[i].idNumber) == 0 && strcmp(password, memberDetails[i].password) == 0)
            return 1;
    cout << "\nInvalid account number or password. Please try again.\n\n";
    return 0;


}

void reservation( char idNumber[], vector< ReservationRecord > &reservations )
{
   ReservationRecord newReservation;
   
   for (int i = 0; i < strlen(idNumber); i++)
       newReservation.idNumber[i] = idNumber[i];
   newReservation.idNumber[strlen(idNumber)] = '\0';
   for (int i = 1; i < 19; i++)
       cout << setw(2) << i << ". " << branchNames[i] << endl;
   do
   {
       cout << "\nEnter your choice (0 to end): ";
       cin >> newReservation.branchCode;
       cin.ignore();
       cout << endl;
       if (newReservation.branchCode == 0)
           return;
   } while (newReservation.branchCode < 1 || newReservation.branchCode>18);

   Date currentdate = compCurrentDate();
   cout << "The current hour is " << currentdate.year << '/' << currentdate.month << '/' << currentdate.day << ':' << currentdate.hour << endl;
   
   Date availableDates[7];
   compAvailableDates(availableDates);
   int choice;
   cout << "\nAvailable days:\n\n";
   for (int i = 0; i < 7; i++)
       cout << i + 1 << ". " << availableDates[i].year << '/' << availableDates[i].month << '/' << availableDates[i].day << endl;
   
   do
   {
       cout << "\nEnter your choice (0 to end): ";
       cin >> choice;
       cin.ignore();
       if (choice == 0)
           return;
   } while (choice < 1 || choice>7 );
   
   newReservation.date = availableDates[choice - 1];
   int beginhour = availableDates[choice - 1].day == currentdate.day ? currentdate.hour + 1 : 0;
   int endhour = 23;
   do
   {
       cout << "\nEnter hour (" << beginhour << '~' << endhour << "): ";
       cin >> newReservation.date.hour;
       cin.ignore();
   } while (newReservation.date.hour<beginhour || newReservation.date.hour>endhour);

   
   do
   {
       cout << "\nEnter the number of customers (1~30, 0 to end): ";
       cin >> newReservation.numCustomers;
       cin.ignore();
       cout << endl;
       if (newReservation.numCustomers == 0)
           return;
   } while (newReservation.numCustomers < 1 || newReservation.numCustomers>30);

   

   cout << endl << setw( 26 ) << "Branch"
                << setw( 14 ) << "Date" << setw( 8 ) << "Hour"
                << setw( 19 ) << "No of Customers" << endl;

   output( newReservation );

   cout << "\nReservation Completed!\n";

   reservations.push_back( newReservation );
}

void compAvailableDates( Date availableDates[] )
{
   Date currentDate = compCurrentDate();
   int dayOfMonth[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
   if (currentDate.year % 400 == 0 || currentDate.year % 4 == 0 && currentDate.year % 100 != 0)
       dayOfMonth[2] = 29;
   
   bool yearchange = 0;
   for (int i = 0; i < 7; i++)
   {
       if (currentDate.hour == 23)
       {
           currentDate.day += 1;
           currentDate.hour = 0;
       }
       if (currentDate.day > dayOfMonth[currentDate.month])
       {
           currentDate.day = 1;
           currentDate.month += 1;
       }
       if (currentDate.month > 12)
       {
           currentDate.month = 1;
           currentDate.year += 1;
           yearchange = 1;
       }
       if (yearchange)
       {
           yearchange = 0;
           if (currentDate.year % 400 == 0 || currentDate.year % 4 == 0 && currentDate.year % 100 != 0)
               dayOfMonth[2] = 29;
       }
       availableDates[i] = currentDate;
       ++currentDate.day;
   }

   
   


}

void output( ReservationRecord reservation )
{
   cout << setw( 26 ) << branchNames[ reservation.branchCode ]
        << setw( 8 ) << reservation.date.year << '-'
        << setw( 2 ) << setfill( '0' ) << reservation.date.month << '-'
        << setw( 2 ) << setfill( '0' ) << reservation.date.day
        << setw( 8 ) << setfill( ' ' ) << reservation.date.hour
        << setw( 19 ) << reservation.numCustomers << endl;
}

void queryDelete( char idNumber[], vector< ReservationRecord > &reservations )
{
   Date currentDate = compCurrentDate();

   int index = 0;
   for (int i = 0; i < reservations.size(); i++)
       if (strcmp(idNumber, reservations[i].idNumber) == 0)
       {
           if (!index) {
               cout << endl << setw(26) << "Branch"
                   << setw(14) << "Date" << setw(8) << "Hour"
                   << setw(19) << "No of Customers" << endl;
           }
           cout << ++index << '.';
           output(reservations[i]);
       }
   if (!index)
   {
       cout << "\nNo reservations!\n";
       return;
   }
   int choice;

   do
   {
       cout << "\nChoose a reservation to cancel (0: keep all reservations): ";
       cin >> choice;
       cout << endl;
       cin.ignore();
       if (choice == 0)
           return;
   } while (!(choice >= 1 && choice <= index));
   
   int ii = 0;
   vector< ReservationRecord >Newreservations;
  
   for (int i = 0; i < reservations.size(); i++){
        if (strcmp(idNumber, reservations[i].idNumber) == 0){
           if (++ii == choice)
               continue;}
        Newreservations.push_back(reservations[i]);
   }
   reservations = Newreservations;
   

}

void registration( vector< MemberRecord > &memberDetails )
{
   MemberRecord newMember;

   cout << "Input your ID Number: ";
   cin >> newMember.idNumber;
   cin.ignore();

   if( existingID( newMember.idNumber, memberDetails ) )
   {
      cout << "\nYou are already a member!\n\n";
      return;
   }

   cout << "Input your Name: ";
   cin >> newMember.name;

   cout << "Choose a password: ";
   cin >> newMember.password;

   cin.ignore();

   memberDetails.push_back( newMember );

   cout << "\nRegistration Completed!\n\n";
}

bool existingID( char idNumber[], const vector< MemberRecord > &memberDetails )
{

    for (int i = 0; i < memberDetails.size(); i++)
        if (strcmp(idNumber, memberDetails[i].idNumber) == 0)
            return 1;
    return 0;


}

void saveMemberDetails( const vector< MemberRecord > &memberDetails )
{

    ofstream outFile("Members.dat",  ios::binary);
    for (int i = 0; i < memberDetails.size(); i++)
        outFile.write(reinterpret_cast<const char*>(&memberDetails[i]), sizeof(MemberRecord));
    outFile.close();


}

void saveReservations( const vector< ReservationRecord > &reservations )
{

    ofstream outFile("Reservations.dat", ios::binary);
    for (int i = 0; i < reservations.size(); i++)
        outFile.write(reinterpret_cast<const char*>(&reservations[i]), sizeof(ReservationRecord));
    outFile.close();


}