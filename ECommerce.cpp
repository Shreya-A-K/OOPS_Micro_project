#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
using namespace std;

//USER BASE CLASS 
class User {
protected:
    string username;
    string password;
    string role; // "buyer" or "seller"
public:
    User(string u="", string p="", string r=""){
        username=u;
        password=p;
        role=r;
    } 

    string getRole() { 
        return role; 
}
    string getUsername() { 
        return username; 
}
    string getPassword() {
         return password; 
}
};


// BUYER CLASS 
class Buyer : public User {
public:
    Buyer(string u="", string p=""): User(u,p,"buyer") {}

    void viewProducts(const vector<class Product> &products) {
        if (products.empty()) { cout << "No products available.\n"; return; }
        cout << "\n=== Product List ===\n";
        for (auto &p : products) {
            cout << "ID: " << p.getID() << " | " << p.getName() 
                 << " | Price: " << p.getPrice() << " | Qty: " << p.getStock() << endl;
        }
    }
};



//SELLER CLASS
class Seller : public User {
public:
    Seller(string u="", string p=""): User(u,p,"seller") {}

    void addProduct(vector<class Product> &products, const string &id, const string &name, double price, int qty);
    void viewProducts(const vector<class Product> &products);
};



// PRODUCT CLASS
class Product {
private:
    static int index;
    string productID;
    string name;
    double price;
    int stock;
public:
    Product( string n="", double p=0, int s=0){
        index+=1;
        productID=to_string(index)+n;
        name=n;
        price=p;
        stock=s;
    }

    string getID() const { return productID; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    void setStock(int s) { stock = s; }
};

int Product::index =0;

void Seller::addProduct(vector<Product> &products, const string &id, const string &name, double price, int qty) {
    if (id.empty() || name.empty() || price < 0 || qty <= 0)
        throw runtime_error("Invalid product details.");
    products.push_back(Product(id, name, price, qty));
    cout << "Product added successfully.\n";
}

void Seller::viewProducts(const vector<Product> &products) {
    if (products.empty()) { cout << "No products available.\n"; return; }
    cout << "\n=== Seller Products ===\n";
    for (auto &p : products) {
        cout << "ID: " << p.getID() << " | " << p.getName() 
             << " | Price: " << p.getPrice() << " | Qty: " << p.getStock() << endl;
    }
}

// CART CLASS 
class Cart {
private:
    struct CartItem {
        string productID;
        string name;
        double price;
        int quantity;
    };
    vector<CartItem> items;
public:
    void addToCart(vector<Product> &products, const string &pid, int qty) {
        if (pid.empty() || qty <= 0) throw runtime_error("Invalid input.");

        int idx = -1;
        for (size_t i=0; i<products.size(); i++)
            if (products[i].getID() == pid) { idx = i; break; }
        if (idx == -1) throw runtime_error("Product not found.");
        if (products[idx].getStock() < qty) throw runtime_error("Insufficient stock.");

        products[idx].setStock(products[idx].getStock() - qty);

        CartItem ci{products[idx].getID(), products[idx].getName(), products[idx].getPrice(), qty};
        items.push_back(ci);
        cout << ci.name << " x" << qty << " added to cart.\n";
    }

    void showCart() {
        if (items.empty()) { cout << "Cart is empty.\n"; return; }
        cout << "\n=== Cart Items ===\n";
        for (auto &it : items) {
            cout << "ID: " << it.productID << " | " << it.name 
                 << " | Price: " << it.price << " | Qty: " << it.quantity << endl;
        }
    }

    double totalAmount() {
        double t=0;
        for (auto &it : items) t += it.price * it.quantity;
        return t;
    }

    bool empty() const { return items.empty(); }

    void clear() { items.clear(); }

