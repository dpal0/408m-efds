#define UNIT_TESTING

#include <iostream>
#include <fstream>
#include <vector>
#include "../../src/validate/main.cpp"

using namespace std;

int main (int argc, char** argv) {

    vector<double> rates = {1.0, 1.0};
    
    for (int i = 0; i < 3; i++) {
        rates = makeEvalReport ("./eval", rates[0], rates[1], 9, 2, 18, 1, 12345678.123);
    }
}