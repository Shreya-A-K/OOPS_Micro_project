#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

// PRODUCT CLASS
class Product
{
public:
    static int index;
    string productID;
    string name;
    double price;
    int stock;

    Product(string n = "", double p = 0, int s = 0)
    {
        index += 1;
        productID = to_string(index) + n; // auto ID
        name = n;
        price = p;
        stock = s;
    }

    void showProduct()
    {
        cout << "ID: " << productID
             << " | " << name
             << " | Price: " << price
             << " | Stock: " << stock << endl;
    }
};

int Product::index = 0;

// CART CLASS
class Cart
{
    vector<Product*>products_in_cart;
    vector <int> quantities;

public:
    void addToCart(vector<Product> &products, const string &pid, int qty)
    {
        if (pid.empty() || qty <= 0)
        {
            throw runtime_error("Invalid input.");
        }

        int idx = -1;
        for (size_t i = 0; i < products.size(); i++)
            if (products[i].productID == pid)
            {
                idx = i;
                break;
            }

        if (idx == -1) throw runtime_error("Product not found!");
        if (products[idx].stock < qty) {
            throw runtime_error("Insufficient stock!!");
        }

        products[idx].stock=(products[idx].stock - qty);
        quantities.push_back(qty);

        products_in_cart.push_back(&products[idx]);
        
        cout << products_in_cart.back()->name << " x" << qty << " Added to cart!\n";
    }

    void showCart()
    {
        if (products_in_cart.empty())
        {
            cout << "Cart is empty!\n";
            return;
        }
        cout << "\n=== Cart Items ===\n";
        for (size_t i = 0; i < products_in_cart.size(); i++)
        {
            cout << "ID: " << products_in_cart[i]->productID
                 << " | Name: " << products_in_cart[i]->name
                 << " | Price: " << products_in_cart[i]->price
                 << " | Qty: " << quantities[i] << endl;
        }
    }

    double totalAmount()
    {
        double t = 0;
        for (size_t i = 0; i < products_in_cart.size(); i++)
            t += products_in_cart[i]->price * quantities[i];
        return t;
    }

    bool empty() const {
        return products_in_cart.empty(); 
    }

    void clear()
    {
        products_in_cart.clear();
        quantities.clear();
    }

    void printBill(const string &buyer)
    {
        if (products_in_cart.empty()) throw runtime_error("Cart empty!");
        cout << "\n----- BILL for " << buyer << " -----\n";
        double total = 0;
        for (size_t i = 0; i < products_in_cart.size(); i++)
        {
            double sub = products_in_cart[i]->price * quantities[i];
            cout << products_in_cart[i]->name << " | " << quantities[i]
                 << " x " << products_in_cart[i]->price << " = " << sub << endl;
            total += sub;
        }
        cout << "---------------------\nTotal: " << total << endl;
    }
};

// USER CLASS
class User
{
protected:
    string username;
    string password;
    bool loggedIn; // added

public:
    static int userid;
    User(string u = "", string p = "") {
        userid++;
        username=to_string(userid)+u;
        password=p; 
        loggedIn=false;
        cout<<"Created an account Successfully!!"<<endl;
        cout<<"Your username is "<<username<<endl;
    }

    string getUsername() { 
        return username; 
    }

    bool login(string u, string p)
    {
        if (username == u && password == p) {
            loggedIn = true;
            return true;
        }
        return false;
    }

    void logout() {
        if (loggedIn) {
            loggedIn = false;
            cout << username << " logged out.\n";
        }
    }

    bool isLoggedIn() const {
        return loggedIn;
    }
};
int User::userid=0;

// SELLER CLASS
class Seller : public User
{
public:
    Seller(string u = "", string p = "") : User(u, p) {}

    void addProduct(vector<Product> &products, const string &name, double price, int qty)
    {
        if (name.empty() || price < 0 || qty <= 0)
        {
            throw runtime_error("Invalid product details!!");
        }
        products.push_back(Product(name, price, qty)); // auto ID
        cout << "Product added successfully!\n";
    }

