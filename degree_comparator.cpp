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
unordered_map<string,vector<string>> graph1;


int find1(const unordered_map<string, vector<string>>& graph, const string& s) {
    if (graph.find(s) == graph.end()) {
        return 0;
    }
    return 1;
}

void printgraph(const unordered_map<string, vector<string>>& graph) {
    for (const auto& pair : graph) {
        cout << pair.first << " -> ";
        for (const auto& course : pair.second) {
            cout << course << " ";
        }
        cout << endl;
    }
}

vector<string> findpostrequisites(const unordered_map<string, vector<string>>& graph, const string& s) {
    ifstream file("degree1.csv");
    string line;
    vector<string> ans;

    while (getline(file, line)) {
        vector<string> courses;
        string course;
        stringstream ss(line);

        while (getline(ss, course, ',')) {
            courses.push_back(course);
        }

        for (size_t i = 1; i < courses.size(); i++) {
            if (s == courses[i]) {
                ans.push_back(courses[0]);
            }
        }
    }

    file.close();
    return ans;
}

void print_percentage_of_coredomains(const unordered_map<string, vector<string>>& graph) {
    unordered_map<string, vector<string>> domains;
    domains["core cs"] = {"datastructures", "datastructures lab", "design and analysis of algorithms", 
        "computer networks", "computer organization", "operating systems", 
        "unix architechture and programming", "digital electronics", "digital electronics lab", 
        "microprocessors and embedded systems", "embedded systems lab", "transform techniques", 
        "problem solving and c programming"};

    domains["Programming & Software Engineering"] = {"object oriented programming", "java programming", 
        "java programming lab", "python programming lab", "software patterns", "RDBMS lab", 
        "mobile computing"};

    domains["Databases & Information Systems"] = {"DBMS", "RDBMS lab"};
    domains["Mathematics & Theory"] = {"calculus", "linear algebra", "probability", "stochastics", 
        "Discrete structures"};
    domains["Artificial Intelligence"] = {"ML", "ML lab", "Big Data", "python"};
    domains["Security & Networks"] = {"security in computing", "computer networks"};
    domains["Interdisciplinary"] = {"mobile computing", "transform techniques"};

    int count_cs = 0, count_soft = 0, count_database = 0, count_maths = 0;
    int count_ai = 0, count_security = 0, count_inter = 0;

    for (const auto& pair1 : graph) {
        vector<string> keys;

        for (const auto& pair : domains) {
            const vector<string>& vec = pair.second;
            if (find(vec.begin(), vec.end(), pair1.first) != vec.end()) {
                keys.push_back(pair.first);
            }
        }

        for (const auto& key : keys) {
            if (key == "core cs") count_cs++;
            else if (key == "Programming & Software Engineering") count_soft++;
            else if (key == "Databases & Information Systems") count_database++;
            else if (key == "Mathematics & Theory") count_maths++;
            else if (key == "Artificial Intelligence") count_ai++;
            else if (key == "Security & Networks") count_security++;
            else if (key == "Interdisciplinary") count_inter++;
        }
    }

    vector<int> arr = {count_cs, count_soft, count_database, count_maths, count_ai, count_security, count_inter};
    vector<string> v = {"core cs", "Programming & Software Engineering", "Databases & Information Systems", 
                        "Mathematics & Theory", "Artificial Intelligence", "Security & Networks", "Interdisciplinary"};

    cout << left << setw(40) << "Domain" 
         << setw(15) << "Courses" 
         << setw(5) << "%" << endl;
    cout << string(60, '-') << endl;

    int total = accumulate(arr.begin(), arr.end(), 0);
    for (size_t j = 0; j < v.size(); j++) {
        cout << left << setw(40) << v[j]
             << setw(15) << arr[j]
             << setw(5) << (total > 0 ? (arr[j] * 100) / total : 0)
             << endl;
    }
}


void buildGraphFromUser(unordered_map<string, vector<string>>& graph) {
    int c = 1;
    while (c) {
        ifstream file("degree1.csv");
        string input;
        cout << "Enter the course: ";
        getline(cin, input);

        // Add postrequisites using your existing logic
        vector<string> post = findpostrequisites(graph, input);
        for (size_t i = 0; i < post.size(); ++i) {
            if (find1(graph, post[i])) {
                graph[input].push_back(post[i]);
            }
        }

        // Read CSV to connect prerequisites
        string line;
        while (getline(file, line)) {
            vector<string> courses;
            string course;
            stringstream ss(line);
            while (getline(ss, course, ',')) {
                courses.push_back(course);
            }

            if (!courses.empty() && courses[0] == input) {
                graph.insert({courses[0], {}});
                for (size_t i = 1; i < courses.size(); ++i) {
                    if (!courses[i].empty() && find1(graph, courses[i])) {
                        graph[courses[i]].push_back(courses[0]);
                    }
                }
            }
        }

        cout << "Press 1 to enter another course, 0 to stop: ";
        cin >> c;
        cin.ignore();
        file.close();
    }
}


void printGraphIntersection(
    const unordered_map<string, vector<string>>& graph1,
    const unordered_map<string, vector<string>>& graph2
) {
    cout << "Courses common to both graphs:\n";
    bool found = false;

    for (const auto& pair : graph1) {
        const string& course = pair.first;
        if (graph2.find(course) != graph2.end()) {
            cout << course << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No common courses found.\n";
    }
}










int main() {
    unordered_map<string, vector<string>> graph1;
     unordered_map<string, vector<string>> graph2;
    
    
    buildGraphFromUser(graph1);
    buildGraphFromUser(graph2);   // now graph is populated
    printgraph(graph1); 
    cout<<endl;
    printgraph(graph2);     
    cout<<endl;      // call your refactored print function
    print_percentage_of_coredomains(graph1);
    cout<<endl;
    print_percentage_of_coredomains(graph2); // call your domain stats function
    cout<<endl;
    printGraphIntersection(graph1,graph2);
    
    return 0;
}
