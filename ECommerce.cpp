#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

// PRODUCT CLASS
class Product
{
    static int index;
    string productID;
    string name;
    double price;
    int stock;

public:
    Product(string n = "", double p = 0, int s = 0)
    {
        index += 1;
        productID = to_string(index) + n; // auto ID
        name = n;
        price = p;
        stock = s;
    }

    string getID() { return productID; }
    string getName() { return name; }
    double getPrice() { return price; }
    int getStock() { return stock; }

    void setStock(int s) { stock = s; }

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
    vector<string> productIDs;
    vector<string> names;
    vector<double> prices;
    vector<int> quantities;

public:
    void addToCart(vector<Product> &products, const string &pid, int qty)
    {
        if (pid.empty() || qty <= 0)
        {
            throw runtime_error("Invalid input.");
        }

        int idx = -1;
        for (size_t i = 0; i < products.size(); i++)
            if (products[i].getID() == pid)
            {
                idx = i;
                break;
            }

        if (idx == -1) throw runtime_error("Product not found.");
        if (products[idx].getStock() < qty) throw runtime_error("Insufficient stock.");

        products[idx].setStock(products[idx].getStock() - qty);

        productIDs.push_back(products[idx].getID());
        names.push_back(products[idx].getName());
        prices.push_back(products[idx].getPrice());
        quantities.push_back(qty);

        cout << names.back() << " x" << qty << " added to cart.\n";
    }

    void showCart()
    {
        if (productIDs.empty())
        {
            cout << "Cart is empty.\n";
            return;
        }
        cout << "\n=== Cart Items ===\n";
        for (size_t i = 0; i < productIDs.size(); i++)
        {
            cout << "ID: " << productIDs[i]
                 << " | " << names[i]
                 << " | Price: " << prices[i]
                 << " | Qty: " << quantities[i] << endl;
        }
    }

    double totalAmount()
    {
        double t = 0;
        for (size_t i = 0; i < prices.size(); i++)
            t += prices[i] * quantities[i];
        return t;
    }

    bool empty() const { return productIDs.empty(); }

    void clear()
    {
        productIDs.clear();
        names.clear();
        prices.clear();
        quantities.clear();
    }

    void printBill(const string &buyer)
    {
        if (productIDs.empty()) throw runtime_error("Cart empty.");
        cout << "\n----- BILL for " << buyer << " -----\n";
        double total = 0;
        for (size_t i = 0; i < productIDs.size(); i++)
        {
            double sub = prices[i] * quantities[i];
            cout << names[i] << " | " << quantities[i]
                 << " x " << prices[i] << " = " << sub << endl;
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

public:
    User(string u = "", string p = "") : username(u), password(p) {}

    string getUsername() { return username; }

    bool login(string u, string p)
    {
        return (username == u && password == p);
    }
};

// SELLER CLASS
class Seller : public User
{
public:
    Seller(string u = "", string p = "") : User(u, p) {}

    void addProduct(vector<Product> &products, const string &name, double price, int qty)
    {
        if (name.empty() || price < 0 || qty <= 0)
        {
            throw runtime_error("Invalid product details.");
        }
        products.push_back(Product(name, price, qty)); // auto ID
        cout << "Product added successfully.\n";
    }

    void viewProducts(vector<Product> &products)
    {
        if (products.empty())
        {
            cout << "No products available.\n";
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
        if (cart.empty()) throw runtime_error("Cart is empty.");
        cart.printBill(username);
        cart.clear();
    }
};

// MAIN
int main()
{
    vector<Product> products;
    vector<Seller> sellers;
    vector<Buyer> buyers;

    sellers.push_back(Seller("seller1", "pass"));
    buyers.push_back(Buyer("buyer1", "123"));

    while (true)
    {
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
            bool loggedIn = false;
            for (auto &s : sellers)
            {
                if (s.login(u, p))
                {
                    loggedIn = true;
                    cout << "Seller logged in.\n";
                    int c;
                    do
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
                    } while (c != 3);
                }
            }
            if (!loggedIn) cout << "Invalid login.\n";
        }
        else if (ch == 2) // buyer login
        {
            bool loggedIn = false;
            for (auto &b : buyers)
            {
                if (b.login(u, p))
                {
                    loggedIn = true;
                    cout << "Buyer logged in.\n";
                    int c;
                    do
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
                    } while (c != 5);
                }
            }
            if (!loggedIn) cout << "Invalid login.\n";
        }
    }
    return 0;
}
