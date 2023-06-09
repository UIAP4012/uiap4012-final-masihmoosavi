#include <iostream>
#include <windows.h>
#include <string>
#include <set>
#include <vector>
#include <ctime>
#include <fstream>


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

//------------------------------------------------------------------------
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

    MoneyValue () {
        ifstream m ("moneyvalue.txt") ;
        if (m.is_open()) {
            if (m.peek()  == ifstream::traits_type::eof() ) {
                return;
            }
            else {
                m >> IRR ;
                m >> EUR ;
                m.close() ;
            }
        }
    }

    ~MoneyValue () {
        ofstream m ("moneyvalue.txt" , ios::trunc) ;
        if (m.is_open()) {
            m << IRR << endl ;
            m << EUR << endl ;
            m.close() ;
        }
    }


};
//------------------------------------------------------------------------
class product {
public:
    long long ID ;
    string Name ;
    int Number ;
    double Price ; // USD price
    void print ( MoneyValue a ) {
        white ;
        cout << "ID : " << ID << endl ;
        cout << "Name : " << Name << endl ;
        cout << "Number : " << Number << endl ;
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
        white ;
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
            cout << "Product ID is duplicate !" << endl ;
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
                cout << "Number : " << i.Number << endl ;
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
        bool is_find = 0 ;
        for ( auto i : (*this) )
            if ( i.Name == name ) {
                white ;
                i.print(a) ;
                is_find = 1 ;
            }
        if (is_find == 0) {
            red ;
            cout << "Not found !" << endl ;
        }
    }
    // sum of USD prices of produts that is in Cart
    double sum_of_prices () {
        double sum = 0 ;
        for ( const auto &i : (*this) )
            sum += i.Price*i.Number ;
        return sum ;
    }

    void pset_read () {
        ifstream p ("pset.txt") ;
        if (p.is_open()) {
            if (p.peek()  == ifstream::traits_type::eof() ) {
                return;
            }
            else {
                while (! p.eof()) {
                    product t ;
                    p >> t.ID ;
                    p >> t.Name ;
                    p >> t.Number ;
                    p >> t.Price ;
                    this->insert(t) ;
                }
            }
            p.close() ;
        }
    }

