#include <iostream>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "classifierActor.h"
using namespace std;
#define CAPACITY 3
int main(){
    welt_c_fifo_pointer in=welt_c_fifo_new(CAPACITY,sizeof(IIS*),0);
    welt_c_fifo_pointer abort_out=welt_c_fifo_new(CAPACITY,sizeof(IIS*),0);
    welt_c_fifo_pointer cont_out=welt_c_fifo_new(CAPACITY,sizeof(IIS*),0);
    int idx=10;
    int f=0;
    bool faceF=false;
    while(idx<=19){
        ifstream img("../../../data/testing/face/00"+to_string(idx)+".txt");
        cerr<<"test face\n";
        if(!img.is_open()){
            std::cerr<<"unable to open file\n";
            return 1;
        }
        string line;
        vector<vector<int>> pix(24, vector<int>(24));
        int row=0;
        while(getline(img,line)){
            stringstream ss(line);
            string val;
            int col=0;
            while(getline(ss,val, ',') && col <24){
                pix[row][col]=stoi(val);
                col +=1;
            }
            row +=1;
        }
        PTIS *image=new PTIS();
        image->pix=pix;
        IIS *token=new IIS();
        token->I =image;
        token->k=1;
        welt_c_fifo_write(in,&token);
        classifierActor *stage=new classifierActor(in,abort_out,cont_out,1);
        for(int i=0; i<10; i++){
            if(stage->enable()){
                stage->invoke();
            }
        }
        if (welt_c_fifo_population(cont_out) > 0) {
            IIS* outToken;
            welt_c_fifo_read(cont_out,&outToken);
            std::cout << "FACE detected, k = " << outToken->k << std::endl;
            faceF=true;
        }

        if (welt_c_fifo_population(abort_out) > 0) {
            IIS* outToken;
            welt_c_fifo_read(abort_out,&outToken);
            std::cout << "NON-FACE detected, k = " << outToken->k << std::endl;
            faceF=false;
            f++;
        }
        idx++;
    }
    if(faceF==false){
        std::cout<<"Some faceF are False\n";
        std::cout<<"number of false: "<<f<<endl;
    }else{
        std::cout<<"All faceF are True\n";
    }
    return 0;
}