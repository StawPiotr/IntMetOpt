#include <iostream>
#include <fstream>
#include <vector>
#include "Nagłówek.h"
#include <string>
#include <sstream>
#include <cmath>
#include <iterator>


using namespace std;

class point {
public:
    float x;
    float y;
    float distances[100];
    int index;
};

point * loadPoints(string filename, const int n_points) {
    ifstream MyReadFile(filename);
    string myText;
    string word;
    point * points = new point[n_points];
    vector<point> points_dynamic;

    // Read the text file
    int j = 0;
    while (getline(MyReadFile, myText)) {
        //cout << myText << endl;
        istringstream iss(myText);
        int i = 0;
        while (getline(iss, word, ' ')) {
            //cout << word << std::endl;
            switch (i) {
            case 1:
            {
                points[j].x = stoi(word);
                break;
            }
            case 2:
            {
                points[j].y = stoi(word);
                break;
            }
            }
            i++;
        }
        points[j].index = j++;
    }

    MyReadFile.close();

    return points;
}

float totalDistance(vector<point> points) {
    float total_distance = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        total_distance += sqrt(pow((points[i].x - points[i + 1].x), 2) + pow((points[i].y - points[i + 1].y), 2));
    }
    return total_distance;
}

point popVertex(vector<point>& available_vertices, int index) {
    point vertex = available_vertices[index];
    available_vertices.erase(available_vertices.begin() + index);
    return vertex;
}

void printCycles(vector<vector<point>> cycles, const int n_cycles, const int n_points)
{
    for (int j = 0; j < n_cycles; j++) {
        cout << "NEXT CYCLE: \n";
        for (int i = 0; i < n_points / 2; i++) {
            cout << "\t" << cycles[j][i].index << ";\n";
        }
        cout << endl;
    }
}

vector<vector<point>> greedyNearestNeighbor(vector<point> available_vertices, const int n_cycles, const int n_points) {
    vector<vector<point>> cycles;
    float best_distance = 0;

    for (int i = 0; i < n_cycles; i++) {

        // randomize starting point index
        srand(time(NULL));
        int random_index = rand() % available_vertices.size();

        point vertex = popVertex(available_vertices, random_index);
        vector<point> cycle = { vertex };
        cycles.push_back(cycle);
    }

    while (available_vertices.size() > 0) {
        for (int i = 0; i < n_cycles; i++) {
            int best_index, best_j;
            float best_distance = std::numeric_limits<float>::max();
            for (int j = 0; j < available_vertices.size(); j++) {
                point measured_point = available_vertices[j];

                // use of index parameter to avoid using wrong 
                // j index after popping available_vertices
                float distance = cycles[i].back().distances[measured_point.index];
                // alternatively use distance calculation
                //float distance = totalDistance({ cycles[i].back(),  measured_point });

                if (distance != 0 && distance < best_distance) {
                    best_distance = distance;
                    best_index = measured_point.index;
                    best_j = j;
                }
            }
            point vertex = popVertex(available_vertices, best_j);
            cycles[i].push_back(vertex);
        }
    }

    return cycles;
}

int main()
{
    const int n_points = 100;
    const int n_cycles = 2;
    point * points = loadPoints("kroa100.txt", n_points);

    // add neighbor distances to points
    for (int i = 0; i < n_points; i++) {
        for (int j = 0; j < n_points; j++) {
            if (i == j) {
                points[i].distances[j] = 0;
            }
            else {
                points[i].distances[j] = sqrt( pow((points[i].x - points[j].x),2) + pow((points[i].y - points[j].y), 2));
            }
        }
    }

    // convert points list to vector
    vector<point> points_dynamic(points, points + n_points);

    vector<vector<point>> cycles = greedyNearestNeighbor(points_dynamic, n_cycles, n_points);

    printCycles(cycles, n_cycles, n_points);

    /*for (int j = 0; j < n_points; j++) {
        cout << "NEXT VECTOR "<<j<<"-----------------------------------------------------------------------"<<endl;
        for (int i = 0; i < n_points; i++) {
            cout << points[j].distances[i] << endl;
        }
    }*/
}

