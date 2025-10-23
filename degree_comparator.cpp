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

unordered_map<string,vector<string>> graph1;
unordered_map<string,vector<string>> graph2;


int find1(const unordered_map<string, vector<string>>& graph, const string& s) {
    if (graph.find(s) == graph.end()) {
        return 0;
    }
    return 1;
}

void drawGraph(const unordered_map<string, vector<string>>& graph) {
    int n = graph.size();
    if (n == 0) {
        cerr << "Graph is empty!\n";
        return;
    }

    // --- Dynamically scale window based on number of nodes ---
    int maxNodesPerLayer = 15;
    int layers = ceil((float)n / maxNodesPerLayer);
    int windowWidth = max(1200, 400 + layers * 300);
    int windowHeight = max(900, 400 + layers * 300);

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "DAG Visualization");

    sf::Font font;
    if (!font.loadFromFile("NotoSerif-Regular.ttf")) {
        cerr << "Error: Could not load font!\n";
        return;
    }

    float centerX = windowWidth / 2.f;
    float centerY = windowHeight / 2.f;
    float baseRadius = 200.0f;
    float nodeRadius = 28.0f;
    float layerSpacing = 180.0f;

    vector<sf::Vector2f> positions;
    vector<string> nodes;

    for (auto &p : graph)
        nodes.push_back(p.first);

    int nodeIndex = 0;
    for (int layer = 0; layer < layers; ++layer) {
        int nodesInLayer = min(maxNodesPerLayer, n - nodeIndex);
        float angleStep = 2 * M_PI / nodesInLayer;
        float radius = baseRadius + layer * layerSpacing;

        for (int i = 0; i < nodesInLayer; ++i) {
            float angle = i * angleStep;
            float x = centerX + radius * cos(angle);
            float y = centerY + radius * sin(angle);
            positions.push_back({x, y});
            nodeIndex++;
        }
    }

    // --- Render loop ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Draw directed edges with arrowheads
        for (size_t i = 0; i < nodes.size(); ++i) {
            const auto &from = nodes[i];
            if (!graph.count(from)) continue;

            for (const auto &to : graph.at(from)) {
                auto it = find(nodes.begin(), nodes.end(), to);
                if (it != nodes.end()) {
                    int j = distance(nodes.begin(), it);

                    sf::Vector2f start = positions[i];
                    sf::Vector2f end = positions[j];

                    // Compute direction vector
                    sf::Vector2f dir = end - start;
                    float len = sqrt(dir.x * dir.x + dir.y * dir.y);
                    dir /= len;

                    // Move line endpoints so it touches the node boundary instead of center
                    sf::Vector2f lineStart = start + dir * nodeRadius;
                    sf::Vector2f lineEnd = end - dir * nodeRadius;

                    // Draw main line
                    sf::Vertex line[] = {
                        sf::Vertex(lineStart, sf::Color(60, 60, 60)),
                        sf::Vertex(lineEnd, sf::Color(60, 60, 60))
                    };
                    window.draw(line, 2, sf::Lines);

                    // Draw arrowhead slightly before node
                    sf::Vector2f perp(-dir.y, dir.x);
                    float arrowSize = 12.f;
                    sf::ConvexShape arrowHead;
                    arrowHead.setPointCount(3);
                    arrowHead.setPoint(0, lineEnd);
                    arrowHead.setPoint(1, lineEnd - dir * arrowSize + perp * (arrowSize / 2));
                    arrowHead.setPoint(2, lineEnd - dir * arrowSize - perp * (arrowSize / 2));
                    arrowHead.setFillColor(sf::Color::Black);
                    window.draw(arrowHead);
                }
            }
        }

        // Draw nodes
        for (size_t i = 0; i < nodes.size(); ++i) {
            sf::CircleShape node(nodeRadius);
            node.setFillColor(sf::Color(180, 220, 255));
            node.setOutlineThickness(2);
            node.setOutlineColor(sf::Color::Black);
            node.setPosition(positions[i].x - nodeRadius, positions[i].y - nodeRadius);
            window.draw(node);

            // Node label
            sf::Text text;
            text.setFont(font);
            text.setString(nodes[i]);
            text.setCharacterSize(n > 40 ? 12 : 14);
            text.setFillColor(sf::Color::Black);
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.width / 2, textRect.height / 2);
            text.setPosition(positions[i]);
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


