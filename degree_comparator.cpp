#include <iostream>
#include<unordered_map>
#include <unordered_set>
#include<vector>
#include<fstream>
#include<sstream>
#include <algorithm>
#include <numeric> 
#include <iomanip>
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <bits/stdc++.h>

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

void drawArrow(sf::RenderWindow &window, sf::Vector2f start, sf::Vector2f end) {
    sf::Vertex line[] = {
        sf::Vertex(start, sf::Color::Black),
        sf::Vertex(end, sf::Color::Black)
    };
    window.draw(line, 2, sf::Lines);

    // Create a small triangle for the arrowhead
    sf::Vector2f direction = end - start;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= length;

    sf::Vector2f perpendicular(-direction.y, direction.x);
    float arrowSize = 10.f;

    sf::ConvexShape arrowHead;
    arrowHead.setPointCount(3);
    arrowHead.setPoint(0, end);
    arrowHead.setPoint(1, end - direction * arrowSize + perpendicular * (arrowSize / 2));
    arrowHead.setPoint(2, end - direction * arrowSize - perpendicular * (arrowSize / 2));
    arrowHead.setFillColor(sf::Color::Black);

    window.draw(arrowHead);
}


void drawGraph(const unordered_map<string, vector<string>>& graph) {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Graph Visualization");

    // Load a font
    sf::Font font;
    if (!font.loadFromFile("NotoSerif-Regular.ttf")) {
    cerr << "Error: Could not load font!\n";
    system("pause");
    return;
}


    // Arrange nodes in a circle layout
    int n = graph.size();
    float centerX = 500, centerY = 400, radius = 250;
    vector<sf::Vector2f> positions;
    vector<string> nodes;

    for (auto &p : graph)
        nodes.push_back(p.first);

    for (int i = 0; i < n; ++i) {
        float angle = i * (2 * M_PI / n);
        positions.push_back({centerX + radius * cos(angle), centerY + radius * sin(angle)});
    }

    // Main draw loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Draw edges
        for (int i = 0; i < n; ++i) {
            const auto &from = nodes[i];
            for (auto &to : graph.at(from)) {
                auto it = find(nodes.begin(), nodes.end(), to);
                if (it != nodes.end()) {
                    int j = distance(nodes.begin(), it);
                    drawArrow(window, positions[i], positions[j]);
                }
            }
        }

        // Draw nodes
        for (int i = 0; i < n; ++i) {
            sf::CircleShape node(30);
            node.setFillColor(sf::Color(150, 200, 255));
            node.setOutlineThickness(2);
            node.setOutlineColor(sf::Color::Black);
            node.setPosition(positions[i].x - 30, positions[i].y - 30);

            sf::Text text;
            text.setFont(font);
            text.setString(nodes[i]);
            text.setCharacterSize(14);
            text.setFillColor(sf::Color::Black);

            // Center text inside circle
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.width / 2, textRect.height / 2);
            text.setPosition(positions[i]);

            window.draw(node);
            window.draw(text);
        }

        window.display();
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

        
        vector<string> post = findpostrequisites(graph, input);
        for (size_t i = 0; i < post.size(); ++i) {
            if (find1(graph, post[i])) {
                graph[input].push_back(post[i]);
            }
        }

     
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

struct ChainResult 
{
    int length;                
    vector<string> path;      
};

ChainResult longestChainTopo(const unordered_map<string, vector<string>>& graph)
{
    unordered_map<string, int> indeg;
    unordered_set<string> nodes;

    // Step 1: Build indegree table and collect all nodes
    for (auto& [u, nbrs] : graph) {
        nodes.insert(u);
        for (auto& v : nbrs) {
            indeg[v]++;
            nodes.insert(v);
        }
    }
    for (auto& n : nodes)
        if (!indeg.count(n)) indeg[n] = 0;

    // Step 2: Kahn’s Topological Sort
    queue<string> q;
    for (auto& [n, d] : indeg)
        if (d == 0) q.push(n);

    vector<string> topo;
    while (!q.empty()) {
        string u = q.front(); q.pop();
        topo.push_back(u);
        if (graph.count(u)) {
            for (auto& v : graph.at(u)) {
                indeg[v]--;
                if (indeg[v] == 0) q.push(v);
            }
        }
    }

    if (topo.size() != nodes.size())
        throw runtime_error("Cycle detected in prerequisites!");

    // Step 3: DP arrays
    unordered_map<string, int> dp;
    unordered_map<string, string> nextNode;

    for (auto& n : nodes) {
        dp[n] = 1;          // each node itself
        nextNode[n] = "";   // no next
    }

    // Step 4: Process in reverse topological order
    for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
        string u = *it;
        if (graph.count(u)) {
            for (auto& v : graph.at(u)) {
                if (1 + dp[v] > dp[u]) {
                    dp[u] = 1 + dp[v];
                    nextNode[u] = v;
                }
            }
        }
    }

    // Step 5: Find starting node of longest chain
    string start;
    int maxLen = 0;
    for (auto& [n, val] : dp) {
        if (val > maxLen) {
            maxLen = val;
            start = n;
        }
    }

    // Step 6: Reconstruct path
    vector<string> path;
    string cur = start;
    while (!cur.empty()) {
        path.push_back(cur);
        cur = nextNode[cur];
    }

    // Step 7: Print results here (inside function)
    cout << "\n--- Longest Prerequisite Chain Details ---\n";
    cout << "Length: " << maxLen << "\n";
    cout << "Chain: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\n------------------------------------------\n";

    // Step 8: Return for completeness (optional)
    ChainResult result;
    result.length = maxLen;
    result.path = path;
    return result;
}




void printIndependentCourses(const unordered_map<string, vector<string>>& graph) {
    
    unordered_set<string> dependentCourses;
    for (const auto& pair : graph) {
        for (const auto& neighbor : pair.second) {
            dependentCourses.insert(neighbor);
        }
    }

   
    cout << "Independent courses (no prerequisites):\n";
    bool found = false;
    for (const auto& pair : graph) {
        if (dependentCourses.find(pair.first) == dependentCourses.end()) {
            cout << pair.first << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No independent courses found.\n";
    }
}





int main() {
    unordered_map<string, vector<string>> graph1;
     unordered_map<string, vector<string>> graph2;
    
    
    buildGraphFromUser(graph1);
    buildGraphFromUser(graph2); 
     
    drawGraph(graph1); 
    cout<<endl;
    //printgraph(graph2); 
    drawGraph(graph2);     
    cout<<endl;      
    print_percentage_of_coredomains(graph1);
    cout<<endl;
    system("pause");
    print_percentage_of_coredomains(graph2); 
    cout<<endl;
    system("pause");
    printGraphIntersection(graph1,graph2);
    system("pause");
    longestChainTopo(graph1);
    system("pause");
    return 0;
}
