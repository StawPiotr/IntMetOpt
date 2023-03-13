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
    int find_farest() {
        float max = 0;
        int max_index = 101;
        for (int i = 0; i < 100; i++) {
            if (distances[i] > max) {
                max = distances[i];
                max_index = i;
            }
        }
        return max_index;
    }
};

class line {
public:
    point first_point;
    point second_point;
    int find_nearest_point(vector<point> available_vertices) {
        float distance;
        int nearest_point = std::numeric_limits<int>::max();
        float shortest_distance = std::numeric_limits<float>::max();
        for (int i = 0; i < available_vertices.size(); i++) {
            point measured_point = available_vertices[i];            
            distance = first_point.distances[measured_point.index] + second_point.distances[measured_point.index];
            if (first_point.index != measured_point.index and second_point.index != measured_point.index) {
                if (distance < shortest_distance) {
                    shortest_distance = first_point.distances[measured_point.index] + second_point.distances[measured_point.index];
                    nearest_point = i;
                }
            }
        }
        return nearest_point;
    }
    int find_second_nearest_point(vector<point> available_vertices) {
        float distance;
        int nearest_point = std::numeric_limits<int>::max();
        int second_nearest_point = std::numeric_limits<int>::max();
        float shortest_distance = std::numeric_limits<float>::max();
        float second_shortest_distance = std::numeric_limits<float>::max();
        for (int i = 0; i < available_vertices.size(); i++) {
            point measured_point = available_vertices[i];
            distance = first_point.distances[measured_point.index] + second_point.distances[measured_point.index];
            if (first_point.index != measured_point.index and second_point.index != measured_point.index) {
                if (distance < shortest_distance) {
                    shortest_distance = first_point.distances[measured_point.index] + second_point.distances[measured_point.index];
                    nearest_point = i;
                }
                else if(distance < second_shortest_distance){
                    second_shortest_distance = first_point.distances[measured_point.index] + second_point.distances[measured_point.index];
                    second_nearest_point = i;
                }
            }
        }
        return second_nearest_point;
    }
    float shortest_distance(point best_point) {
        return first_point.distances[best_point.index] + second_point.distances[best_point.index];
    }
    line(point fp, point sp) {
        first_point = fp;
        second_point = sp;
    }
    line(){}
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

float totalDistanceFromDistanceList(vector<point> points) {
    float total_distance = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        total_distance += points[i].distances[points[i + 1].index];
    }
    return total_distance;
}

