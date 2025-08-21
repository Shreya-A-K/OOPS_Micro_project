#include <iostream>
#include <string>
#include <vector>
using namespace std;

//Created a class User
class User{
    //declaring all variables
    private:
        string password;
        static vector<User*> allUsers;
    public:
        static int user_id;
        
        string username;
        string email;
        bool loggedIn=false;

        //custom default constructor function 
        User(string email, string password){
            
                for (User * user : allUsers){
                        if ((*user).email==email){
                                throw string("Already registered!"); 

                        }
                }
                this->user_id=++user_id;
                allUsers.push_back(this);
                username=to_string(user_id)+ email;
                this->email=email;
                this->password = password;
                cout<<"Hello User!"<<endl;
                cout<<"Your username is :"<<username<<endl;
                cout<<"Remember your password for login purposes!!"<<endl;
        }

        void login(string email, string password){
            for (User * user: allUsers){
                if ((*user).email==email && (*user).password==password){
                        (*user).loggedIn=true;
                }
            }
        }

        void updateProfile(string username, string email){
                bool is_valid_username=false;
                for (User * user :allUsers){
                        if  ((*user).username==username ){
                                is_valid_username=true;
                                (*user).email=email;
                                cout<<"Email Update was successful";
                                (*user).username=to_string((*user).user_id)+email;
                                cout<<"Your new username is :"<<(*user).username;
                        }
                }
                if (is_valid_username==false){
                        throw string("Invalid Username");
                }
        }

        void logout(string un){
                bool valid_action=false;
                for (User * user:allUsers){
                        
                        if ((*user).username==un && (*user).loggedIn==true){
                                valid_action=true;
                                (*user).loggedIn=false;
                                cout<<"Logged out successfully!!";
                        }
                }
                if (valid_action==false){
                        throw string("Invalid action!!");
                }
        }
};

int User::user_id=0;
vector <User*> User::allUsers={};

class Product{
    public:
        int ProductId,stock;
        string name,category;
        double price;
        Product(int pi,int st,string n,string c, double p){
                ProductId = pi;
                name = n;
                stock = st;
                category = c;
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
        int getID() const { return ProductId; }
        string getName() const { return name; }
        double getPrice() const { return price; }
        int getStock() const { return stock; }
        string getCategory() const { return category; }
        
};
               



int main(){
    try{
        User user1("em","pw");
    }
    catch(string e){
                cerr<<"Error:"<<e<<endl;
        }
    return 0;

}

