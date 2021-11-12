#include <map>
#include "TwitterData.hpp"
using namespace std;

int main(int argc, char *argv[])
{
    // Schema: UserName,Last,First,Email,NumTweets,MostViewedCategory
    string raw_data[5][6] = {{"rangerPower", "Smit", "Rick", "smitRick@gmail.com", "1117", "power lifting"},
                             {"kittyKat72", "Smith", "Kathryn", "kat@gmail.com", "56", "health"},
                             {"lexi5", "Anderson", "Alexis", "lexi5@gmail.com", "900", "education"},
                             {"savage1", "Savage", "Ken", "ksavage@gmail.com", "17", "president"},
                             {"smithMan", "Smith", "Rick", "rick@hotmail.com", "77", "olympics"}};
    TwitterData *twitter_data = new TwitterData[5];
    for (int i = 0; i < 5; ++i)
    {
        twitter_data[i].setUserName(raw_data[i][0]);
        twitter_data[i].setActualName(raw_data[i][2] + " " + raw_data[i][1]);
        twitter_data[i].setEmail(raw_data[i][3]);
        twitter_data[i].setNumTweets(stoi(raw_data[i][4]));
        twitter_data[i].setCategory(raw_data[i][5]);
    }



    //
    // Your code goes here
    //

    map<string, TwitterData> nameMap; //creating user map 

    for(int i = 0; i < 5; i++){  // filling the user name map with the twitter data 
        nameMap[twitter_data[i].getUserName()] = twitter_data[i];
    }


    //key value pairs line by line 
    cout << "\nPrint searchByName map:" << endl;

    for(auto pair:nameMap){  // easier to declare as auto with the template
        cout << endl;
        cout <<"Key: " + pair.first << endl   //  print the key email
        << "Values: " + pair.second.print() << endl << endl; //print the record of the user
    }


    //finding a person with the username inputted (E.X "savage1")

    string name;
    cout << "Please enter the username of the record you are trying to find: ";
    cin >> name;
    auto it = nameMap.find(name);

    if(it != nameMap.end()){
        cout << "The Username: " << "'" << it->first << "'" << " was found in the map\n" << endl
        << "Record: " << it->second.print() << endl;
    }
    else{
        cout << "The Username: " << "'" << name << "'" <<  " was not found\n" << endl;
        
    }

    // deleting a select person from the map 

    string nameDel;
    cout << "\nPlease enter the username of the record you are trying to delete: ";
    cin >> nameDel;
    auto itDel = nameMap.find(nameDel);

    if(itDel != nameMap.end()){
    }
    else{
        cout << "The Username: " << "'" << name << "'" <<  " was not found\n" << endl;
    }

    map<string, TwitterData>::iterator i = nameMap.begin();

    while(i !=nameMap.end()){
        if(nameDel == i->first){
            i = nameMap.erase(i++); //need to erase the iterator after the function is complete 
        }else{
            ++i;   
        }
    }

// block below is for validating the delete function
    // for(auto pair:nameMap){
    //     cout << endl;
    //     cout <<" Username: " + pair.first << endl
    //     << pair.second.print() << endl << endl;
    // }










    //Part 2: Search by email 

    map<string, TwitterData> emailMap; //creating user email map 

    for(int i = 0; i < 5; i++){  // filling the user email map with the twitter data 
        emailMap[twitter_data[i].getEmail()] = twitter_data[i];
    }

    cout << "\nPrint searchByEmail map:" << endl;

    for(auto pair:emailMap){
        cout << endl;
        cout <<"Key: " + pair.first << endl
        << "Values: " + pair.second.print() << endl << endl;
    }

        //finding a person with the username inputted (E.X "savage1")

    string email;
    cout << "Please enter the email of the record you are trying to find: ";
    cin >> email;
    auto it2 = emailMap.find(email);


    if(it2 != emailMap.end()){
        cout << "The Email: " << "'" << it2->first << "'" << " was found in the map\n" << endl
        << "Record: " << it2->second.print() << endl;
    }
    else{
        cout << "The Email: " << "'" << name << "'" <<  " was not found\n" << endl;
        
    }

    // deleting a select person from the map 

    string emailDel;
    cout << "\nPlease enter the email of the record you are trying to delete: ";
    cin >> emailDel;

    auto itDelE = nameMap.find(emailDel);

    if(itDelE != emailMap.end()){
    }
    else{
        cout << "The email: " << "'" << email << "'" <<  " was not found\n" << endl;
    }

    map<string, TwitterData>::iterator i2 = emailMap.begin();  //change iterator as it would become redundant 

    while(i2 !=emailMap.end()){
        if(emailDel == i2->first){ // same code as the name search and delete
            i2 = emailMap.erase(i2++); 
        }else{
            ++i2;
        }
    }

    //for validating the email delete funtion 
    
    // for(auto pair:emailMap){
    //     cout << endl;
    //     cout <<"Key: " + pair.first << endl
    //     << "Values: " + pair.second.print() << endl << endl;
    // }

    return 0;
}