    void viewProducts(vector<Product> &products)
    {
        if (products.empty())
        {
            cout << "No products available!\n";
            return;
        }
        cout << "\n=== Product List ===\n";
        for (auto &p : products) p.showProduct();
    }
};

// BUYER CLASS
class Buyer : public User
{
    Cart cart;

public:
    Buyer(string u = "", string p = "") : User(u, p) {}

    void addToCart(vector<Product> &products, const string &pid, int qty)
    {
        cart.addToCart(products, pid, qty);
    }

    void viewCart()
    {
        cart.showCart();
    }

    void checkout()
    {
        if (cart.empty()) throw runtime_error("Cart is empty!");
        cart.printBill(username);
        cart.clear();
    }
};

// MAIN
int main()
{
    vector<Product> products;
    products.reserve(1000);
    vector<Seller> sellers;
    vector<Buyer> buyers;

    while (true)
    {   int choice;
        cout<<"\n1. New User \n2. Existing user?: ";
        cin>>choice;
        if (choice==1){
            int type;
            cout << "Register as 1.Seller 2.Buyer: ";
            cin >> type;
            string em;
            string pw;
            cout<<"Enter your emailID:";
            cin>>em;
            cout<<"Enter your password!:";
            cin>>pw;

            if(type == 1)
                sellers.push_back(Seller(em, pw));
            else if(type == 2)
                buyers.push_back(Buyer(em, pw));
        }
        cout << "\n1. Seller Login\n2. Buyer Login\n3. Exit\nChoice: ";
        int ch;
        cin >> ch;
        if (ch == 3) break;

        string u, p;
        cout << "Username: ";
        cin >> u;
        cout << "Password: ";
        cin >> p;

        if (ch == 1) // seller login
        {
            bool valid = false;
            for (auto &s : sellers)
            {
                if (s.login(u, p))
                {
                    valid = true;
                    cout << "Seller logged in.\n";
                    int c;
                    while (s.isLoggedIn())
                    {
                        cout << "\n1.Add Product\n2.View Products\n3.Logout\nChoice: ";
                        cin >> c;
                        if (c == 1)
                        {
                            string n;
                            double pr;
                            int q;
                            cout << "Enter name price qty: ";
                            cin >> n >> pr >> q;
                            try
                            {
                                s.addProduct(products, n, pr, q);
                            }
                            catch (exception &e)
                            {
                                cout << e.what() << endl;
                            }
                        }
                        else if (c == 2)
                        {
                            s.viewProducts(products);
                        }
                        else if (c == 3)
                        {
                            s.logout();
                        }
                    }
                }
            }
            if (!valid) cout << "Invalid login.\n";
        }
        else if (ch == 2) // buyer login
        {
            bool valid = false;
            for (auto &b : buyers)
            {
                if (b.login(u, p))
                {
                    valid = true;
                    cout << "Buyer logged in.\n";
                    int c;
                    while (b.isLoggedIn())
                    {
                        cout << "\n1.View Products\n2.Add to Cart\n3.View Cart\n4.Checkout\n5.Logout\nChoice: ";
                        cin >> c;
                        if (c == 1)
                        {
                            if (products.empty()) cout << "No products.\n";
                            else
                            {
                                cout << "\n=== Product List ===\n";
                                for (auto &p : products) p.showProduct();
                            }
                        }
                        else if (c == 2)
                        {
                            string pid;
                            int qty;
                            cout << "Enter product ID and qty: ";
                            cin >> pid >> qty;
                            try
                            {
                                b.addToCart(products, pid, qty);
                            }
                            catch (exception &e)
                            {
                                cout << e.what() << endl;
                            }
                        }
                        else if (c == 3)
                        {
                            b.viewCart();
                        }
                        else if (c == 4)
                        {
                            try
                            {
                                b.checkout();
                            }
                            catch (exception &e)
                            {
                                cout << e.what() << endl;
                            }
                        }
                        else if (c == 5)
                        {
                            b.logout();
                        }
                    }
                }
            }
            if (!valid) cout << "Invalid login.\n";
        }
    }
    return 0;
}
