#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>//provides with fast retrievl of values associated with a key value

using namespace std;

struct Data {
    int sourceID;
    int targetID;
    int rating;
    long long time;

    Data(int src, int tgt, int rtg, long long t) : sourceID(src), targetID(tgt), rating(rtg), time(t) {}
};

struct Vertex {
    int id;// id of the vertex 
    int distance;

    Vertex(int i, int d) : id(i), distance(d) {}

    //custom comparator=> priority_queue
    bool operator>(const Vertex& other) const {
        return distance > other.distance;
    }
};

void dijkstra(const vector<vector<Data>>& graph, int source, vector<int>& distances) {
    // priority queue is being used to store vertices with their respective distances
    priority_queue<Vertex, vector<Vertex>, greater<Vertex>> pq;

  
    distances[source] = 0;   //setting the  initial distance for the source vertex to 0

   
    pq.push(Vertex(source, 0));  //enqueue source vert

    while (!pq.empty()) {
       
        Vertex current = pq.top();  //getting the vertex with the smallest distance from source
        pq.pop();

        int currentID = current.id;
        int currentDistance = current.distance;

        
        if (currentDistance > distances[currentID]) { 
            continue; //checking if the current distance is small than the alr stored distance
        }

        //iterate over the neighbors of the current vertex
        for (const Data& edge : graph[currentID]) {
            int neighborID = edge.targetID;
            int weight = edge.rating;  // rating = weight

            //relaxation step heree
            if (currentDistance + weight < distances[neighborID]) {
                distances[neighborID] = currentDistance + weight;
                pq.push(Vertex(neighborID, distances[neighborID]));
            }
        }
    }
}

int main() {
    //open the file
    ifstream inputFile("values.csv");

    if (!inputFile.is_open()) {
        cerr << "error opening file" << endl;
        return 1;
    }

   
    vector<Data> dataVector;  //vector => store the data

    
    string line; //read and parse each line
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string token;

        //parse each field using ','
        vector<string> fields;
        while (getline(ss, token, ',')) {
            fields.push_back(token);
        }

        //there are four fields in each col
        if (fields.size() == 4) {
            try {
                int sourceID = stoi(fields[0]);
                int targetID = stoi(fields[1]);
                int rating = stoi(fields[2]);
                long long time = stoll(fields[3]);

                //create data object and add it to the vector
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

//    //file." <<endl;
//    return 1;
//}
//string line;
//getline(file, line);
//
//for (char a : line) {
//    if (a == ',') {
//        count++;
//    }
//}
//cout << " C: " << count << endl;
//return count;
//}


  
    inputFile.close();

    //maximum vertex ID
    int maxID = 0;
    for (const auto& data : dataVector) {
        maxID = max({ maxID, data.sourceID, data.targetID });
    }

    //adjacency list
    vector<vector<Data>> graph(maxID + 1);
    for (const auto& data : dataVector) {
        graph[data.sourceID].emplace_back(data);
    }

    //set initial distances to infinity
    vector<int> distances(maxID + 1, numeric_limits<int>::max());

    //choosing  a source vertex
    int sourceVertex = 0; 

    //call
    dijkstra(graph, sourceVertex, distances);

    //printing and store the results
    ofstream outputFile("shortest_paths.txt");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }
    /// <summary>
    /// /////
    /// </summary>
    /// <returns></returns>

    for (int j = 0; j < distances.size(); ++j) {
        cout << "The shortest distance from " << sourceVertex << " to " << j << ": " << distances[j] << endl;
        outputFile << "The shortest distance from " << sourceVertex << " to " << j << ": " << distances[j] << endl;
    }

    outputFile.close();

    return 0;
}
