#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>

using namespace std;

struct Data {
    int sourceID;
    int targetID;
    int rating;
    long long time;

    Data(int src, int tgt, int rtg, long long t) : sourceID(src), targetID(tgt), rating(rtg), time(t) {}
};

void merge(vector<Data>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Data> leftArr(arr.begin() + left, arr.begin() + left + n1);
    vector<Data> rightArr(arr.begin() + mid + 1, arr.begin() + mid + 1 + n2);

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (leftArr[i].time <= rightArr[j].time) {
            arr[k++] = leftArr[i++];
        }
        else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < n1) {
        arr[k++] = leftArr[i++];
    }

    while (j < n2) {
        arr[k++] = rightArr[j++];
    }
}

void mergeSort(vector<Data>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
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
    int lineNumber = 0;

    while (getline(inputFile, line)) {
        lineNumber++;

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
                cerr << "Error parsing line " << lineNumber << ": " << line << ", " << e.what() << endl;
            }
        }
        else {
            cerr << "Invalid number of fields in line " << lineNumber << ": " << line << endl;
        }
    }

    inputFile.close();

    // Sorting the dataVector based on the fourth column (time)
    mergeSort(dataVector, 0, dataVector.size() - 1);

    // Printing the sorted data on the terminal
    for (const auto& data : dataVector) {
        cout << "Source ID: " << data.sourceID << ", Target ID: " << data.targetID
            << ", Rating: " << data.rating << ", Time: " << data.time << endl;
    }

    // Writing the sorted data to a new CSV file
    ofstream outputFile("sorted_values.csv");

    if (!outputFile.is_open()) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }

    for (const auto& data : dataVector) {
        outputFile << data.sourceID << "," << data.targetID << "," << data.rating << "," << data.time << "\n";
    }

    outputFile.close();

    return 0;
}