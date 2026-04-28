#include "classifierActor.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include "classifierActor.h"
int main() {
    // NOTE: Replace fifo creation/init below with the exact WELT calls used in your project.
    welt_c_fifo_pointer in;
    welt_c_fifo_pointer cont;
    welt_c_fifo_pointer abortf;

    // Example placeholders:
    // in = (welt_c_fifo_pointer)malloc(sizeof(welt_c_fifo));
    // cont = (welt_c_fifo_pointer)malloc(sizeof(welt_c_fifo));
    // abortf = (welt_c_fifo_pointer)malloc(sizeof(welt_c_fifo));
    // welt_c_fifo_init(in, 16, sizeof(IIS*), 0);
    // welt_c_fifo_init(cont, 16, sizeof(IIS*), 0);
    // welt_c_fifo_init(abortf, 16, sizeof(IIS*), 0);

    classifierActor actor(in, abortf, cont, 1);

    cv::Mat* img = new cv::Mat(24, 24, CV_8U, cv::Scalar(0));

    IIS* token = new IIS;
    token->I = img;
    token->k = 1;

    // Push one token into input FIFO
    welt_c_fifo_write(in, &token);

    // Simulate scheduler
    for (int i = 0; i < 10; i++) {
        if (actor.enable()) {
            actor.invoke();
        }
    }

    std::cout << "Input FIFO population: " << welt_c_fifo_population(in) << std::endl;
    std::cout << "Continue FIFO population: " << welt_c_fifo_population(cont) << std::endl;
    std::cout << "Abort FIFO population: " << welt_c_fifo_population(abortf) << std::endl;

    return 0;
}