    void printBill(const string &buyer) {
        if (items.empty()) throw runtime_error("Cart empty.");
        cout << "\n----- BILL for " << buyer << " -----\n";
        double total=0;
        for (auto &it : items) {
            double sub = it.price * it.quantity;
            cout << it.name << " | " << it.quantity << " x " << it.price << " = " << sub << endl;
            total += sub;
        }
        cout << "---------------------\nTotal: " << total << endl;
    }
};

//PAYMENT CLASSES
class Payment {
protected:
    double amount;
public:
    Payment(double a=0): amount(a) {}
    virtual void pay() { cout << "Processing payment...\n"; }
};

class CreditCardPayment : public Payment {
    string cardNumber;
public:
    CreditCardPayment(double amt, string card): Payment(amt), cardNumber(card) {}
    void pay() override {
        if (cardNumber.size() < 8) throw runtime_error("Invalid card number.");
        cout << "Credit Card Payment of " << amount << " done using " << cardNumber << endl;
    }
};

class UPIPayment : public Payment {
    string upiID;
public:
    UPIPayment(double amt, string id): Payment(amt), upiID(id) {}
    void pay() override {
        if (upiID.find('@') == string::npos) throw runtime_error("Invalid UPI ID.");
        cout << "UPI Payment of " << amount << " done via " << upiID << endl;
    }
};

int main() {
    vector<User> users;
    vector<Product> products;

    int mainChoice;
    string uname, pass;

    while (true) {
        cout << "\n=== Welcome to E-Commerce ===\n";
        cout << "1. Create Account\n2. Login\n3. Exit\nChoice: ";
        if (!(cin >> mainChoice)) break;

        if (mainChoice == 1) {
            string role;
            cout << "Enter username: "; cin >> uname;
            cout << "Enter password: "; cin >> pass;
            cout << "Role (buyer/seller): "; cin >> role;
            if (role!="buyer" && role!="seller") { cout << "Invalid role.\n"; continue; }
            users.push_back(User(uname, pass, role));
            cout << "Account created.\n";
        }
        else if (mainChoice == 2) {
            cout << "Username: "; cin >> uname;
            cout << "Password: "; cin >> pass;
            User *logged=nullptr;
            for (auto &u: users) {
                if (u.getUsername()==uname && u.getPassword()==pass) { logged=&u; break; }
            }
            if (!logged) { cout << "Invalid login.\n"; continue; }

            cout << "Welcome " << uname << " (" << logged->getRole() << ")\n";

            if (logged->getRole()=="buyer") {
                Buyer b(uname,pass);
                Cart cart;
                int c;
                do {
                    cout << "\n=== Buyer Menu ===\n1.View Products\n2.Add to Cart\n3.View Cart\n4.Print Bill\n5.Place Order\n6.Logout\nChoice: ";
                    cin >> c;
                    if (c==1) b.viewProducts(products);
                    else if (c==2) {
                        string id; int q;
                        cout<<"Enter product ID: ";cin>>id;
                        cout<<"Quantity: ";cin>>q;
                        try { cart.addToCart(products,id,q);} catch(exception &e){cout<<e.what()<<endl;}
                    }
                    else if (c==3) cart.showCart();
                    else if (c==4) try { cart.printBill(uname);} catch(exception&e){cout<<e.what()<<endl;}
                    else if (c==5) {
                        if (cart.empty()) {cout<<"Cart empty.\n"; continue;}
                        cart.printBill(uname);
                        cout<<"Choose payment: 1.Credit Card 2.UPI\n";int p;cin>>p;
                        double amt=cart.totalAmount();
                        try {
                            if (p==1) {string card; cout<<"Card: ";cin>>card; CreditCardPayment cc(amt,card);cc.pay();}
                            else if (p==2){string id; cout<<"UPI: ";cin>>id; UPIPayment up(amt,id);up.pay();}
                            else {cout<<"Invalid.\n";continue;}
                            cout<<"Order placed!\n"; cart.clear();
                        } catch(exception&e){cout<<e.what()<<endl;}
                    }
                } while (c!=6);
            } else {
                Seller s(uname,pass);
                int c;
                do {
                    cout << "\n=== Seller Menu ===\n1.Add Product\n2.View Products\n3.Logout\nChoice: ";
                    cin >> c;
                    if (c==1) {
                        string id,n;double p;int q;
                        cout<<"Enter id name price qty: ";cin>>id>>n>>p>>q;
                        try{s.addProduct(products,id,n,p,q);}catch(exception&e){cout<<e.what()<<endl;}
                    } else if (c==2) s.viewProducts(products);
                } while (c!=3);
            }
        }
        else if (mainChoice == 3) { cout << "Goodbye!\n"; break; }
    }
    return 0;
}

