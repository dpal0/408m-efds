#define UNIT_TESTING

#include <iostream>
#include <fstream>
#include <vector>
#include "../../src/main.cpp"

using namespace std;

int main (int argc, char** argv) {

    vector<vector<int>> img;
    loadTXT("input.txt", img);

    return 0;

}