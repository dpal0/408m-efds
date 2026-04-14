#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int main(int argc, char **argv) { 
    const int IMG_DIM = 24;

    ifstream infile(argv[1]);
    string line, pixel;
    int img[IMG_DIM * IMG_DIM];
    int a, b, c;

    for (int row = 0; row < IMG_DIM; row++) {
        getline(infile, line);
        stringstream ss(line);

        for (int col = 0; col < IMG_DIM; col++) {
            getline(ss, pixel, ',');

            if (row == 0 && col == 0) {
                img[row * IMG_DIM + col] = stoi(pixel);
            } else if (row == 0) {
                c = img[(row) * IMG_DIM + (col-1)];
                img[row * IMG_DIM + col] = c + stoi(pixel);
            } else if (col == 0) {
                b = img[(row-1) * IMG_DIM + (col)];
                img[row * IMG_DIM + col] = b + stoi(pixel);
            } else {
                a = img[(row-1) * IMG_DIM + (col-1)];
                b = img[(row-1) * IMG_DIM + (col)];
                c = img[(row) * IMG_DIM + (col-1)];
                img[row * IMG_DIM + col] = -a + b + c + stoi(pixel);
            }
        }
    }

    infile.close();

    remove(argv[1]);

    ofstream outfile(argv[1]);
    for (int row = 0; row < IMG_DIM; row++) {
        for (int col = 0; col < IMG_DIM; col++) {

            outfile << img[row * IMG_DIM + col];
            if (col < IMG_DIM - 1) {
                outfile << ",";
            } else {
                outfile << '\n';
            }
        }
    }

}