    void pset_save () {
        ofstream p ("pset.txt" , ios::trunc) ;
        if (p.is_open()) {
            for ( const auto &i : (*this)) {
                p << endl << i.ID << endl ;
                p << i.Name << endl ;
                p << i.Number << endl ;
                p << i.Price  ;
            }
            p.close() ;
        }
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
    invoice () {};
    void print_csv () {
        ofstream f ("invoice.csv" , ios::app) ;
        if (f.is_open()) {
            f << "Sales Pro," ;
            f << Username_of_buyer << "," ;
            f << local_time->tm_mday << "/"
              << local_time->tm_mon+1 << "/" << local_time->tm_year+1900 << "," ;
            f << local_time->tm_hour << ":" << local_time->tm_min << "," ;
            f << DPayment_amount << "," ;
            f << EPayment_amount << "," ;
            f << RPayment_amount << "," ;
            for (pset::iterator i = O.begin() ; i != O.end() ; i ++ ) {
                f << i->ID << "/" ;
                f << i->Name << "/" ;
                f << i->Number << "/" ;
                f << i->Price ;
                f << "," ;
                f << endl ;
                f << ",,,,,,," ;
            }
            f << endl ;
            f.close() ;
        }
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
                cout << "Date : " << localtime(&(i.now))->tm_mday << "/"
                     << localtime(&(i.now))->tm_mon+1 << "/" << localtime(&(i.now))->tm_year+1900 << endl ;
                cout << "Time : " << localtime(&(i.now))->tm_hour << ":" << localtime(&(i.now))->tm_min << endl ;
                for ( const auto &j : i.O ) {
                    cout << "ID : " << j.ID << "        "
                         << "Name : " << j.Name << "        "
                         << "Price (USD) : " << j.Price << "          "
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
    user () {
        DWallet = 0 ;
        EWallet = 0 ;
        RWallet = 0 ;
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
                    if (num == 0) {
                        red ;
                        cout << "Error !" << endl ;
                        return;
                    }
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
            bool r = 0 ; // be remove ?
            bool b = 1 ; // be buy ?
            product l ;
            for ( const auto &i : this->Cart ) {
                if (r == 1) {
                    red ;
                    cout << "The product with ID " << l.ID << " is no longer available !" << endl ;
                    cout << "The product with ID " << l.ID << " removed from your Cart !" << endl ;
                    this->Cart.erase(l) ;
                }
                r = 1 ;
                l = i ;
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

            }
            if (r == 1) {
                red ;
                cout << "The product with ID " << l.ID << " is no longer available !" << endl ;
                cout << "The product with ID " << l.ID << " removed from your Cart !" << endl ;
                this->Cart.erase(l) ;
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
                    product y ;
                    product z ;
                    y.ID = 0 ;
                    z.ID = 0 ;
                    if (this->Cart.sum_of_prices() <= this->DWallet ) {
                        this->DWallet -= this->Cart.sum_of_prices() ;
                        for ( const auto &i : this->Cart ) {
                            if (z.ID != 0 && y.ID != 0 ) {
                                p.erase(y) ;
                                p.insert(z) ;
                            }
                            for ( const auto &j : p ) {
                                if (i.ID == j.ID ){
                                    y = j ;
                                    z.ID = j.ID ;
                                    z.Name = j.Name ;
                                    z.Number = (j.Number) - (i.Number) ;
                                    z.Price = j.Price ;
                                }
                            }
                        }
                        p.erase(y) ;
                        p.insert(z) ;
                        invoice x (this->Cart , this->Username , this->Cart.sum_of_prices() , 0 , 0 ) ;
                        this->Orders.push_back(x) ;
                        admin.Orders.push_back(x) ;
                        x.print_csv() ;
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
                    product y ;
                    product z ;
                    y.ID = 0 ;
                    z.ID = 0 ;
                    if (a.tabdil_DE(this->Cart.sum_of_prices()) <= this->EWallet ) {
                        this->EWallet -= a.tabdil_DE(this->Cart.sum_of_prices()) ;
                        for ( const auto &i : this->Cart ) {
                            if (z.ID != 0 && y.ID != 0 ) {
                                p.erase(y) ;
                                p.insert(z) ;
                            }
                            for ( pset::iterator j = p.begin() ; j != p.end() ; j++ ) {
                                if (i.ID == j->ID ){
                                    y = (*j) ;
                                    z.ID = j->ID ;
                                    z.Name = j->Name ;
                                    z.Number = (j->Number) - (i.Number) ;
                                    z.Price = j->Price ;
                                }
                            }
                        }
                        p.erase(y) ;
                        p.insert(z) ;
                        invoice x (this->Cart , this->Username , 0 , a.tabdil_DE(this->Cart.sum_of_prices()) , 0 ) ;
                        this->Orders.push_back(x) ;
                        admin.Orders.push_back(x) ;
                        x.print_csv() ;
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
                    product y ;
                    product z ;
                    y.ID = 0 ;
                    z.ID = 0 ;
                    if (a.tabdil_DR(this->Cart.sum_of_prices()) <= this->RWallet ) {
                        this->RWallet -= a.tabdil_DR(this->Cart.sum_of_prices()) ;
                        for ( const auto &i : this->Cart ) {
                            if (z.ID != 0 && y.ID != 0 ) {
                                p.erase(y) ;
                                p.insert(z) ;
                            }
                            for ( pset::iterator j = p.begin() ; j != p.end() ; j++ ) {
                                if (i.ID == j->ID ){
                                    y = (*j) ;
                                    z.ID = j->ID ;
                                    z.Name = j->Name ;
                                    z.Number = (j->Number) - (i.Number) ;
                                    z.Price = j->Price ;
                                }
                            }
                        }
                        p.erase(y) ;
                        p.insert(z) ;
                        invoice x (this->Cart , this->Username , 0 , 0 , a.tabdil_DR(this->Cart.sum_of_prices()) ) ;
                        this->Orders.push_back(x) ;
                        admin.Orders.push_back(x) ;
                        x.print_csv() ;
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
        bool e = 0 ; // be edited ?
        product z ;
        product y ;
        for ( const auto &i : (this->Cart) ){
            if ( i.ID == id ){
                cout << "Enter new number of this product : " ;
                int num ;
                cin >> num ;
                cor;
                e = 1 ;
                if (num == 0) {
                    red ;
                    cout << "Error !" << endl ;
                    return;
                }
                y = i ;
                z.ID = i.ID ;
                z.Name = i.Name ;
                z.Number = num ;
                z.Price = i.Price ;
            }
        }
        if (e == 1) {
            this->Cart.erase(y) ;
            this->Cart.insert(z) ;
            green ;
            cout << "Edited !" << endl ;
            return;
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
        cout << "Enter 0 to Return ." << endl ;
        cout << "Enter username : " ;
        string u ;
        cin >> u ;
        if ( u == "0" ) {
            return;
        }
        cout << endl ;
        cout << "Enter password : " ;
        string p ;
        cin >> p ;
        if ( p == "0" ) {
            return;
        }
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
        cout << "Enter 0 to Return ." << endl ;
        cout << "Enter username : " ;
        string u ;
        cin >> u ;
        if ( u == "0" ) {
            return nullptr;
        }
        cout << endl ;
        cout << "Enter password : " ;
        string p ;
        cin >> p ;
        if ( p == "0" ) {
            return nullptr;
        }
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
        white ;
        if (this->size() <= 1 ) {
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

    void uvec_save () {
        ofstream u ("uvec.txt" , ios::trunc) ;
        if (u.is_open()) {
            for ( const auto &i : (*this)) {
                u << endl << i.Username << endl ;
                u << i.Password << endl ;
                u << i.DWallet << endl ;
                u << i.EWallet << endl ;
                u << i.RWallet << endl ;
                u << i.Cart.size() ;
                for ( const auto &j : i.Cart ){
                    u << endl << j.ID << endl ;
                    u << j.Name << endl ;
                    u << j.Number << endl ;
                    u << j.Price ;
                }
                u << endl << i.Orders.size() ;
                for ( const auto &j : i.Orders ){
                    u << endl << j.Username_of_buyer << endl ;
                    u << j.now << endl ;
                    u << j.DPayment_amount << endl ;
                    u << j.EPayment_amount << endl ;
                    u << j.RPayment_amount << endl ;
                    u << j.O.size() ;
                    for (const auto &k : j.O) {
                        u << endl << k.ID << endl ;
                        u << k.Name << endl ;
                        u << k.Number << endl ;
                        u << k.Price ;
                    }
                }
            }
            u.close() ;
        }
    }

    void uvec_read () {
        ifstream u ("uvec.txt") ;
        if (u.is_open()) {
            if (u.peek()  == ifstream::traits_type::eof() ) {
                return;
            }
            else {
                while (u.peek() != EOF ) {
                    user t ;
                    int c ; // count Cart
                    u >> t.Username ;
                    u >> t.Password ;
                    u >> t.DWallet ;
                    u >> t.EWallet ;
                    u >> t.RWallet ;
                    u >> c ;
                    if (c == 0) {
                        t.Cart.clear() ;
                    }
                    for (int i = 0 ; i < c ; i++ ) {
                        product a ;
                        u >> a.ID ;
                        u >> a.Name ;
                        u >> a.Number ;
                        u >> a.Price ;
                        t.Cart.insert(a) ;
                    }
                    int g ; //count Orders
                    u >> g ;
                    if (g == 0) {
                        t.Orders.clear() ;
                    }
                    for (int i = 0 ; i < g ; i++ ) {
                        invoice w ;
                        u >> w.Username_of_buyer ;
                        u >> w.now ;
                        u >> w.DPayment_amount ;
                        u >> w.EPayment_amount ;
                        u >> w.RPayment_amount ;
                        int h ; // count w.O
                        u >> h ;
                        for (int j = 0 ; j < h ; j++ ) {
                            product s ;
                            u >> s.ID ;
                            u >> s.Name ;
                            u >> s.Number ;
                            u >> s.Price ;
                            w.O.insert(s) ;
                        }
                        t.Orders.push_back(w) ;
                    }
                    this->push_back(t) ;
                }
            }
            u.close() ;
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
    MoneyValue money ;
    pset p ; // set <product>
    p.pset_read() ;
    uvec u ; // vector <user>
    u.uvec_read() ;
    user * q ;
    for ( uvec::iterator i = u.begin() ; i != u.end() ; i++ ) {
        if ( i->Username == "admin" )
            q = &(*i) ;
    }
    bool end = 0 ;
    int com ; // command number
    cout << "SALES PRO" << endl ;
    while (true) {
        blue ;
        cout << endl << endl << endl << endl << endl << endl << endl ;
        cout << "Commands :" << endl << endl ;
        cout << "1 - Sign up" << endl ;
        cout << "2 - Log in" << endl ;
        cout << "0 - Exit" << endl ;
        cout << endl ;
        cout << "Enter command number : " ;
        cin >> com ;
        isintc ;
        cout << endl ;
        switch (com) {
        case 0 : {
            end = 1 ;
            break;
        }
        case 1 : {
            system ("cls") ;
            u.sign_up() ;
            break;
        }
        case 2 : {
            system ("cls") ;
            user * t = u.log_in()  ;
            if ( t != nullptr) {
                // users --------------------------------------------------
                if ( t->Username != "admin" ) {
                    while (true) {
                        blue ;
                        int c ; // command number
                        cout << endl << endl << endl ;
                        cout << "Commands :" << endl << endl ;
                        cout << "1 - Profile" << endl ;
                        cout << "2 - Change Password" << endl ;
                        cout << "3 - Delete Account" << endl ;
                        cout << "4 - Show All Products" << endl ;
                        cout << "5 - Search Product" << endl ;
                        cout << "6 - Add to Cart" << endl ;
                        cout << "7 - Show Cart Products" << endl ;
                        cout << "8 - Remove from Cart" << endl ;
                        cout << "9 - Edit Number of Product in Cart " << endl ;
                        cout << "10 - Search in The Cart" << endl ;
                        cout << "11 - Payment" << endl ;
                        cout << "12 - History of Orders and Invoices" << endl ;
                        cout << "13 - Increase Credit" << endl ;
                        cout << "0 - Exit" << endl ;
                        cout << endl ;
                        cout << "Enter command number : " ;
                        cin >> c ;
                        isintc ;
                        cout << endl ;
                        switch (c) {
                        case 0 : {
                            end = 1 ;
                            break;
                        }
                        case 1 : {
                            system ("cls") ;
                            yelow ;
                            cout << "Username : " << t->Username << endl ;
                            cout << "USD Wallet : " << t->DWallet << endl ;
                            cout << "EUR Wallet : " << t->EWallet << endl ;
                            cout << "IRR Wallet : " << t->RWallet << endl ;
                            break ;
                        }
                        case 2 : {
                            system ("cls") ;
                            u.change_password(t) ;
                            break ;
                        }
                        case 3 : {
                            system ("cls") ;
                            u.delete_account() ;
                            end = 1 ;
                            break ;
                        }
                        case 4 : {
                            system ("cls") ;
                            p.print(money) ;
                            break ;
                        }
                        case 5 : {
                            system ("cls") ;
                            p.search_name(money) ;
                            break ;
                        }
                        case 6 : {
                            system ("cls") ;
                            t->add_product_id(p) ;
                            break ;
                        }
                        case 7 : {
                            system ("cls") ;
                            t->Cart.print(money) ;
                            break ;
                        }
                        case 8 : {
                            system ("cls") ;
                            t->Cart.print(money) ;
                            cout << endl ;
                            t->Cart.remove_product() ;
                            break ;
                        }
                        case 9 : {
                            system ("cls") ;
                            t->Cart.print(money) ;
                            cout << endl ;
                            t->edit_cart() ;
                            break ;
                        }
                        case 10 : {
                            system ("cls") ;
                            t->Cart.search_name(money) ;
                            break ;
                        }
                        case 11 : {
                            system ("cls") ;
                            t->payment(money,p,(*q)) ;
                            break ;
                        }
                        case 12 : {
                            system ("cls") ;
                            t->Orders.i_print() ;
                            break ;
                        }
                        case 13 : {
                            system ("cls") ;
                            t->increase_credit() ;
                            break ;
                        }
                        default: {
                            red ;
                            cout << "Command not found !" << endl ;
                        }
                        }
                        if ( end == 1 )
                            break;
                    }
                }
                //admin ----------------------------------------------------
                else {
                    while (true) {
                        int c ; // command number
                        blue ;
                        cout << endl << endl << endl ;
                        cout << "Commands :" << endl << endl ;
                        cout << "1 - Add Product" << endl ;
                        cout << "2 - Remove Product" << endl ;
                        cout << "3 - Edit Product" << endl ;
                        cout << "4 - Search Product" << endl ;
                        cout << "5 - Show All Product" << endl ;
                        cout << "6 - Show Users Profile" << endl ;
                        cout << "7 - Set the Money Value" << endl ;
                        cout << "8 - Invoice of Products Sold" << endl ;
                        cout << "0 - Exit" << endl ;
                        cout << endl ;
                        cout << "Enter command number : " ;
                        cin >> c ;
                        isintc ;
                        cout << endl ;
                        switch (c) {
                        case 0 : {
                            end = 1 ;
                            break;
                        }
                        case 1 : {
                            system ("cls") ;
                            p.add_product() ;
                            break ;
                        }
                        case 2 : {
                            system ("cls") ;
                            p.remove_product() ;
                            break ;
                        }
                        case 3 : {
                            system ("cls") ;
                            p.edit_product() ;
                            break ;
                        }
                        case 4 : {
                            system ("cls") ;
                            p.search_name(money) ;
                            break ;
                        }
                        case 5 : {
                            system("cls") ;
                            p.print(money) ;
                            break ;
                        }
                        case 6 : {
                            system ("cls") ;
                            u.u_print() ;
                            break ;
                        }
                        case 7 : {
                            system ("cls") ;
                            money.update_price() ;
                            break ;
                        }
                        case 8 : {
                            system ("cls") ;
                            q->Orders.i_print() ;
                            q->Orders.total_incom() ;
                            break ;
                        }
                        default: {
                            red ;
                            cout << "Command not found !" << endl ;
                        }
                        }
                        if ( end == 1 )
                            break;
                    }
                }
            }
            break;
        }
        default: {
            red ;
            cout << "Command not found !" << endl ;
        }
        }
        if ( end == 1 )
            break;
    }
    u.uvec_save() ;
    p.pset_save() ;
    return 0;
}
