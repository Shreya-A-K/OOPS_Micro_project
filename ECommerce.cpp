#include <iostream>
#include <string>
#include <vector>
using namespace std;

//Created a class User
class User{
    //declaring all variables
    private:
        string password;
    public:
        static int user_id;
        string username;
        string email;

        //custom default constructor function 
        User(string email, string password){
            user_id++;
            username=to_string(user_id)+ email;
            cout<<"Hello User!"<<endl;
            cout<<"Your username is :"<<username<<endl;
            cout<<"Remember your password for login purposes!!"<<endl;
        }

        //trial to see if changes are getting updated 
        cout<<hello;


};
class Product{
    public:
        int ProductId,stock;
        string name,category;
        double price;
        Product(int pi,int st,string n,string c, double p){
                ProductId = pi;
                name = n;
                stock = st;
                cateogory = c;
                price = p;
        }
        void getstring(){
                return string;
        }
        void displayProduct() const {
                cout<<"\nProduct Id : "<<ProductId<<endl;
                cout<<"\nCategory : "<<category<<endl;
                cout<<"\nName of the product : "<<name<<endl;
                cout<<"\nPrice : "<<price<<endl;
                cout<<"\nStock : "<<stock<<endl;
        }
        void updatePrice(double newPrice) {
                price = newPrice;
        }
        void updateStock(int newStock) {
                stock = newStock;
        }
        int getID() const { return productID; }
        string getName() const { return name; }
        double getPrice() const { return price; }
        int getStock() const { return stock; }
        string getCategory() const { return category; }
        
};
               
int User::user_id=0;


int main(){
    User user1("em","pw");
    return 0;

}

