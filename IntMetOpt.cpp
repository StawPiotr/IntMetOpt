#include <iostream>
#include <fstream>
#include <vector>
#include "Nagłówek.h"
#include <string>
#include <sstream>
#include <cmath>


using namespace std;

class point {
public:
    float x;
    float y;
    float vector[100];
};



int main()
{
    ifstream MyReadFile("kroa100.txt");
    string myText;
    string word;
    point points[100];

    int j = 0;
    while (getline(MyReadFile, myText)) {
        cout << myText <<endl;
        istringstream iss(myText);
        int i = 0;
        while (getline(iss, word, ' ')) {
            cout << word << std::endl;
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
        j++;
    }

    MyReadFile.close();
    
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (i == j) {
                points[i].vector[j] = 0;
            }
            else {
                points[i].vector[j] = sqrt( pow((points[i].x - points[j].x),2) + pow((points[i].y - points[j].y), 2));
            }
            
        }
    }
    for (int j = 0; j < 100; j++) {
        cout << "NEXT VECTOR "<<j<<"-----------------------------------------------------------------------"<<endl;
        for (int i = 0; i < 100; i++) {
            cout << points[j].vector[i] << endl;
        }
    }


}

