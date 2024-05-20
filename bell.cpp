#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <limits>
#include <algorithm>


using namespace std;

struct Data {
    int sourceID;
    int targetID;
    int rating;
    long long time;

    Data(int src, int tgt, int rtg, long long t) : sourceID(src), targetID(tgt), rating(rtg), time(t) {}
};

struct Edge {
    int source, destination, weight;
};

void BellmanFord(vector<Edge>& edges, int V, int source, vector<int>& distance) {
    distance[source] = 0;

    //relax edge
    for (int i = 0; i < V - 1; ++i) {
        for (const Edge& edge : edges) {
            if (distance[edge.source] != numeric_limits<int>::max() &&
                distance[edge.source] + edge.weight < distance[edge.destination]) {
                distance[edge.destination] = distance[edge.source] + edge.weight;
            }
        }
    }
}

int main() {
    
    ifstream inputFile("values.csv");

    if (!inputFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

  
    vector<Data> dataVector;

    
    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string token;

        vector<string> fields;
        while (getline(ss, token, ',')) {
            fields.push_back(token);
        }

       
        if (fields.size() == 4) {
            try {
                int sourceID = stoi(fields[0]);
                int targetID = stoi(fields[1]);
                int rating = stoi(fields[2]);
                long long time = stoll(fields[3]);

            
                dataVector.emplace_back(sourceID, targetID, rating, time);
            }
            catch (const exception& e) {
                cerr << "Error parsing line: " << line << ", " << e.what() << endl;
            }
        }
        else {
            cerr << "Invalid number of fields in line: " << line << endl;
        }
    }

    inputFile.close();

    int V = 0; //find  vertices
    for (const auto& data : dataVector) {
        V = max({ V, data.sourceID, data.targetID });
        
    }
    cout << "no of vertices " << V << endl;
    ++V; //increment  account for zero based indexing?

    //create edges for the graph
    vector<Edge> edges;
    for (const auto& data : dataVector) {
        edges.push_back({ data.sourceID, data.targetID, data.rating });
    }

    //initialize distances => infinity
    vector<int> distance(V, numeric_limits<int>::max());

    // Choose a source vertex (you can modify this as needed)
    int sourceVertex = 0;

    // Run Bellman-Ford algorithm
    BellmanFord(edges, V, sourceVertex, distance);

    // Display and store the results
    ofstream outputFile("shortest_paths.txt");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }

    cout << "Shortest distances from the source vertex (" << sourceVertex << "):" << endl;
    for (int i = 0; i < V; ++i) {
        cout << "To vertex " << i << ": " << distance[i] << endl;
        outputFile << "To vertex " << i << ": " << distance[i] << endl;
    }

    // Close the output file
    outputFile.close();

    return 0;
}