void buildGraphFromCSV(unordered_map<string, vector<string>>& graph, const string& degreeName) {
    string filename;

    // Convert to lowercase for flexible matching
    string degreeLower = degreeName;
    transform(degreeLower.begin(), degreeLower.end(), degreeLower.begin(), ::tolower);

    // Select CSV file based on degree name
    if (degreeLower == "msc softwaresystems" || degreeLower == "msc software systems")
        filename = "degree1.csv";
    else if (degreeLower == "anna university be cse" || degreeLower == "be cse")
        filename = "degree2.csv";
    else {
        cerr << "❌ Unknown degree name: " << degreeName << "\n";
        cerr << "Available options: 'msc softwaresystems' or 'anna university be cse'\n";
        return;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "❌ Error: Could not open file: " << filename << "\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        vector<string> courses;
        string course;
        stringstream ss(line);

        while (getline(ss, course, ',')) {
            // convert to lowercase and trim spaces
            transform(course.begin(), course.end(), course.begin(), ::tolower);
            course.erase(remove_if(course.begin(), course.end(), ::isspace), course.end());
            courses.push_back(course);
        }

        if (courses.empty()) continue;

        string mainCourse = courses[0];
        graph[mainCourse];  // ensure the main course is present in the graph

        // Add edges: prereq -> mainCourse
        for (size_t i = 1; i < courses.size(); ++i) {
            if (!courses[i].empty()) {
                graph[courses[i]].push_back(mainCourse);
            }
        }
    }

    file.close();
    cout << "✅ Successfully built graph for degree: " << degreeName << "\n";
    cout << "📘 Loaded from file: " << filename << "\n";
    cout << "Total courses loaded: " << graph.size() << "\n";
}