float getSumOfCyclesDistances(vector<vector<point>> cycles, int n_cycles) {
    float dist_sum = 0;
    // iterate over cycles and print distances
    for (int i = 0; i < n_cycles; i++) {
        float dist = totalDistanceFromDistanceList(cycles[i]);
        dist_sum += dist;
        //cout << "CYCLE " << i << " TOTAL DISTANCE: " <<  << endl;
    }
    return dist_sum;
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

void findNearestPoint(vector<vector<point>>& cycles, const int n_cycles, vector<point>& available_vertices) {
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

void add_point_to_cycle(vector<vector<point>>& cycles, vector<point>& available_vertices, vector<vector<line>>& allLines, int i, int best_line, int best_point) {
    line new_line(allLines[i][best_line].first_point, available_vertices[best_point]);
    line new_line2(available_vertices[best_point], allLines[i][best_line].second_point);
    available_vertices.erase(available_vertices.begin() + best_point);
    allLines[i].erase(allLines[i].begin() + best_line);
    allLines[i].insert(allLines[i].begin() + best_line, new_line);
    allLines[i].insert(allLines[i].begin() + best_line+1, new_line2);
}

void createCycles(vector<vector<point>>& cycles, vector<vector<line>>& allLines, const int n_cycles, const int n_points)
{
    for (int j = 0; j < n_cycles; j++) {
        cycles[j].clear();
        cycles[j].push_back(allLines[j][0].first_point);
        for (int i = 0; i < allLines[j].size(); i++) {
            cycles[j].push_back(allLines[j][i].second_point);
        }
    }

}



vector<vector<point>> greedyNearestNeighbor(vector<point> available_vertices, const int n_cycles, const int n_points, int ind1, int ind2) {
    vector<vector<point>> cycles;
    float best_distance = 0;

    //for (int i = 0; i < n_cycles; i++) {

    //    // randomize starting point index
    //    srand(time(NULL));
    //    int random_index = rand() % available_vertices.size();

    //    point vertex = popVertex(available_vertices, random_index);
    //    vector<point> cycle = { vertex };
    //    cycles.push_back(cycle);
    //}

    point vertex = popVertex(available_vertices, ind1);
    vector<point> cycle = { vertex };
    cycles.push_back(cycle);

    point vertex2 = popVertex(available_vertices, ind2);
    vector<point> cycle2 = { vertex2 };
    cycles.push_back(cycle2);

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

vector<vector<point>> greedyCycle(vector<point> available_vertices, const int n_cycles, const int n_points, int ind1, int ind2) {
    vector<vector<point>> cycles;
    vector<vector<line>> allLines;

    srand(time(NULL));
    //int random_index = rand() % available_vertices.size();
    //int random_index2 = rand() % available_vertices.size();
    int random_index = ind1;
    int random_index2 = ind2;
    point vertex = popVertex(available_vertices, random_index);
    vector<point> cycle = { vertex };
    cycles.push_back(cycle);

    point vertex2 = popVertex(available_vertices, random_index2);
    vector<point> cycle2 = { vertex2 };
    cycles.push_back(cycle2);

    findNearestPoint(cycles, n_cycles, available_vertices);

    line l1(cycles[0][0], cycles[0][1]);
    vector<line> lineCycle = { l1 };
    allLines.push_back(lineCycle);

    line l2(cycles[1][0], cycles[1][1]);
    vector<line> lineCycle2 = { l2 };
    allLines.push_back(lineCycle2);

    for (int i = 0; i < n_cycles; i++) {
        int point = allLines[i][0].find_nearest_point(available_vertices);
        line new_line(available_vertices[point], allLines[i][0].first_point);
        line new_line2(allLines[i][0].second_point, available_vertices[point]);
        available_vertices.erase(available_vertices.begin() + point);
        allLines[i].insert(allLines[i].begin(), new_line);
        allLines[i].push_back(new_line2);
    }
    

    while (available_vertices.size() > 0) {
        for (int i = 0; i < n_cycles; i++) {
            int best_line = 0;
            int mes_point = 0;
            int best_point = 0;
            float best_distance = std::numeric_limits<float>::max();
            for (int j = 0; j < allLines[i].size(); j++) {
                mes_point = allLines[i][j].find_nearest_point(available_vertices);
                if (allLines[i][j].shortest_distance(available_vertices[best_point]) < best_distance) {
                    best_distance = allLines[i][j].shortest_distance(available_vertices[best_point]);
                    best_point = mes_point;
                    best_line = j;
                }
            }
            add_point_to_cycle(cycles, available_vertices, allLines, i, best_line, best_point);
        }
    }
    createCycles(cycles, allLines, n_cycles, n_points);

    return cycles;
}

vector<vector<point>> regratsCycle(vector<point> available_vertices, const int n_cycles, const int n_points, int ind1, int ind2) {
    vector<vector<point>> cycles;
    vector<vector<line>> allLines;

    /*srand(time(NULL));
    int random_index = rand() % available_vertices.size();
    int random_index2 = rand() % available_vertices.size();*/
    int random_index = ind1;
    int random_index2 = ind2;
    point vertex = popVertex(available_vertices, random_index);
    vector<point> cycle = { vertex };
    cycles.push_back(cycle);

    point vertex2 = popVertex(available_vertices, random_index2);
    vector<point> cycle2 = { vertex2 };
    cycles.push_back(cycle2);

    findNearestPoint(cycles, n_cycles, available_vertices);

    line l1(cycles[0][0], cycles[0][1]);
    vector<line> lineCycle = { l1 };
    allLines.push_back(lineCycle);

    line l2(cycles[1][0], cycles[1][1]);
    vector<line> lineCycle2 = { l2 };
    allLines.push_back(lineCycle2);

    for (int i = 0; i < n_cycles; i++) {
        int point = allLines[i][0].find_nearest_point(available_vertices);
        line new_line(available_vertices[point], allLines[i][0].first_point);
        line new_line2(allLines[i][0].second_point, available_vertices[point]);
        available_vertices.erase(available_vertices.begin() + point);
        allLines[i].insert(allLines[i].begin(), new_line);
        allLines[i].push_back(new_line2);
    }


    while (available_vertices.size() > 0) {
        for (int i = 0; i < n_cycles; i++) {
            int best_line = 0;
            int best_point = 0;
            int mes_point = 0;
            int second_best_point = 0;
            float regrats = 0;
            float best_regrats = 0;
            float best_distance = std::numeric_limits<float>::max();
            for (int j = 0; j < allLines[i].size(); j++) {
                mes_point = allLines[i][j].find_nearest_point(available_vertices);
                second_best_point = allLines[i][j].find_nearest_point(available_vertices);
                regrats = available_vertices[best_point].distances[available_vertices[second_best_point].index];
                if (regrats > best_regrats) {
                    best_distance = allLines[i][j].shortest_distance(available_vertices[best_point]);
                    best_line = j;
                    best_point = mes_point;

                }
            }
            add_point_to_cycle(cycles, available_vertices, allLines, i, best_line, best_point);
        }
    }
    createCycles(cycles, allLines, n_cycles, n_points);

    return cycles;
}

void saveVector(vector<float> vector, string filename) {
		ofstream file;
		file.open(filename);
    for (int i = 0; i < vector.size(); i++) {
				file << vector[i] << endl;
		}
		file.close();
}

void appendVector(vector<float> vector, string filename) {
		ofstream file;
		file.open(filename, ios::app);
		for (int i = 0; i < vector.size(); i++) {
        file << vector[i];
		}
    file << endl;
		file.close();
}

void saveCycles(vector<vector<point>> cycles, string filename) {
		ofstream file;
		file.open(filename);
    for (int i = 0; i < cycles.size(); i++) {
        for (int j = 0; j < cycles[i].size(); j++) {
						file << cycles[i][j].index << " ";
				}
				file << endl;
		}
		file.close();
}

void appendCycles(vector<vector<point>> cycles, string filename) {
		ofstream file;
		file.open(filename, ios::app);
		for (int i = 0; i < cycles.size(); i++) {
				for (int j = 0; j < cycles[i].size(); j++) {
						file << cycles[i][j].index << " ";
				}
				file << endl;
		}
		file.close();
}

int main()
{
    const int n_points = 100;
    const int n_cycles = 2;
    string instance_filename = "krob100.txt";
    point * points = loadPoints(instance_filename, n_points);
    
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

    // create 2d vector of distances for each algorithm
    vector<vector<float>> distances;
    // add three rows to the vector
    distances.push_back(vector<float>());
    distances.push_back(vector<float>());
    distances.push_back(vector<float>());
    
    // run 100 experiments
    for (int j = 0; j < 100; j++) {

        cout << "RUN " << j << endl;
        int ind1 = ceil(j/2);
        int ind2 = ceil(n_points/3) + ceil((j/2));

        // convert points list to vector
        vector<point> points_dynamic1(points, points + n_points);
        // greedy nearest neighbor algorithm
        vector<vector<point>> cycles = greedyNearestNeighbor(points_dynamic1, n_cycles, n_points, ind1, ind2);
        float current_distance = getSumOfCyclesDistances(cycles, n_cycles);
        distances[0].push_back(current_distance);
        
        string c_filename = "cycles_" + instance_filename + "_NearestNeighbor_" + ".txt";
        string d_filename = "distances_" + instance_filename + "_NearestNeighbor_" + ".txt";
        if (j == 0) {
						saveCycles(cycles, c_filename);
            saveVector(distances[0], d_filename);
				}
        else {
						appendCycles(cycles, c_filename);
            appendVector(distances[0], d_filename);
				}
        
        cout << "   Nearest: " << current_distance << endl;


        vector<point> points_dynamic2(points, points + n_points);
        cycles = greedyCycle(points_dynamic2, n_cycles, n_points, ind1, ind2);
        current_distance = getSumOfCyclesDistances(cycles, n_cycles);
        distances[1].push_back(current_distance);

        c_filename = "cycles_" + instance_filename + "_GreedyCycle_" + ".txt";
        d_filename = "distances_" + instance_filename + "_GreedyCycle_" + ".txt";
        if (j == 0) {
            saveCycles(cycles, c_filename);
            saveVector(distances[1], d_filename);
        }
        else {
            appendCycles(cycles, c_filename);
            appendVector(distances[1], d_filename);
        }

        cout << "   GreedyCycle: " << current_distance << endl;


        vector<point> points_dynamic3(points, points + n_points);
        cycles = regratsCycle(points_dynamic3, n_cycles, n_points, ind1, ind2);
        current_distance = getSumOfCyclesDistances(cycles, n_cycles);
        distances[2].push_back(current_distance);

        c_filename = "cycles_" + instance_filename + "_RegretCycle_" + ".txt";
        d_filename = "distances_" + instance_filename + "_RegretCycle_" + ".txt";
        if (j == 0) {
            saveCycles(cycles, c_filename);
            saveVector(distances[2], d_filename);
        }
        else {
            appendCycles(cycles, c_filename);
            appendVector(distances[2], d_filename);
        }

        cout << "   RegretCycle: " << current_distance << endl;
    }

    /*for (int j = 0; j < n_points; j++) {
        cout << "NEXT VECTOR "<<j<<"-----------------------------------------------------------------------"<<endl;
        for (int i = 0; i < n_points; i++) {
            cout << points[j].distances[i] << endl;
        }
    }*/
}

