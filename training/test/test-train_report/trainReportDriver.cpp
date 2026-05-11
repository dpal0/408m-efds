#define UNIT_TESTING

#include <iostream>
#include <fstream>
#include <vector>
#include "../../src/train/main.cpp"

using namespace std;

int main (int argc, char** argv) {

    int sc_rounds[3] = {10, 20, 30};
    makeTrainingReport(".", 3, 12345.67, sc_rounds);
}