#include <iostream>
#include <windows.h>
#include <set>

#define green HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,10)
#define red HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,12)
#define blue HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,11)
#define white HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,7)
#define yelow HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,6)

using namespace std;

//----------------------------------------------------------------------
class MoneyValue {
public:
    double IRR = 1 ; // value of USD/IRR
    double EUR = 1 ; // value of USD/EUR
    void update_price () {
        yelow;
        cout << "Enter the equivalent value" << endl ;
        cout << "1 USD = ? IRR" << endl ;
        double r ; // 1 USD = r IRR
        cin >> r ;
        IRR = r ;
        cout << "Enter the equivalent value" << endl ;
        cout << "1 USD = ? EUR" << endl ;
        double e ; // 1 USD = e EUR
        cin >> e ;
        EUR = e ;
    }
};
//-----------------------------------------------------------------------
class product {
public:
    long long ID ;
    string Name ;
    int Number ;
    double Price ;
    void print ( MoneyValue a ) {
        cout << "ID : " << ID << endl ;
        cout << "Name : " << Name << endl ;
        cout << "Number of available : " << Number << endl ;
        cout << "Price : " << Price << " USD = " << Price*a.EUR
             << " EUR = " << Price*a.IRR << " IRR" << endl << endl ;
    }
    bool operator < (product b) {
        if ( this->Name < b.Name )
            return true ;
        if ( this->Name > b.Name )
            return false ;
        if ( this->ID < b.ID )
            return true ;
        return false;
    }

};
//------------------------------------------------------------------------
class pset : public set<product> {
public:
    // search by name
    bool search (string name) {
        for (const auto &i : (*this) )
            if ( i.Name == name )
                return  true ;
        return false ;
    }
};
//------------------------------------------------------------------------
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//------------------------------------------------------------------------


int main()
{
    return 0;
}
