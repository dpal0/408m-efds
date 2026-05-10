#define UNIT_TESTING

#include <iostream>
#include <fstream>
#include <vector>
#include "../../src/train/main.cpp"

using namespace std;

int main (int argc, char** argv) {

    vector<vector<vector<int>>> images = loadImages("inputs");

    ofstream output;
    for (std::size_t i = 0; i < images.size(); ++i) {
        output.open("output" + to_string(i) + ".txt");

        for (int row = 0; row < 24; row++) {
            for (int col = 0; col < 24; col++) {
                output << images[i][row][col];
                if (col != 23) 
                    output << ",";
            }
            output << endl;
        }

        output.close();
    }


}