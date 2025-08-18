#include <iostream>
#include <string>
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


};
int User::user_id=0;


int main(){
    User user1("em","pw");
    return 0;

}
