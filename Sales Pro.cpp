#include <iostream>
#include <windows.h>
#include <string>
#include <set>
#include <vector>
#include <ctime>


#define green if (1){HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,10);}
#define red if (1){HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,12);}
#define blue if (1){HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,11);}
#define white if (1){HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,7);}
#define yelow if (1){HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ;SetConsoleTextAttribute(hConsole,6);}

#define isintc if ( cin.fail() ){ \
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ; \
    SetConsoleTextAttribute(hConsole,12) ; \
    cin.clear() ; \
    cin.ignore(10000,'\n') ; \
    cout << endl << "Error !" << endl ; \
    continue ; }

#define isint if ( cin.fail() ){ \
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE) ; \
    SetConsoleTextAttribute(hConsole,12) ; \
    cin.clear() ; \
    cin.ignore(10000,'\n') ; \
    cout << endl << "Error !" << endl ; }

// cin is correct ?
#define cor if ( cin.fail() ) { \
    cin.clear() ; \
    red ; \
    cout << endl << "Error !" << endl ; \
    return ; }

using namespace std;

//----------------------------------------------------------------------
class MoneyValue {
public:
    double IRR = 1 ; // value of USD/IRR
    double EUR = 1 ; // value of USD/EUR
    void update_price () {
        yelow ;
        cout << "Enter the equivalent value" << endl ;
        cout << "1 USD = ? IRR" << endl ;
        double r ; // 1 USD = r IRR
        cin >> r ;
        cor ;
        IRR = r ;
        cout << "Enter the equivalent value" << endl ;
        cout << "1 USD = ? EUR" << endl ;
        double e ; // 1 USD = e EUR
        cin >> e ;
        cor ;
        EUR = e ;
    }
    double tabdil_DR (double d) {
        double r ;
        r = d*(this->IRR) ;
        return r ;
    }
    double tabdil_DE (double d) {
        double e ;
        e = d*(this->EUR) ;
        return e ;
    }
};
//-----------------------------------------------------------------------
class product {
public:
    long long ID ;
    string Name ;
    int Number ;
    double Price ; // USD price
    void print ( MoneyValue a ) {
        cout << "ID : " << ID << endl ;
        cout << "Name : " << Name << endl ;
        cout << "Number of available : " << Number << endl ;
        cout << "Price : " << Price << " USD = " << Price*a.EUR
             << " EUR = " << Price*a.IRR << " IRR" << endl << endl ;
    }
};
//------------------------------------------------------------------------
// operator < overloaded for pset
bool operator < (product a , product b) {
    if ( a.Name < b.Name )
        return true ;
    if ( a.Name > b.Name )
        return false ;
    if ( a.ID < b.ID )
        return true ;
    return false;
}
//------------------------------------------------------------------------
class pset : public set<product> {
public:
    // print all products
    void print ( MoneyValue a ) {
        if (this->empty()) {
            red ;
            cout << "Empty !" << endl ;
        }
        else {
            int j = 1 ;
            for ( auto i : (*this)) {
                cout << j << ")" << endl ;
                i.print(a) ;
                j++ ;
            }
        }
    }
    // add product (for admin)
    void add_product () {
        yelow;
        cout << "Enter ID : " ;
        long long id ;
        cin >> id ;
        cor ;
        cout << "Enter name : " ;
        string name ;
        cin >> name ;
        cout << "Enter number of available : " ;
        int num ;
        cin >> num ;
        cor ;
        cout << "Enter price (USD) : " ;
        double price ;
        cin >> price ;
        cor ;
        cout << endl ;
        if ( this->search_id(id) ) {
            red ;
            cout << "Product is duplicate !" << endl ;
        }
        else {
            this->insert ({id,name,num,price}) ;
            green ;
            cout << "Added !" << endl ;
        }
    }
    void remove_product () {
        yelow ;
        cout << "Enter ID : " ;
        long long id ;
        cin >> id ;
        cor;
        for ( const auto &i : (*this) )
            if ( i.ID == id ) {
                cout << "Name : " << i.Name << endl ;
                cout << "Number of available : " << i.Number << endl ;
                cout << "Price (USD) : " << i.Price << endl ;
                this->erase({i.ID,i.Name,i.Number,i.Price}) ;
                green ;
                cout << endl << "Deleted the product !" << endl ;
                return;
            }
        red ;
        cout << "No ID found !" << endl ;
    }
    void edit_product () {
        yelow ;
        cout << "Enter ID : " ;
        long long id ;
        cin >> id ;
        cor;
        for (  pset::iterator i = this->begin() ; i != this->end() ; i++ )
            if ( i->ID == id ) {
                cout << "Name : " << i->Name << endl ;
                cout << "Number of available : " << i->Number << endl ;
                cout << "Price (USD) : " << i->Price << endl ;
                cout << endl  ;
                blue ;
                cout << "Enter new name : " ;
                string name ;
                cin >> name ;
                cout << "Enter new number of available : " ;
                int num ;
                cin >> num ;
                cor ;
                cout << "Enter new price (USD) : " ;
                double price ;
                cin >> price ;
                cor ;
                this->erase(i) ;
                this->insert({id,name,num,price}) ;
                green;
                cout << endl << "Edited !" << endl ;
                return ;
            }
        red ;
        cout << endl << "No ID found !" << endl ;
    }
    // search by ID
    bool search_id (long long id) {
        for ( const auto &i : (*this) )
            if ( i.ID == id )
                return  true ;
        return false ;
    }
    // search by name
    void search_name ( MoneyValue a ) {
        yelow;
        cout << "Enter name : " ;
        string name ;
        cin >> name ;
        for ( auto i : (*this) )
            if ( i.Name == name ) {
                white ;
                i.print(a) ;
                return ;
            }
        red ;
        cout << "Not found !" << endl ;
    }
    // sum of USD prices of produts that is in Cart
    double sum_of_prices () {
        double sum = 0 ;
        for ( const auto &i : (*this) )
            sum += i.Price*i.Number ;
        return sum ;
    }
};
//------------------------------------------------------------------------
class invoice {
public:
    time_t now ;
    tm * local_time ;
    string Username_of_buyer ;
    pset O ; // orders
    double DPayment_amount ; // USD payment amount
    double EPayment_amount ; // EUR payment amount
    double RPayment_amount ; // IRR payment amount
    invoice (pset &cart , string username , double Dpay_amount , double Epay_amount , double Rpay_amount ) {
        now = time(0) ;
        local_time = localtime(&now) ;
        Username_of_buyer = username ;
        for (const auto &i : cart)
            O.insert(i) ;
        DPayment_amount = Dpay_amount ;
        EPayment_amount = Epay_amount ;
        RPayment_amount = Rpay_amount ;
    }
};
//------------------------------------------------------------------------
class ivec : public vector<invoice> {
public:
    // print all orders and invoices
    void i_print () {
        if (this->empty()) {
            red ;
            cout << "Empty !" << endl ;
        }
        else {
            white ;
            cout << "Seller : Sales Pro" << endl ;
            for ( const auto &i : (*this) ) {
                cout << "--------------------------------------------------------------------------" << endl ;
                cout << "Buyer : " << i.Username_of_buyer << endl ;
                cout << "Date : " << i.local_time->tm_mday << "/"
                     << i.local_time->tm_mon+1 << "/" << i.local_time->tm_year+1900 << endl ;
                cout << "Time : " << i.local_time->tm_hour << ":" << i.local_time->tm_min << endl ;
                for ( const auto &j : i.O ) {
                    cout << "ID : " << j.ID << "\t"
                         << "Name : " << j.Name << "\t"
                         << "Price : " << j.Price << "\t"
                         << "Number : " << j.Number << endl ;
                }
                cout << "USD amount paid : " << i.DPayment_amount << endl ;
                cout << "EUR amount paid : " << i.EPayment_amount << endl ;
                cout << "IRR amount paid : " << i.RPayment_amount << endl ;
                cout << endl ;
            }
        }
    }
    void total_incom () {
        double Dincome = 0 ;
        double Eincome = 0 ;
        double Rincome = 0 ;
        for ( const auto &i : (*this) ) {
            Dincome += i.DPayment_amount ;
            Eincome += i.EPayment_amount ;
            Rincome += i.RPayment_amount ;
        }
        green ;
        cout << endl ;
        cout << "Total USD income : " << Dincome << endl ;
        cout << "Total EUR income : " << Eincome << endl ;
        cout << "Total IRR income : " << Rincome << endl ;
    }
};
//------------------------------------------------------------------------


//------------------------------------------------------------------------
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//------------------------------------------------------------------------
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//------------------------------------------------------------------------

int main()
{
    return 0;
}