void visualizeCourseLevels(const unordered_map<string, vector<string>>& graph) {
    // Build indegree map and collect all nodes
    unordered_map<string, int> indeg;
    unordered_set<string> nodes;
    for (const auto& [u, nbrs] : graph) {
        nodes.insert(u);
        if (!indeg.count(u)) indeg[u] = 0;
        for (const auto& v : nbrs) {
            indeg[v]++;
            nodes.insert(v);
        }
    }
    for (const auto& n : nodes)
        if (!indeg.count(n)) indeg[n] = 0;

    // BFS to assign levels
    unordered_map<string, int> level;
    queue<string> q;
    for (const auto& [n, d] : indeg)
        if (d == 0) {
            q.push(n);
            level[n] = 0;
        }

    while (!q.empty()) {
        string u = q.front(); q.pop();
        int curL = level[u];
        if (graph.count(u)) {
            for (const auto& v : graph.at(u)) {
                indeg[v]--;
                if (indeg[v] == 0) {
                    level[v] = curL + 1;
                    q.push(v);
                }
            }
        }
    }

    // Assign default levels for unconnected or cyclic nodes
    int maxLevel = 0;
    for (const auto& [_, l] : level)
        maxLevel = max(maxLevel, l);
    for (const auto& n : nodes)
        if (!level.count(n))
            level[n] = maxLevel + 1;
    for (const auto& [_, l] : level)
        maxLevel = max(maxLevel, l);

    // Group nodes by level
    unordered_map<int, vector<string>> levels;
    int maxNodesInALevel = 0;
    for (const auto& [node, l] : level) {
        levels[l].push_back(node);
        maxNodesInALevel = max<int>(maxNodesInALevel, levels[l].size());
    }

    // --- Dynamic window sizing ---
    int nodeDiameter = 60;
    int courseGapX = max(160, nodeDiameter + 40);
    int levelGapY = max(140, nodeDiameter + 40);

    int marginX = 160;
    int marginY = 160;

    int windowWidth = max(1200, marginX * 2 + maxNodesInALevel * courseGapX);
    int windowHeight = max(800, marginY * 2 + (maxLevel + 1) * levelGapY);

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight),
                            "Course Clustering by Prerequisite Depth");

    sf::Font font;
    if (!font.loadFromFile("NotoSerif-Regular.ttf")) {
        cerr << "Error: Could not load font!\n";
        return;
    }

    // --- Compute node positions ---
    unordered_map<string, sf::Vector2f> pos;
    for (int lvl = 0; lvl <= maxLevel; ++lvl) {
        auto it = levels.find(lvl);
        if (it == levels.end()) continue;

        vector<string>& vec = it->second;
        int count = vec.size();
        if (count == 0) continue;

        float totalWidth = (count - 1) * courseGapX;
        float startX = (windowWidth - totalWidth) / 2.0f;
        float y = marginY + lvl * levelGapY;

        for (int i = 0; i < count; ++i) {
            float x = startX + i * courseGapX;
            pos[vec[i]] = { x, y };
        }
    }

    // --- Render loop ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear(sf::Color::White);

        // Draw directed edges with visible arrowheads
        for (const auto& [u, nbrs] : graph) {
            for (const auto& v : nbrs) {
                if (pos.count(u) && pos.count(v)) {
                    sf::Vector2f start = pos[u];
                    sf::Vector2f end = pos[v];

                    sf::Vector2f dir = end - start;
                    float len = sqrt(dir.x * dir.x + dir.y * dir.y);
                    if (len < 1e-3f) continue;
                    dir /= len;

                    // Move endpoints to edge of nodes
                    sf::Vector2f lineStart = start + dir * (nodeDiameter / 2.0f);
                    sf::Vector2f lineEnd = end - dir * (nodeDiameter / 2.0f);

                    // Draw line
                    sf::Vertex line[] = {
                        sf::Vertex(lineStart, sf::Color(50, 50, 50)),
                        sf::Vertex(lineEnd, sf::Color(50, 50, 50))
                    };
                    window.draw(line, 2, sf::Lines);

                    // Arrowhead before circle
                    sf::Vector2f perp(-dir.y, dir.x);
                    float arrowSize = 12.f;
                    sf::ConvexShape arrowHead;
                    arrowHead.setPointCount(3);
                    arrowHead.setPoint(0, lineEnd);
                    arrowHead.setPoint(1, lineEnd - dir * arrowSize + perp * (arrowSize / 2));
                    arrowHead.setPoint(2, lineEnd - dir * arrowSize - perp * (arrowSize / 2));
                    arrowHead.setFillColor(sf::Color::Black);
                    window.draw(arrowHead);
                }
            }
        }

        // Draw nodes
        for (const auto& [lvl, vec] : levels) {
            for (const auto& node : vec) {
                if (!pos.count(node)) continue;
                sf::Vector2f p = pos[node];

                sf::CircleShape circle(nodeDiameter / 2.0f);
                circle.setFillColor(sf::Color(180, 220, 255));
                circle.setOutlineThickness(2);
                circle.setOutlineColor(sf::Color::Black);
                circle.setPosition(p.x - nodeDiameter / 2.0f, p.y - nodeDiameter / 2.0f);
                window.draw(circle);

                sf::Text label;
                label.setFont(font);
                label.setString(node);
                label.setCharacterSize(vec.size() > 6 ? 14 : 16);
                label.setFillColor(sf::Color::Black);
                sf::FloatRect textRect = label.getLocalBounds();
                label.setOrigin(textRect.width / 2, textRect.height / 2);
                label.setPosition(p.x, p.y - 4);
                window.draw(label);
            }
        }

        // Draw "Level n" headers
        for (int lvl = 0; lvl <= maxLevel; ++lvl) {
            sf::Text header;
            header.setFont(font);
            header.setCharacterSize(18);
            header.setFillColor(sf::Color::Red);
            header.setString("Level " + to_string(lvl));
            header.setPosition(20, marginY + lvl * levelGapY - 12);
            window.draw(header);
        }

        window.display();
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

    
    for (auto& [u, nbrs] : graph) {
        nodes.insert(u);
        for (auto& v : nbrs) {
            indeg[v]++;
            nodes.insert(v);
        }
    }
    for (auto& n : nodes)
        if (!indeg.count(n)) indeg[n] = 0;

    
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

    
    unordered_map<string, int> dp;
    unordered_map<string, string> nextNode;

    for (auto& n : nodes) {
        dp[n] = 1;          
        nextNode[n] = "";   
    }

    
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

    
    string start;
    int maxLen = 0;
    for (auto& [n, val] : dp) {
        if (val > maxLen) {
            maxLen = val;
            start = n;
        }
    }

    vector<string> path;
    string cur = start;
    while (!cur.empty()) {
        path.push_back(cur);
        cur = nextNode[cur];
    }

    
    cout << "\n--- Longest Prerequisite Chain Details ---\n";
    cout << "Length: " << maxLen << "\n";
    cout << "Chain: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\n------------------------------------------\n";

    
    ChainResult result;
    result.length = maxLen;
    result.path = path;
    return result;
}




