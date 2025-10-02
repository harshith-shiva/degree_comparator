#include <iostream>
#include<unordered_map>
#include<vector>
#include<fstream>
#include<sstream>

using namespace std;

unordered_map<string,vector<string>> graph;

int find(string s){
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
    if(find(post[i])){
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
                if(find(courses[i])){
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

//file.close();



}


