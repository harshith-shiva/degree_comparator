#include <iostream>
#include<unordered_map>
#include<vector>
#include<fstream>
#include<sstream>
#include <algorithm>
#include <numeric> 
#include <iomanip>
using namespace std;

unordered_map<string,vector<string>> graph;

int find1(string s){
    if(graph.find(s)==graph.end()){
        return 0;
    }

    return 1;
}

void printgraph(){
    int i;
    for(auto& pair: graph){
        cout<<pair.first<<"->";
        for (const auto &course : pair.second) {
            cout << course << " ";
        }
        cout << endl;
    }

    }

vector<string> findpostrequisites(string s){
    ifstream file("degree1.csv");
    string line;
    int i;
    vector<string> ans;
    while(getline(file,line)){
        vector<string> courses;
        string course;
        stringstream ss(line);
        while(getline(ss,course,',')){
        courses.push_back(course);
        }

        for(i=1;i<courses.size();i++){
            if(s==courses[i]){
                ans.push_back(courses[0]);
            }
        }


    }
    file.close();
    return ans;

}

void print_percentage_of_coredomains(){
    unordered_map<string,vector<string>> domains;
    domains["core cs"]={"datastructures", "datastructures lab", "design and analysis of algorithms", "computer networks", "computer organization", "operating systems", "unix architechture and programming", "digital electronics", "digital electronics lab", "microprocessors and embedded systems", "embedded systems lab", "transform techniques", "problem solving and c programming"};
    domains["Programming & Software Engineering"]={"object oriented programming", "java programming", "java programming lab", "python programming lab", "software patterns", "RDBMS lab", "mobile computing"};
    domains["Databases & Information Systems"]={"DBMS", "RDBMS lab"};
    domains["Mathematics & Theory"]={"calculus", "linear algebra", "probability", "stochastics", "Discrete structures"};
    domains["Artificial Intelligence"]={"ML", "ML lab", "Big Data", "python"};
    domains["Security & Networks"]={"security in computing", "computer networks"};
    domains["Interdisciplinary"]={"mobile computing", "transform techniques"};

int count_cs=0;int count_soft=0;int count_database=0; int count_maths=0; int count_ai=0; int count_security=0; int count_inter=0;




 for(auto& pair1: graph){
        vector<string> keys;
       
        for (const auto& pair : domains) {
        const vector<string>& vec = pair.second; // reference to vector
        if (find(vec.begin(), vec.end(), pair1.first) != vec.end()) {
            keys.push_back(pair.first);
        }
    }

        int i;
        for(i=0;i<keys.size();i++){
            if(keys[i]=="core cs"){
                 count_cs++;
            }
            else if(keys[i]=="Programming & Software Engineering"){
                count_soft++;
            }

           else if(keys[i]=="Databases & Information Systems"){
                count_database++;
            }

            else if(keys[i]=="Mathematics & Theory"){
                count_maths++;
            }

            else if(keys[i]=="Artificial Intelligence"){
                count_ai++;
            }

            else if(keys[i]=="Security & Networks"){
                count_security++;
            }

            else if(keys[i]=="Interdisciplinary"){
                count_inter++;

            }



        }
 }
 vector<int> arr={count_cs, count_soft, count_database,count_maths, count_ai, count_security, count_inter};
 vector<string> v={"core cs","Programming & Software Engineering","Databases & Information Systems","Mathematics & Theory","Artificial Intelligence","Security & Networks","Interdisciplinary"};
 //cout<< "Domain                               Courses                              %"<<endl;

cout << left << setw(40) << "Domain" 
         << setw(15) << "Courses" 
         << setw(5) << "%" << endl;
    cout << string(60, '-') << endl;

 int j=0;
 int total = std::accumulate(arr.begin(), arr.end(), 0);
 for( j=0; j<v.size(); j++){
        cout << left << setw(40) << v[j]
             << setw(15) << arr[j]
             << setw(5) << (arr[j]*100)/total
             << endl;
    }


}




int main(){
string*ptr=nullptr;
int c=1;
//ifstream file("degree1.csv");

while(c){
ifstream file("degree1.csv");
string input;
cout<<"enter the course ";
getline(cin,input);
string line;
int i;
vector<string> post;
post= findpostrequisites(input);
   for(i=0;i<post.size();i++){
    if(find1(post[i])){
        graph[input].push_back(post[i]);
    }
   }

while (getline(file,line)){
    vector<string> courses;
    
    string course;
    stringstream ss(line);
    while(getline(ss,course,',')){
        courses.push_back(course);
    
    }
    if(courses[0]==input){
        graph.insert({courses[0],{}});
        for(i=1;i<courses.size();i++){
            if(courses[i]!=""){
                if(find1(courses[i])){
                    graph[courses[i]].push_back(courses[0]);
                }

            }

        }
    }
   


    

}
cout<<"press 1 to enter again";
cin>>c;
cin.ignore(); 
file.close();
}
printgraph();
cout<<endl;

print_percentage_of_coredomains();


//file.close();



}


