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

class user {
public:
    double DWallet ; // amount of USD
    double EWallet ; // amount of EUR
    double RWallet ; // amount of IRR
    string Username ;
    string Password ;
    pset Cart ;
    ivec Orders ; // successful orders and invoices
    user (string u , string p ) {
        DWallet = 0 ;
        EWallet = 0 ;
        RWallet = 0 ;
        Username = u ;
        Password = p ;
        Cart.clear() ;
        Orders.clear() ;
    }
    // add product by search ID (for users)
    void add_product_id (pset &p) {
        yelow ;
        cout << "Enter the product ID : " ;
        long long id ;
        cin >> id ;
        cor ;
        if ( this->Cart.search_id(id) ) {
            red ;
            cout << "ID is duplicate !" << endl ;
        }
        else {
            for ( const auto &i : p )
                if ( i.ID == id ){
                    cout << "Enter number of the product : " ;
                    int num ;
                    cin >> num ;
                    cor;
                    product a ;
                    a.ID = id ;
                    a.Name = i.Name ;
                    a.Number = num ;
                    a.Price = i.Price ;
                    this->Cart.insert(a) ;
                    green ;
                    cout << "Added !" << endl ;
                    return;
                }
            red ;
            cout << "ID not found !" << endl ;
        }
    }
    void payment (MoneyValue a , pset &p , user & admin ) {
        if (this->Cart.empty()) {
            red ;
            cout << "Cart is empty !" << endl ;
        }
        else {
            bool r ; // be remove ?
            bool b = 1 ; // be buy ?
            for ( const auto &i : this->Cart ) {
                r = 1 ;
                for ( const auto &j : p ) {
                    if (i.ID == j.ID ){
                        r = 0 ;
                        if (i.Number > j.Number ) {
                            red ;
                            cout << "This quantity of product with ID " << i.ID << " is not available !" << endl ;
                            b = 0 ;
                        }
                    }
                }
                if (r == 1) {
                    red ;
                    cout << "The product with ID " << i.ID << " is no longer available !" << endl ;
                    cout << "The product with ID " << i.ID << " removed from your Cart !" << endl ;
                    this->Cart.erase(i) ;
                }
            }
            if (b == 1 ) {
                yelow ;
                cout << "1 - Payment by USD Wallet" << endl ;
                cout << "2 - Payment by EUR Wallet" << endl ;
                cout << "3 - Payment by IRR Wallet" << endl ;
                cout << endl ;
                cout << "Enter command number : " ;
                int c ; // command number
                cin >> c ;
                isint ;
                cout << endl ;
                switch (c) {
                case 1 : {
                    if (this->Cart.sum_of_prices() <= this->DWallet ) {
                        this->DWallet -= this->Cart.sum_of_prices() ;
                        for ( const auto &i : this->Cart ) {
                            for ( auto j : p ) {
                                if (i.ID == j.ID ){
                                    j.Number -= i.Number ;

                                }
                            }
                        }
                        invoice x (this->Cart , this->Username , this->Cart.sum_of_prices() , 0 , 0 ) ;
                        this->Orders.push_back(x) ;
                        admin.Orders.push_back(x) ;
                        this->Cart.clear() ;
                        green ;
                        cout << "Payment is seccessful !" << endl ;
                    }
                    else {
                        red ;
                        cout << "Insufficient inventory !" << endl ; // not enough
                    }
                    break ;
                }
                case 2 : {
                    if (a.tabdil_DE(this->Cart.sum_of_prices()) <= this->EWallet ) {
                        this->EWallet -= a.tabdil_DE(this->Cart.sum_of_prices()) ;
                        for ( const auto &i : this->Cart ) {
                            for ( auto j : p ) {
                                if (i.ID == j.ID ){
                                    j.Number -= i.Number ;
                                }
                            }
                        }
                        invoice x (this->Cart , this->Username , 0 , a.tabdil_DE(this->Cart.sum_of_prices()) , 0 ) ;
                        this->Orders.push_back(x) ;
                        admin.Orders.push_back(x) ;
                        this->Cart.clear() ;
                        green ;
                        cout << "Payment is seccessful !" << endl ;
                    }
                    else {
                        red ;
                        cout << "Insufficient inventory !" << endl ;
                    }
                    break ;
                }
                case 3 : {
                    if (a.tabdil_DR(this->Cart.sum_of_prices()) <= this->RWallet ) {
                        this->RWallet -= a.tabdil_DR(this->Cart.sum_of_prices()) ;
                        for ( const auto &i : this->Cart ) {
                            for ( auto j : p ) {
                                if (i.ID == j.ID ){
                                    j.Number -= i.Number ;
                                }
                            }
                        }
                        invoice x (this->Cart , this->Username , 0 , 0 , a.tabdil_DR(this->Cart.sum_of_prices()) ) ;
                        this->Orders.push_back(x) ;
                        admin.Orders.push_back(x) ;
                        this->Cart.clear() ;
                        green ;
                        cout << "Payment is seccessful !" << endl ;
                    }
                    else {
                        red ;
                        cout << "Insufficient inventory !" << endl ;
                    }
                    break ;
                }
                default: {
                    red ;
                    cout << "Command not found !" << endl ;
                }
                }
            }
        }
    }
    void edit_cart () {
        yelow ;
        cout << "Enter the product ID : " ;
        long long id ;
        cin >> id ;
        cor;
        for ( auto i : (this->Cart) ){
            if ( i.ID == id ){
                cout << "Enter new number of this product : " ;
                int num ;
                cin >> num ;
                cor;
                i.Number = num ;
                green ;
                cout << "Edited !" << endl ;
                return;
            }
        }
        red ;
        cout << "ID not found !" << endl ;
    }
    void increase_credit () {
        yelow ;
        cout << "Commands :" << endl << endl ;
        cout << "1 - Increase USD credit" << endl ;
        cout << "2 - Increase EUR credit" << endl ;
        cout << "3 - Increase IRR credit" << endl ;
        cout << endl ;
        cout << "Enter command number : " ;
        int c ; // command number
        cin >> c ;
        isint ;
        switch (c) {
        case 1 : {
            cout << "Enter the desired amount (USD) : " ;
            double m ;
            cin >> m ;
            cor;
            this->DWallet += m ;
            break ;
        }
        case 2 : {
            cout << "Enter the desired amount (EUR) : " ;
            double m ;
            cin >> m ;
            cor;
            this->EWallet += m ;
            break ;
        }
        case 3 : {
            cout << "Enter the desired amount (IRR) : " ;
            double m ;
            cin >> m ;
            cor;
            this->RWallet += m ;
            break ;
        }
        default: {
            red ;
            cout << "Command not found !" << endl ;
        }
        }
    }

};
//------------------------------------------------------------------------
class uvec : public vector<user> {
public:
    // search by username and password
    bool search ( string u , string p ) {
        for ( const auto &i : (*this) )
            if ( i.Username == u )
                if (i.Password == p )
                    return true ;
        return false ;
    }
    // search by username
    bool search_username ( string u ) {
        for ( const auto &i : (*this) )
            if ( i.Username == u )
                return true ;
        return false ;
    }
    void sign_up () {
        yelow;
        cout << "Enter username : " ;
        string u ;
        cin >> u ;
        cout << endl ;
        cout << "Enter password : " ;
        string p ;
        cin >> p ;
        cout << endl ;
        if ( this->search_username(u) ) {
            red;
            cout << "Duplicate username !" << endl ;
        }
        else {
            user a(u,p) ;
            this->push_back(a) ;
            green;
            cout << "Sign up is successful !" << endl ;
        }
    }
    user * log_in () {
        yelow ;
        cout << "Enter username : " ;
        string u ;
        cin >> u ;
        cout << endl ;
        cout << "Enter password : " ;
        string p ;
        cin >> p ;
        cout << endl ;
        for (unsigned int i = 0 ; i < this->size() ; i ++ ) {
            if ( (*this)[i].Username == u )
                if ( (*this)[i].Password == p ){
                    green ;
                    cout << "Log in was successfu !" << endl ;
                    return &((*this)[i]);
                }
        }
        red ;
        cout << "Log in failed !" << endl ;
        return nullptr ;
    }
    void delete_account () {
        yelow ;
        cout << "Enter username : " ;
        string u ;
        cin >> u ;
        cout << endl ;
        cout << "Enter password : " ;
        string p ;
        cin >> p ;
        cout << endl ;
        for (uvec::iterator i = this->begin() ; i != this->end() ; i++ ) {
            if ( i->Username == u )
                if ( i->Password == p ){
                    this->erase(i) ;
                    green ;
                    cout << "Deleted !" << endl ;
                    return;
                }
        }
        red ;
        cout << "Account not found !" ;
    }
    void change_password (user * t) {
        yelow ;
        cout << "Enter password : " ;
        string p ;
        cin >> p ;
        if ( t->Password == p ) {
            cout << "Enter new password : " ;
            string np ;
            cin >> np ;
            t->Password = np ;
            green ;
            cout << "Changed !" << endl ;
            return;
        }
        red ;
        cout << "Password is not correct !" << endl ;
    }
    // print all profile users
    void u_print () {
        if (this->empty()) {
            red ;
            cout << "Empty !" << endl ;
        }
        else {
            int j = 1 ;
            for ( const auto &i : (*this) ) {
                if ( i.Username != "admin" ) {
                    cout << j << ")" << endl ;
                    cout << "Username : " << i.Username << endl ;
                    cout << "Password : " << i.Password << endl ;
                    cout << "USD Wallet : " << i.DWallet << endl ;
                    cout << "EUR Wallet : " << i.EWallet << endl ;
                    cout << "IRR Wallet : " << i.RWallet << endl ;
                    j ++ ;
                }
            }
        }
    }

};


//------------------------------------------------------------------------
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//------------------------------------------------------------------------
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//------------------------------------------------------------------------

int main()
{
    return 0;
}