double computeJaccardSimilarity(const unordered_map<string, vector<string>>& graph1,
                                const unordered_map<string, vector<string>>& graph2) {
    unordered_set<string> setA, setB;

    for (const auto& p : graph1)
        setA.insert(p.first);
    for (const auto& p : graph2)
        setB.insert(p.first);


    int intersectionCount = 0;
    for (const auto& course : setA)
        if (setB.count(course))
            intersectionCount++;


    int unionCount = setA.size() + setB.size() - intersectionCount;


    if (unionCount == 0) return 1.0;

    return static_cast<double>(intersectionCount) / unionCount;
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

    string degreeName1 = "MSc Software Systems";
    string degreeName2 = "anna university be cse";

    buildGraphFromCSV(graph1, degreeName1);
    buildGraphFromCSV(graph2, degreeName2);

    sf::RenderWindow menuWindow(sf::VideoMode(1200, 800), "Course Graph Analyzer");
    sf::Font font;
    if (!font.loadFromFile("NotoSerif-Regular.ttf")) {
        cerr << "Error: Could not load font!\n";
        return 1;
    }

    sf::Text title("Course Prerequisite Analyzer", font, 30);
    title.setFillColor(sf::Color::Black);
    title.setPosition((1200.f - title.getLocalBounds().width) / 2.f, 20.f);

    struct Button {
        sf::RectangleShape shape;
        sf::Text label;
        std::function<void()> onClick;
    };

    vector<Button> buttons;

    auto addButton = [&](float x, float y, const string& txt, auto&& func) {
        Button b;
        b.shape.setPosition(x, y);
        b.shape.setSize({340.f, 50.f});
        b.shape.setFillColor(sf::Color(100, 181, 246));
        b.label.setFont(font);
        b.label.setString(txt);
        b.label.setCharacterSize(20);
        b.label.setFillColor(sf::Color::White);
        sf::FloatRect bounds = b.label.getLocalBounds();
        b.label.setPosition(x + (340.f - bounds.width) / 2.f, y + (50.f - bounds.height) / 2.f - 5.f);
        b.onClick = std::forward<decltype(func)>(func);
        buttons.push_back(b);
    };

    auto displayText = [&](const string& windowTitle, const string& content) {
        sf::RenderWindow textWindow(sf::VideoMode(800, 600), windowTitle);
        if (!font.loadFromFile("NotoSerif-Regular.ttf")) {
            cerr << "Error: Could not load font!\n";
            return;
        }
        vector<sf::Text> lines;
        stringstream ss(content);
        string line;
        float y_pos = 20.f;
        while (getline(ss, line)) {
            sf::Text text(line, font, 14);
            text.setFillColor(sf::Color::Black);
            text.setPosition(20.f, y_pos);
            lines.push_back(text);
            y_pos += 20.f;
        }
        while (textWindow.isOpen()) {
            sf::Event event;
            while (textWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    textWindow.close();
            }
            textWindow.clear(sf::Color::White);
            for (auto& t : lines)
                textWindow.draw(t);
            textWindow.display();
        }
    };

    // Add buttons in a grid layout
    addButton(50.f, 100.f, "Visualize MSc Graph", [&]{ drawGraph(graph1); });
    addButton(420.f, 100.f, "Visualize BE CSE Graph", [&]{ drawGraph(graph2); });
    addButton(790.f, 100.f, "Visualize MSc Levels", [&]{ visualizeCourseLevels(graph1); });

    addButton(50.f, 170.f, "Visualize BE CSE Levels", [&]{ visualizeCourseLevels(graph2); });
    addButton(420.f, 170.f, "MSc Domain Percentages", [&]{
        stringstream ss;
        auto old = cout.rdbuf(ss.rdbuf());
        print_percentage_of_coredomains(graph1);
        cout.rdbuf(old);
        displayText("MSc Domain Percentages", ss.str());
    });
    addButton(790.f, 170.f, "BE CSE Domain Percentages", [&]{
        stringstream ss;
        auto old = cout.rdbuf(ss.rdbuf());
        print_percentage_of_coredomains(graph2);
        cout.rdbuf(old);
        displayText("BE CSE Domain Percentages", ss.str());
    });

    addButton(50.f, 240.f, "MSc Longest Chain", [&]{
        stringstream ss;
        auto old = cout.rdbuf(ss.rdbuf());
        longestChainTopo(graph1);
        cout.rdbuf(old);
        displayText("MSc Longest Prerequisite Chain", ss.str());
    });
    addButton(420.f, 240.f, "BE CSE Longest Chain", [&]{
        stringstream ss;
        auto old = cout.rdbuf(ss.rdbuf());
        longestChainTopo(graph2);
        cout.rdbuf(old);
        displayText("BE CSE Longest Prerequisite Chain", ss.str());
    });
    addButton(790.f, 240.f, "Common Courses", [&]{
        stringstream ss;
        auto old = cout.rdbuf(ss.rdbuf());
        printGraphIntersection(graph1, graph2);
        cout.rdbuf(old);
        displayText("Common Courses", ss.str());
    });

    addButton(50.f, 310.f, "Jaccard Similarity", [&]{
        double sim = computeJaccardSimilarity(graph1, graph2);
        stringstream ss;
        ss << "Jaccard Similarity: " << sim << "\n";
        displayText("Jaccard Similarity", ss.str());
    });
    addButton(420.f, 310.f, "MSc Independent Courses", [&]{
        stringstream ss;
        auto old = cout.rdbuf(ss.rdbuf());
        printIndependentCourses(graph1);
        cout.rdbuf(old);
        displayText("MSc Independent Courses", ss.str());
    });
    addButton(790.f, 310.f, "BE CSE Independent Courses", [&]{
        stringstream ss;
        auto old = cout.rdbuf(ss.rdbuf());
        printIndependentCourses(graph2);
        cout.rdbuf(old);
        displayText("BE CSE Independent Courses", ss.str());
    });

    addButton(420.f, 380.f, "Exit", [&]{ menuWindow.close(); });

    while (menuWindow.isOpen()) {
        sf::Event event;
        while (menuWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                menuWindow.close();
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mouse = menuWindow.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                for (auto& b : buttons) {
                    if (b.shape.getGlobalBounds().contains(mouse)) {
                        b.onClick();
                        break;
                    }
                }
            }
        }

        // Hover effect
        sf::Vector2i mousePixel = sf::Mouse::getPosition(menuWindow);
        sf::Vector2f mouse = menuWindow.mapPixelToCoords(mousePixel);
        for (auto& b : buttons) {
            if (b.shape.getGlobalBounds().contains(mouse)) {
                b.shape.setFillColor(sf::Color(66, 165, 245));
            } else {
                b.shape.setFillColor(sf::Color(100, 181, 246));
            }
        }

        menuWindow.clear(sf::Color::White);
        menuWindow.draw(title);
        for (auto& b : buttons) {
            menuWindow.draw(b.shape);
            menuWindow.draw(b.label);
        }
        menuWindow.display();
    }

    return 0;
}